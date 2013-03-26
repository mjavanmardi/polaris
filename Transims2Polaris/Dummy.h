#include "Io\Io.h"

//this method causes empty tables to be created
void Dummy(std::string db_path)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db (open_sqlite_database (db_path));
	odb::transaction t (db->begin ());
	shared_ptr<Component> comp (db->find<Component>(1));
	shared_ptr<Link_Delay> ld (db->find<Link_Delay>(1));
	t.commit();
}