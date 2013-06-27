#include "Tmc.h"
#include "tmc/WeakCC.h"
#include <time.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv)
{
	using namespace std;
	using namespace polaris;
	if (argc != 2) 
	{
		std::cout << "Usage: " << argv[0] << "<db_prefix>\n";
		return 1;
	}
	string db_prefix = argv[1];
	//Matrix mat;
	//int res = read_probability_factors(db_prefix, mat);
	//cout << "Matrix size is: " << mat.M().size() << "\n";
	//polaris::Digraph g, g_conn;
	//populate_graph(db_prefix, g);
	//populate_graph_conn(db_prefix, g_conn);
	//cout << "Number of edges in the graph: " << g.E() << "\n";
	//cout << "Number of edges in the conn graph: " << g_conn.E() << "\n";
	//clock_t now = clock();
	//WeakCC* wcc = new WeakCC();
	//wcc->Run(g);

	//clock_t later = clock();
	//printf("WekCC::Run time:\t%.3f \n",(double)(later-now)/(double)CLOCKS_PER_SEC);
	//cout << "Number of weak connected components in the graph: " << wcc->Count() << "\n";

	///// Example on how to train Detectors
	//std::unordered_map<polaris::LinkID, polaris::Detector1DU<double> > out_link_detectors;
	//tain_detector(db_prefix, out_link_detectors);

	//cout << "Number of detectors: " << out_link_detectors.size() << "\n";
	std::vector<std::vector<double> > centers;
	accident_clustering(db_prefix, 10, centers);
	for (auto it = centers.begin(); it != centers.end(); ++it)
	{
		std::cout << "Center:\t\t";
		for (auto it1 = it->begin(); it1 != it->end(); ++it1)
			std::cout << *it1 << "\t\t";
		std::cout << "\n";
	}



	return 0;

}
