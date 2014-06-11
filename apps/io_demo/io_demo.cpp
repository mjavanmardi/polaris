#include "io/database.h"
#include "spatial/spatial.h"
#include "io/Io.h"
int main( int argc, char** argv)
{
	if (argc != 2) 
	{
		std::cout << "Usage: " << argv[0] << "database_prefix\n";
		return 1;
	}
	char *db_path;
	unique_ptr<odb::database> db;
	db_path = argv[1];
	db = open_sqlite_database(db_path);
	//typedef odb::query<polaris::io::Gtfs_Stop> query;
	//typedef odb::result<polaris::io::Gtfs_Stop> result;
	odb::transaction t(db->begin());
	try
	{
		//result r(db->query<polaris::io::Gtfs_Stop>(query::true_expr));
	}
	catch (const odb::exception& e)
	{
		std::cerr << e.what () << std::endl;
		return 1;
	}
	//result r(db->query<polaris::io::Gtfs_Stop>(query::true_expr));
	int count = 0;
	//for (result::iterator i (r.begin()); i!=r.end(); ++i)
	//{
	//	count++;
	//	if (count%100 == 0)
	//	{

	//		try
	//		{
	//			std::cout <<i->getStop_Id() << " " << i->getStop_Name() << "\n" ;
	//		}
	//		catch (const odb::exception& e)
	//		{
	//			std::cerr << e.what () << std::endl;
	//			return 1;
	//		}
	//	}
	//}
	t.commit();

	std::map<std::string, polaris::io::shape_geometry> geom = polaris::io::GetPointGeometry(db_path, "gtfs_stops", "location", "stop_id", 26916, "gtfs");
	count=0;
	for (auto it=geom.cbegin(); it!=geom.cend();++it)
	{
		count++;
		if (count%100 == 0)
		{

			try
			{
				std::cout << "Stop: " << it->first << "; Coordinates " << it->second.x << ", " << it->second.y <<"\n";
			}
			catch (const odb::exception& e)
			{
				std::cerr << e.what () << std::endl;
				return 1;
			}
		}
	}


	return 0;
}