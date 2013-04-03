// file      : access/database.hxx
// copyright : not copyrighted - public domain

//
// Create concrete database instance based on the DATABASE_* macros.
//

#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 )

#define SCHEMA_REVISION "11"

#ifndef DATABASE
#define DATABASE

#include <string>
#include <memory>   // auto_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <odb/database.hxx>
#include <odb/connection.hxx>
#include <odb/sqlite/connection.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx> 
#include <sqlite3.h>

#include "Demand-odb.hxx"
#include "Supply-odb.hxx"
#include "System-odb.hxx"
#include "Result-odb.hxx"

#include <iostream>
#include <fstream>

using namespace std;

//#pragma warning(disable:4099)
//
//#ifdef _DEBUG
//#pragma comment(lib, "odb-sqlite-d-mt.lib")
//#pragma comment(lib, "sqlite3-d-mt.lib")
//#pragma comment(lib, "odb-d-mt.lib")
//#pragma comment(lib, "odb-sqlite-d.lib")
//#pragma comment(lib, "sqlite3-d.lib")
//#pragma comment(lib, "odb-d.lib")
//#else
//#pragma comment(lib, "odb-sqlite-mt.lib")
//#pragma comment(lib, "sqlite3-mt.lib")
//#pragma comment(lib, "odb-mt.lib")
//#pragma comment(lib, "odb-sqlite.lib")
//#pragma comment(lib, "sqlite3.lib")
//#pragma comment(lib, "odb.lib")
//#endif



namespace polaris
{
namespace io
{
	// This method returns a vector that contains named schemas available in the IO library. Note, this list is duplicated in db-inventory.txt file. 
	// create-io-odb-step.py script uses  db-inventory.txt file to generate the project file wiht the required pre-build steps (obd compiler). So, make sure that the list here and in the file are in SYNC!
	// I know not the best design, but will be changed to something better in the future
	static std::vector<string> get_schemas()
	{		
		std::vector<string> result;
		result.push_back("Supply");
		result.push_back("Result");
		result.push_back("Demand");
		result.push_back("System");
		result.push_back("Its");
		result.push_back("Network_Event");
		assert(result.size() > 0);
		return result;
	}
	static std::vector<string> db_inventory = get_schemas();
	static string make_name(string db_name, string schema_name)
	{
		return db_name+"-"+schema_name+".sqlite";
	}
	static string make_attach_string(string db_name, string schema_name)
	{
		return "ATTACH \'" + make_name(db_name, schema_name) + "\' as " + schema_name;
	}
}}
inline sqlite3* open_raw_sqlite_database(const std::string& name)
{
	using namespace polaris::io;
	int ret;
	//char sql[1024];
	char *err_msg = NULL;
	sqlite3* db_handle;
	ret = sqlite3_open_v2(make_name(name,"Supply").c_str(), &db_handle, SQLITE_OPEN_READWRITE , NULL);
	if (ret != SQLITE_OK)
	{
		fprintf (stderr, "Error: %s\n", err_msg);
		fprintf (stdout, "Error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return NULL;
	}
	ifstream test;
	if (db_inventory.size()>1)
	{
		for (vector<string>::iterator it = db_inventory.begin()+1; it != db_inventory.end(); ++it)
		{
			test.open(make_name(name, *it).c_str());

			if(test.is_open())
			{
				ret = sqlite3_exec (db_handle, make_attach_string(name, *it).c_str(), NULL, NULL, &err_msg);
				if (ret != SQLITE_OK)
				{
					fprintf (stderr, "Error: %s\n", err_msg);
					sqlite3_free (err_msg);
					return NULL;
				}
			}
			test.close();
			
		}
	}
	return db_handle;
}

inline int attach_spatialite(sqlite3* db_handle, bool init) 
{
	using namespace polaris::io;
	char sql[2048];
	//char buff[1024];
	char *err_msg = NULL;
	int ret;
	sqlite3_enable_load_extension (db_handle, 1);
	#ifdef WINDOWS
	strcpy (sql, "SELECT load_extension('libspatialite-4.dll')");
	#else
	strcpy (sql, "SELECT load_extension('libspatialite.so')");
	#endif
	ret = sqlite3_exec (db_handle, sql, NULL, NULL, &err_msg);
	if (ret != SQLITE_OK)
	{
		fprintf (stderr, "load_extension() error: %s\n", err_msg);
		sqlite3_free (err_msg);
		return ret;
	}
	if (init) {
		strcpy (sql, "SELECT InitSpatialMetadata()");
		ret = sqlite3_exec (db_handle, sql, NULL, NULL, &err_msg);
		if (ret != SQLITE_OK)
		{
			fprintf (stderr, "InitSpatialMetadata() error: %s\n", err_msg);
			sqlite3_free (err_msg);
			return ret;
		}
	}
		//fprintf(stderr, "\n\n**** SpatiaLite loaded as an extension ***\n\n");

	return ret;
}

inline sqlite3* open_spatialite_database(const std::string& name, bool init = true)
{
	using namespace polaris::io;
	int ret;
	char *err_msg = NULL;
	sqlite3* db_handle;
	db_handle = open_raw_sqlite_database(name);
	assert(db_handle != NULL);
	ret = attach_spatialite(db_handle, init);
	assert(ret == SQLITE_OK);
	return db_handle;
}

inline auto_ptr<odb::database> open_sqlite_database(const std::string& name)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db (new odb::sqlite::database (make_name(name, db_inventory[0]), SQLITE_OPEN_READWRITE));	
	//odb::transaction t (db->begin());
	//shared_ptr<polaris::io::MetaData> mt = db->find<MetaData>("schema_version");
	//t.commit();
	//if (mt->value != SCHEMA_REVISION)
	//{
	//	std::cout << "Schema mismatch. Current schema is " << SCHEMA_REVISION << " and database schema is " << mt->value << "\n";
	//}
	odb::connection_ptr c (db->connection ());
	c->execute("PRAGMA synchronous = OFF");
	c->execute("PRAGMA journal_mode = MEMORY");
	ifstream test;
	if (db_inventory.size()>1)
	{
		for (vector<string>::iterator it = db_inventory.begin()+1; it != db_inventory.end(); ++it)
		{
			test.open(make_name(name, *it).c_str());
			if(test.is_open())
			{
				test.close();
				c->execute(make_attach_string(name,*it));
			}
			test.close();			
		}
	}
	return db;
}

