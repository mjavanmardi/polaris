#include <Io\Io.h>
#include <iostream>

#include "Component.h"
#include "Instance.h"
#include "PopulateEvent.h"

//#ifdef _DEBUG
//#pragma comment(lib, "odb-sqlite-d-mt.lib")
//#pragma comment(lib, "sqlite3-d-mt.lib")
//#pragma comment(lib, "odb-d-mt.lib")
//#else
//#pragma comment(lib, "odb-sqlite-mt.lib")
//#pragma comment(lib, "sqlite3-mt.lib")
//#pragma comment(lib, "odb-mt.lib")
//#endif


int main(int argc, char* argv[])
{
	using namespace polaris::io;
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " path_to_db\n";
		exit(1);
	}
	std::string db_name = argv[1];
	//check if file exists
	std::fstream test_fh(make_name(db_name, "Network_Event"));
	if (!test_fh.good()) {
		std::cout << "File " << make_name(db_name, "Network_Event") << " does not exist\n";
		exit(1);
	}
	else test_fh.close();

	//check if file exists
	std::fstream test_fh1(make_name(db_name, "Its"));
	if (!test_fh.good()) {
		std::cout << "File " << make_name(db_name, "Its") << " does not exist\n";
		exit(1);
	}
	else test_fh.close();
	PopulateComponent(argv[1]);
	PopulateInstance(argv[1]);
	PopulateEvent(argv[1]);
	return 0;
}