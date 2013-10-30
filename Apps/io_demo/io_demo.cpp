#include "io/database.h"
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

	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(db->begin());
	result r(db->query<polaris::io::Link>(query::true_expr));
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i.id() % 100 == 0)
			std::cout << i.id() << "\n";
	}
	t.commit();

	return 0;
}