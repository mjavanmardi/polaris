#include "Io/Io.h"

int main(int argc, char* argv[])
{
	if (argc != 2) 
	{
		std::cout << "Usage: " << argv[0] << " <db_prefix>\n";
		return 1;
	}
	std::string db_prefix = argv[1];
	unique_ptr<odb::database> db = create_sqlite_database  (db_prefix);
	return 0;
}