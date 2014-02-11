#pragma once
#include <random>
#include <iostream>
#include <string>
namespace RNG_Components
{
	class RngStream
	{
	public:

	RngStream ();


	static bool SetPackageSeed (const unsigned long seed[6]);


	void ResetStartStream ();


	void ResetStartSubstream ();


	void ResetNextSubstream ();


	void SetAntithetic (bool a);


	void IncreasedPrecis (bool incp);


	bool SetSeed (const unsigned long seed[6]);

	bool SetSeed(const unsigned long iseed);

	void AdvanceState (long e, long c);


	void GetState (unsigned long seed[6]) const;


	void WriteState () const;


	void WriteStateFull () const;


	double RandU01 ();

	double triangular_random_variate(double u, double a, double b, double mean);

	int RandInt (int i, int j);



	private:

	double Cg[6], Bg[6], Ig[6];


	bool anti, incPrec;




	static double nextSeed[6];


	double U01 ();


	double U01d ();


	};

	class StlRngStream
	{
	public:

	StlRngStream();
	std::mt19937_64 generator;  
	uniform_real_distribution<double> distribution;
	double RandU01();
	void SetSeed(unsigned long iseed);
	double triangular_random_variate(double u, double a, double b, double mean);
	};
 
	/***********************************************************************\
	 *
	 * File:           RngStream.cpp for multiple streams of Random Numbers
	 * Language:       C++ (ISO 1998)
	 * Copyright:      Pierre L'Ecuyer, University of Montreal
	 * Notice:         This code can be used freely for personal, academic,
	 *                 or non-commercial purposes. For commercial purposes, 
	 *                 please contact P. L'Ecuyer at: lecuyer@iro.umontreal.ca
	 * Date:           14 August 2001
	 *
	\***********************************************************************/


	#include <cstdlib>
	#include <iostream>
	using namespace std;

	namespace
	{
	static const double m1   =       4294967087.0;
	static const double m2   =       4294944443.0;
	static const double norm =       1.0 / (m1 + 1.0);
	static const double a12  =       1403580.0;
	static const double a13n =       810728.0;
	static const double a21  =       527612.0;
	static const double a23n =       1370589.0;
	static const double two17 =      131072.0;
	static const double two53 =      9007199254740992.0;
	static const double fact =       5.9604644775390625e-8;     /* 1 / 2^24  */

	// The following are the transition matrices of the two MRG components
	// (in matrix form), raised to the powers -1, 1, 2^76, and 2^127, resp.

	static const double InvA1[3][3] = {          // Inverse of A1p0
		   { 184888585.0,   0.0,  1945170933.0 },
		   {         1.0,   0.0,           0.0 },
		   {         0.0,   1.0,           0.0 }
		   };

	static const double InvA2[3][3] = {          // Inverse of A2p0
		   {      0.0,  360363334.0,  4225571728.0 },
		   {      1.0,          0.0,           0.0 },
		   {      0.0,          1.0,           0.0 }
		   };

	static const double A1p0[3][3] = {
		   {       0.0,        1.0,       0.0 },
		   {       0.0,        0.0,       1.0 },
		   { -810728.0,  1403580.0,       0.0 }
		   };

	static const double A2p0[3][3] = {
		   {        0.0,        1.0,       0.0 },
		   {        0.0,        0.0,       1.0 },
		   { -1370589.0,        0.0,  527612.0 }
		   };

	static const double A1p76[3][3] = {
		   {      82758667.0, 1871391091.0, 4127413238.0 },
		   {    3672831523.0,   69195019.0, 1871391091.0 },
		   {    3672091415.0, 3528743235.0,   69195019.0 }
		   };

	static const double A2p76[3][3] = {
		   {    1511326704.0, 3759209742.0, 1610795712.0 },
		   {    4292754251.0, 1511326704.0, 3889917532.0 },
		   {    3859662829.0, 4292754251.0, 3708466080.0 }
		   };

	static const double A1p127[3][3] = {
		   {    2427906178.0, 3580155704.0,  949770784.0 },
		   {     226153695.0, 1230515664.0, 3580155704.0 },
		   {    1988835001.0,  986791581.0, 1230515664.0 }
		   };

	static const double A2p127[3][3] = {
		   {    1464411153.0,  277697599.0, 1610723613.0 },
		   {      32183930.0, 1464411153.0, 1022607788.0 },
		   {    2824425944.0,   32183930.0, 2093834863.0 }
		   };



