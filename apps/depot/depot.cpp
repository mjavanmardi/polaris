#include "Io/Io.h"
#include "spatial/spatial.h"
#include "tmc/Clustering.h"
#include "tmc/Tmc.h"

int PopulateDepotLink(const std::string db_prefix)
{
	//run the clustering on Microsoft_Events and put results into a temporary table
	std::vector<std::vector<double> >  event_points;
	char *err_msg = NULL;
	int ret;
		polaris::KMClustering kmc;
	sqlite3* db_handle =  open_spatialite_database(db_prefix, false);
	ret = sqlite3_exec (db_handle, "drop table if exists event_centers_temp", NULL, NULL, &err_msg); if (ret != SQLITE_OK) {hadnle_sqlite_error(&err_msg, ret); goto stop;}
	ret = sqlite3_exec (db_handle, "create table event_centers_temp (id integer)", NULL, NULL, &err_msg); if (ret != SQLITE_OK) {hadnle_sqlite_error(&err_msg, ret); goto stop;}
	ret =  polaris::io::AddGeometryCoumn(db_handle, 4326, "event_centers_temp", "LINESTRING", "XY"); if (ret != SQLITE_OK) {hadnle_sqlite_error(&err_msg, ret); goto stop;}
	polaris::io::GetMicrosoftEventPoints(db_handle, event_points);
	std::cout << "Number of events: " << event_points.size() << "\n";
	ret = sqlite3_exec (db_handle, "drop table event_centers_temp", NULL, NULL, &err_msg); if (ret != SQLITE_OK) {hadnle_sqlite_error(&err_msg, ret); goto stop;}
	ret = sqlite3_close(db_handle);
	// run the clustering algorithm on the points
	kmc.Run(event_points, 20);
	kmc.PrintCenters();

stop:
	return ret;
}


int main(int argc, char* argv[])
{
	using namespace polaris::io;
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <db_prefix> <k>\n";
		exit(1);
	}
	std::string db_name = argv[1];
	int k = std::atoi(argv[2]);
	//check if file exists
	std::fstream test_fh(make_name(db_name, "Supply"));
	if (!test_fh.good()) {
		std::cout << "File " << make_name(db_name, "Network_Event") << " does not exist\n";
		exit(1);
	}
	else test_fh.close();
	//PopulateDepotLink(db_name);
	std::vector<std::vector<double> > centers;
	polaris::accident_clustering(db_name, k, centers);
	return 0;
}