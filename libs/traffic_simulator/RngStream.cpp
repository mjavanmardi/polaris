#include "RngStream.h"
#include <math.h>
namespace RNG_Components
{
	//-------------------------------------------------------------------------
	// Generate the next random number.
	//
	double RngStream::U01 ()
	{
		long k;
		double p1, p2, u;

		/* Component 1 */
		p1 = a12 * Cg[1] - a13n * Cg[0];
		k = static_cast<long> (p1 / m1);
		p1 -= k * m1;
		if (p1 < 0.0) p1 += m1;
		Cg[0] = Cg[1]; Cg[1] = Cg[2]; Cg[2] = p1;

		/* Component 2 */
		p2 = a21 * Cg[5] - a23n * Cg[3];
		k = static_cast<long> (p2 / m2);
		p2 -= k * m2;
		if (p2 < 0.0) p2 += m2;
		Cg[3] = Cg[4]; Cg[4] = Cg[5]; Cg[5] = p2;

		/* Combination */
		u = ((p1 > p2) ? (p1 - p2) * norm : (p1 - p2 + m1) * norm);

		return (anti == false) ? u : (1 - u);
	}


	//-------------------------------------------------------------------------
	// Generate the next random number with extended (53 bits) precision.
	//
	double RngStream::U01d ()
	{
		double u;
		u = U01();
		if (anti) {
			// Don't forget that U01() returns 1 - u in the antithetic case
			u += (U01() - 1.0) * fact;
			return (u < 0.0) ? u + 1.0 : u;
		} else {
			u += U01() * fact;
			return (u < 1.0) ? u : (u - 1.0);
		}
	}

	double RngStream::triangular_random_variate(double u, double a, double b, double mean)
	{
		if (mean == 0.0) return 0.0;

		double x = 0.0;
		double c = 3.0*mean - a - b;
		double fc = (c - a) / (b - a);
	
		if (u < fc)
		{
			x = a + sqrt(u*(b - a)*(c - a));
		}
		else
		{
			x = b - sqrt((1 - u)*(b - a)*(b - c));
		}

		return x;
	}

	//*************************************************************************
	// Public members of the class start here


	//-------------------------------------------------------------------------
	// The default seed of the package; will be the seed of the first
	// declared RngStream, unless SetPackageSeed is called.
	//
	double RngStream::nextSeed[6] =
	{
	   12345.0, 12345.0, 12345.0, 12345.0, 12345.0, 12345.0
	};


	//-------------------------------------------------------------------------
	// constructor
	//
	RngStream::RngStream ()
	{
	   anti = false;
	   incPrec = false;

	   /* Information on a stream. The arrays {Cg, Bg, Ig} contain the current
	   state of the stream, the starting state of the current SubStream, and the
	   starting state of the stream. This stream generates antithetic variates
	   if anti = true. It also generates numbers with extended precision (53
	   bits if machine follows IEEE 754 standard) if incPrec = true. nextSeed
	   will be the seed of the next declared RngStream. */

	   for (int i = 0; i < 6; ++i) {
		  Bg[i] = Cg[i] = Ig[i] = nextSeed[i];
	   }

	   MatVecModM (A1p127, nextSeed, nextSeed, m1);
	   MatVecModM (A2p127, &nextSeed[3], &nextSeed[3], m2);
	}


	//-------------------------------------------------------------------------
	// Reset Stream to beginning of Stream.
	//
	void RngStream::ResetStartStream ()
	{
	   for (int i = 0; i < 6; ++i)
		  Cg[i] = Bg[i] = Ig[i];
	}


	//-------------------------------------------------------------------------
	// Reset Stream to beginning of SubStream.
	//
	void RngStream::ResetStartSubstream ()
	{
	   for (int i = 0; i < 6; ++i)
		  Cg[i] = Bg[i];
	}


	//-------------------------------------------------------------------------
	// Reset Stream to NextSubStream.
	//
	void RngStream::ResetNextSubstream ()
	{
	   MatVecModM(A1p76, Bg, Bg, m1);
	   MatVecModM(A2p76, &Bg[3], &Bg[3], m2);
	   for (int i = 0; i < 6; ++i)
		   Cg[i] = Bg[i];
	}


	//-------------------------------------------------------------------------
	bool RngStream::SetPackageSeed (const unsigned long seed[6])
	{
	   if (CheckSeed (seed))
		  return false;                   // FAILURE     
	   for (int i = 0; i < 6; ++i)
		  nextSeed[i] = seed[i];
	   return true;                       // SUCCESS
	}


	//-------------------------------------------------------------------------
	bool RngStream::SetSeed (const unsigned long seed[6])
	{
	   if (CheckSeed (seed))
		  return false;                   // FAILURE     
	   for (int i = 0; i < 6; ++i)
		  Cg[i] = Bg[i] = Ig[i] = seed[i];
	   return true;                       // SUCCESS
	}

