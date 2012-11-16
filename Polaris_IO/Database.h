// file      : access/database.hxx
// copyright : not copyrighted - public domain

//
// Create concrete database instance based on the DATABASE_* macros.
//

#ifndef DATABASE
#define DATABASE

#ifndef DATABASE_SQLITE
#define DATABASE_SQLITE
#endif

#include <string>
#include <memory>   // std::auto_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <odb/database.hxx>

#if defined(DATABASE_MYSQL)
#  include <odb/mysql/database.hxx>
#elif defined(DATABASE_SQLITE)
#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>
#include <odb/sqlite//exceptions.hxx>
#elif defined(DATABASE_PGSQL)
#  include <odb/pgsql/database.hxx>
#elif defined(DATABASE_ORACLE)
#  include <odb/oracle/database.hxx>
#elif defined(DATABASE_MSSQL)
#  include <odb/mssql/database.hxx>
#else
#  error unknown database; did you forget to define the DATABASE_* macros?
#endif

using namespace std;
using namespace odb::core;

inline std::auto_ptr<odb::database> open_sqlite_database(const std::string& name)
{
	auto_ptr<database> db (new odb::sqlite::database (name, SQLITE_OPEN_READWRITE));
	return db;
}

inline auto_ptr<database> create_sqlite_database(const string& name)
{
	try
	{
		auto_ptr<database> db (new odb::sqlite::database (name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
		// Create the database schema. Due to bugs in SQLite foreign key
		// support for DDL statements, we need to temporarily disable
		// foreign keys.
		connection_ptr c (db->connection ());
		c->execute ("PRAGMA foreign_keys=OFF");
		transaction t (c->begin ());
		schema_catalog::create_schema (*db);
		t.commit ();
		c->execute ("PRAGMA foreign_keys=ON");
		return db;
	}
	catch (odb::sqlite::database_exception e)
	{
		cout << e.message() << "\n";
		exit(0);
	}

}
inline auto_ptr<database> create_database (int& argc, char* argv[])
{
  if (argc > 1 && argv[1] == string ("--help"))
  {
    cout << "Usage: " << argv[0] << " [options]" << endl
         << "Options:" << endl;

#if defined(DATABASE_MYSQL)
    odb::mysql::database::print_usage (cout);
#elif defined(DATABASE_SQLITE)
    odb::sqlite::database::print_usage (cout);
#elif defined(DATABASE_PGSQL)
    odb::pgsql::database::print_usage (cout);
#elif defined(DATABASE_ORACLE)
    odb::oracle::database::print_usage (cout);
#elif defined(DATABASE_MSSQL)
    odb::mssql::database::print_usage (cout);
#endif

    exit (0);
  }

#if defined(DATABASE_MYSQL)
  auto_ptr<database> db (new odb::mysql::database (argc, argv));
#elif defined(DATABASE_SQLITE)
  auto_ptr<database> db (
    new odb::sqlite::database (
      argc, argv, false, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
	// Create the database schema. Due to bugs in SQLite foreign key
	// support for DDL statements, we need to temporarily disable
	// foreign keys.
	{
		connection_ptr c (db->connection ());
		c->execute ("PRAGMA foreign_keys=OFF");
		transaction t (c->begin ());
		schema_catalog::create_schema (*db);
		t.commit ();
		c->execute ("PRAGMA foreign_keys=ON");
	}
#elif defined(DATABASE_PGSQL)
  auto_ptr<database> db (new odb::pgsql::database (argc, argv));
#elif defined(DATABASE_ORACLE)
  auto_ptr<database> db (new odb::oracle::database (argc, argv));
#elif defined(DATABASE_MSSQL)
  auto_ptr<database> db (new odb::mssql::database (argc, argv));
#endif

  return db;
}

#endif // DATABASE_HXX
