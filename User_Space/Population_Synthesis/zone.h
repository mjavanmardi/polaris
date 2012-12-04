#ifndef ZONE_H
#define ZONE_H


#include "popsyn_includes.h"
#include "pop_unit.h"

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------

class Zone : public m_array<double>
{
public:
	// constructors
	Zone (){}
	Zone (double ID, vector<int> &dim_sizes);
	Zone (const Zone& obj);

	// Processing methods
	void initialize(m_array Data);
	bool IPF(double TOL, int MAX_ITER);
	bool Select_HH(int MAX_ITER, const hash_map<uint,vector<Pop_Unit>> &sample, Prob_Generator& Rand);
	void Add_Sample(const Pop_Unit& P);

	// Property access methods
	double& marginal(int dim, int index) {return _marginals[dim][index];} //marginals get/set
	const double& Id(void){return _id;} // ID get

	// I/O methods
	bool write(void);
	bool write(File_Writer &fw);
	bool write_sample(File_Writer &fw);

protected:
	// Marginal totals across categories
	vector<vector<double>> _marginals;
	// List of households in zone
	hash_map<uint, vector<Pop_Unit>> _sample;
	// Unique zone ID
	double _id;
	

private:

};

#endif