	bool RngStream::SetSeed(const unsigned long iseed)
	{
	   unsigned long seed[6];
	   for (int i=0;i<6;++i)
		   seed[i] = iseed+i;

	   if (CheckSeed (seed))
		  return false;                   // FAILURE     
	   for (int i = 0; i < 6; ++i)
		  Cg[i] = Bg[i] = Ig[i] = seed[i];
	   return true;                       // SUCCESS

	}

	//-------------------------------------------------------------------------
	// if e > 0, let n = 2^e + c;
	// if e < 0, let n = -2^(-e) + c;
	// if e = 0, let n = c.
	// Jump n steps forward if n > 0, backwards if n < 0.
	//
	void RngStream::AdvanceState (long e, long c)
	{
		double B1[3][3], C1[3][3], B2[3][3], C2[3][3];

		if (e > 0) {
			MatTwoPowModM (A1p0, B1, m1, e);
			MatTwoPowModM (A2p0, B2, m2, e);
		} else if (e < 0) {
			MatTwoPowModM (InvA1, B1, m1, -e);
			MatTwoPowModM (InvA2, B2, m2, -e);
		}

		if (c >= 0) {
			MatPowModM (A1p0, C1, m1, c);
			MatPowModM (A2p0, C2, m2, c);
		} else {
			MatPowModM (InvA1, C1, m1, -c);
			MatPowModM (InvA2, C2, m2, -c);
		}

		if (e) {
			MatMatModM (B1, C1, C1, m1);
			MatMatModM (B2, C2, C2, m2);
		}

		MatVecModM (C1, Cg, Cg, m1);
		MatVecModM (C2, &Cg[3], &Cg[3], m2);
	}


	//-------------------------------------------------------------------------
	void RngStream::GetState (unsigned long seed[6]) const
	{
	   for (int i = 0; i < 6; ++i)
		  seed[i] = static_cast<unsigned long> (Cg[i]);
	}


	//-------------------------------------------------------------------------
	void RngStream::WriteState () const
	{
		cout << "The current state of the Rngstream";
		cout << ":\n   Cg = { ";

		for (int i = 0; i < 5; i++) {
			cout << static_cast<unsigned long> (Cg [i]) << ", ";
		}
		cout << static_cast<unsigned long> (Cg [5]) << " }\n\n";
	}


	//-------------------------------------------------------------------------
	void RngStream::WriteStateFull () const
	{
		int i;

		cout << "The RngStream";
		cout << ":\n   anti = " << (anti ? "true" : "false") << "\n";
		cout << "   incPrec = " << (incPrec ? "true" : "false") << "\n";

		cout << "   Ig = { ";
		for (i = 0; i < 5; i++) {
			cout << static_cast<unsigned long> (Ig [i]) << ", ";
		}
		cout << static_cast<unsigned long> (Ig [5]) << " }\n";

		cout << "   Bg = { ";
		for (i = 0; i < 5; i++) {
			cout << static_cast<unsigned long> (Bg [i]) << ", ";
		}
		cout << static_cast<unsigned long> (Bg [5]) << " }\n";

		cout << "   Cg = { ";
		for (i = 0; i < 5; i++) {
			cout << static_cast<unsigned long> (Cg [i]) << ", ";
		}
		cout << static_cast<unsigned long> (Cg [5]) << " }\n\n";
	}


	//-------------------------------------------------------------------------
	void RngStream::IncreasedPrecis (bool incp)
	{
	   incPrec = incp;
	}


	//-------------------------------------------------------------------------
	void RngStream::SetAntithetic (bool a)
	{
	   anti = a;
	}


	//-------------------------------------------------------------------------
	// Generate the next random number.
	//
	double RngStream::RandU01 ()
	{
	   if (incPrec)
		  return U01d();
	   else
		  return U01();
	}


	//-------------------------------------------------------------------------
	// Generate the next random integer.
	//
	int RngStream::RandInt (int low, int high)
	{
		return low + static_cast<int> ((high - low + 1.0) * RandU01 ());
	};

	StlRngStream::StlRngStream():generator(1u),distribution(0.0,1.0)
	{
		cout << "inside StlRngStream constructor" << endl;
	}

	double StlRngStream::RandU01()
	{
		return distribution(generator);
	}

	void StlRngStream::SetSeed(unsigned long iseed)
	{
		generator.seed(iseed);
	}

	double StlRngStream::triangular_random_variate(double u, double a, double b, double mean)
	{
		if (mean == 0.0) return 0.0;

		double x = 0.0;
		double c = 3.0*mean - a - b;
		double fc = (c - a) / (b - a);
	
		if (u < fc)
		{
			x = a + sqrt(u*(b - a)*(c - a));
		}
		else
		{
			x = b - sqrt((1 - u)*(b - a)*(b - c));
		}

		return x;
	}

}
