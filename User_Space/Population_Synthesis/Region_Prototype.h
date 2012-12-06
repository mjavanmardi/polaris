#pragma once


//#include "popsyn_includes.h"
//#include "zone.h"
//#include "linker.h"
//
//
//using namespace std;
//
//
////---------------------------------------------------------
////	REGION LEVEL OBJECT CLASS (i.e. PUMA, etc.)
////---------------------------------------------------------
//
//class Region : public Zone
//{
//public:
//	// constructor
//	Region(){_randseed = 0;}
//	Region(double ID, vector<int> &dim_sizes);
//	Region(const Region& obj);
//
//	// Initialization
//	void initialize(m_array Data);
//	void reweight_sample();
//
//	// Processing methods
//	bool Synthesize(double TOL, int MAX_ITER, int MAX_SELECT_ITER);
//	void start_synthesis(double TOL, int MAX_ITER, int MAX_SELECT_ITER);
//	void Join()
//	{
//		_thread.join();
//	}
//
//	// I/O methods
//	void add_zone(Zone &zone);
//	void add_sample_data(Linker &link, File_Reader &fr);
//	bool write(File_Writer &fw, File_Writer &fw_sample);
//	bool write(void);
//
//	// Property access methods
//	Zone& get_zone(int index){return _zones[index];}
//	const int Num_Zones() { return _zones.size();}
//
//private:
//	vector<Zone> _zones;
//	boost::thread _thread;
//
//	// Thread (Region) specific random number generator
//	uint _randseed;
//	Prob_Generator Rand;
//};
//
//
//#endif