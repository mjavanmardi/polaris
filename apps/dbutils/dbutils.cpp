#include "io/database.h"
#include "io/Io_db.h"
#include <string>
int main( int argc, char** argv)
{
	if (argc < 3) 
	{
		std::cout << "Usage: " << argv[0] << "command database_prefix\n";
		return 1;
	}
	char *db_path;
	unique_ptr<odb::database> db;
	db_path = argv[2];
	std::string command  (argv[1]);

	if (command.compare("create") == 0)
	{
		std::cout <<  "Creating a new database: " << db_path << "\n";
		db = create_sqlite_database(db_path);
	}
	return 0;
}