	//-------------------------------------------------------------------------
	// Return (a*s + c) MOD m; a, s, c and m must be < 2^35
	//
	static double MultModM (double a, double s, double c, double m)
	{
		double v;
		long a1;

		v = a * s + c;

		if (v >= two53 || v <= -two53) {
			a1 = static_cast<long> (a / two17);    a -= a1 * two17;
			v  = a1 * s;
			a1 = static_cast<long> (v / m);     v -= a1 * m;
			v = v * two17 + a * s + c;
		}

		a1 = static_cast<long> (v / m);
		/* in case v < 0)*/
		if ((v -= a1 * m) < 0.0) return v += m;   else return v;
	}


	//-------------------------------------------------------------------------
	// Compute the vector v = A*s MOD m. Assume that -m < s[i] < m.
	// Works also when v = s.
	//
	static void MatVecModM (const double A[3][3], const double s[3], double v[3],
					 double m)
	{
		int i;
		double x[3];               // Necessary if v = s

		for (i = 0; i < 3; ++i) {
			x[i] = MultModM (A[i][0], s[0], 0.0, m);
			x[i] = MultModM (A[i][1], s[1], x[i], m);
			x[i] = MultModM (A[i][2], s[2], x[i], m);
		}
		for (i = 0; i < 3; ++i)
			v[i] = x[i];
	}


	//-------------------------------------------------------------------------
	// Compute the matrix C = A*B MOD m. Assume that -m < s[i] < m.
	// Note: works also if A = C or B = C or A = B = C.
	//
	static void MatMatModM (const double A[3][3], const double B[3][3],
					 double C[3][3], double m)
	{
		int i, j;
		double V[3], W[3][3];

		for (i = 0; i < 3; ++i) {
			for (j = 0; j < 3; ++j)
				V[j] = B[j][i];
			MatVecModM (A, V, V, m);
			for (j = 0; j < 3; ++j)
				W[j][i] = V[j];
		}
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				C[i][j] = W[i][j];
	}


	//-------------------------------------------------------------------------
	// Compute the matrix B = (A^(2^e) Mod m);  works also if A = B. 
	//
	static void MatTwoPowModM (const double A[3][3], double B[3][3], double m, long e)
	{
	   int i, j;

	   /* initialize: B = A */
	   if (A != B) {
		  for (i = 0; i < 3; ++i)
			 for (j = 0; j < 3; ++j)
				B[i][j] = A[i][j];
	   }
	   /* Compute B = A^(2^e) mod m */
	   for (i = 0; i < e; i++)
		  MatMatModM (B, B, B, m);
	}


	//-------------------------------------------------------------------------
	// Compute the matrix B = (A^n Mod m);  works even if A = B.
	//
	static void MatPowModM (const double A[3][3], double B[3][3], double m, long n)
	{
		int i, j;
		double W[3][3];

		/* initialize: W = A; B = I */
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j) {
				W[i][j] = A[i][j];
				B[i][j] = 0.0;
			}
		for (j = 0; j < 3; ++j)
			B[j][j] = 1.0;

		/* Compute B = A^n mod m using the binary decomposition of n */
		while (n > 0) {
			if (n % 2) MatMatModM (W, B, B, m);
			MatMatModM (W, W, W, m);
			n /= 2;
		}
	}


	//-------------------------------------------------------------------------
	// Check that the seeds are legitimate values. Returns 0 if legal seeds,
	// -1 otherwise.
	//
	static int CheckSeed (const unsigned long seed[6])
	{
		int i;

		for (i = 0; i < 3; ++i) {
			if (seed[i] >= m1) {
				cerr << "****************************************\n"
					 << "ERROR: Seed[" << i << "] >= 4294967087, Seed is not set."
					 << "\n****************************************\n\n";
				return (-1);
			}
		}
		for (i = 3; i < 6; ++i) {
			if (seed[i] >= m2) {
				cerr << "*****************************************\n"
					 << "ERROR: Seed[" << i << "] >= 4294944443, Seed is not set."
					 << "\n*****************************************\n\n";
				return (-1);
			}
		}
		if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) {
			 cerr << "****************************\n"
				  << "ERROR: First 3 seeds = 0.\n"
				  << "****************************\n\n";
			 return (-1);
		}
		if (seed[3] == 0 && seed[4] == 0 && seed[5] == 0) {
			 cerr << "****************************\n"
				  << "ERROR: Last 3 seeds = 0.\n"
				  << "****************************\n\n";
			 return (-1);
		}

		return 0;
	}

	} // end of anonymous namespace
}