inline auto_ptr<odb::database> open_sqlite_demand_database(const std::string& name)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db (new odb::sqlite::database (make_name(name, db_inventory[2]), SQLITE_OPEN_READWRITE));
	odb::connection_ptr c (db->connection ());
	c->execute("PRAGMA synchronous = OFF");
	c->execute("PRAGMA journal_mode = MEMORY");
	//ifstream test;
	//if (db_inventory.size()>1)
	//{
	//	for (vector<string>::iterator it = db_inventory.begin()+1; it != db_inventory.end(); ++it)
	//	{
	//		test.open(make_name(name, *it));

	//		if(test.is_open())
	//		{
	//			test.close();
	//			c->execute(make_attach_string(name,*it));
	//		}
	//		
	//	}
	//}
	return db;
}

inline auto_ptr<odb::database> create_sqlite_database(const string& name, string& schema)
{
	using namespace polaris::io;
	try
	{
		auto_ptr<odb::database> db (new odb::sqlite::database (make_name(name, schema), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
		// Create the database schema. Due to bugs in SQLite foreign key
		// support for DDL statements, we need to temporarily disable
		// foreign keys.
		odb::connection_ptr c (db->connection ());
		c->execute ("PRAGMA foreign_keys=OFF");
		odb::transaction t (c->begin ());
		try
		{
			odb::schema_catalog::create_schema (*db, schema);
		}
		catch (odb::unknown_schema e)
		{
			throw e;
		}
		t.commit ();
		c->execute ("PRAGMA foreign_keys=ON");
		return db;
	}
	catch (odb::sqlite::database_exception e)
	{
		cout << "Exception in create_sqlite_database while creating database: " << name << "\n Exception:" << e.what() << "\n";
		exit(0);
	}

}

inline auto_ptr<odb::database> create_sqlite_database(const string& name)
{
	using namespace polaris::io;
	auto_ptr<odb::database> db = create_sqlite_database(name, db_inventory[0]);
	shared_ptr<MetaData> mt (new MetaData("schema_version", SCHEMA_REVISION));
	odb::transaction t (db->begin());
	db->persist(mt);
	t.commit();
	odb::connection_ptr c (db->connection ());
	if (db_inventory.size()>1)
	{
		for (vector<string>::iterator it = db_inventory.begin()+1; it != db_inventory.end(); ++it)
		{
			try
			{
				auto_ptr<odb::database> temp = create_sqlite_database(name, *it);
			}
			catch (odb::unknown_schema e)
			{
				cout << e.what()  << "\n";
				continue;
			}
			c->execute(make_attach_string(name,*it));
		}
	}
	return db;
}




#endif // DATABASE_HXX
