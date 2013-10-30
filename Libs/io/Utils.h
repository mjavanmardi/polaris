#pragma once
#include <string>
#include "Io/database.h"

namespace polaris 
{
static int int_callback(void *number_, int argc, char** argv, char** azColName)
{
	*((int*)number_) = atoi(argv[0]);
	return 0;
}

int CountRows(const std::string& db_path, const std::string& table_name)
{
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_path, false);
	int result;
	char *err_msg = NULL;
	int ret;
	char sql[2048];
	sprintf (sql, "select count(*) from %s", table_name.c_str());
	ret = sqlite3_exec(db_handle, sql, int_callback, &result, &err_msg);
	return result;
}

}