#pragma once
namespace polaris {
namespace io {
/// usage: AddGeometryCoumn(db_handle, 4326, event_cdenters_temp, "LINESTRING", "XY")
int AddGeometryCoumn(sqlite3* db_handle, int srid, std::string table, std::string type, std::string dim)
{
	char sql[2048];
	int ret;
	char buff[1024];
	char *err_msg = NULL;
	char** pazResult = NULL;
	int pnRow, pnColumn, pnRowt;
	//check if geo column exists
	sprintf (buff, "SELECT * FROM geometry_columns	WHERE Lower(f_table_name) = Lower('%s') AND Lower(f_geometry_column) = Lower('geo')", table.c_str());
	strcpy (sql, buff);
	ret = sqlite3_get_table(db_handle, sql, &pazResult, &pnRow, &pnColumn, &err_msg); sqlite3_free_table(pazResult); //deallocate the resulting table 
	if (ret != SQLITE_OK) hadnle_sqlite_error(&err_msg, ret);

	sprintf (buff, "SELECT 'geo' FROM '%s'", table.c_str());
	strcpy (sql, buff);
	ret = sqlite3_get_table(db_handle, sql, &pazResult, &pnRowt, &pnColumn, &err_msg); sqlite3_free_table(pazResult); //deallocate the resulting table
	if (ret != SQLITE_OK) hadnle_sqlite_error(&err_msg, ret);
	if (pnRow > 0) 
	{
		sprintf (buff, "SELECT DiscardGeometryColumn('%s','GEO')", table.c_str());
		strcpy (sql, buff);
		ret = sqlite3_exec (db_handle, sql, NULL, NULL, &err_msg); if (ret != SQLITE_OK) hadnle_sqlite_error(&err_msg, ret);

	}
	if (pnColumn > 0)
	{
		sprintf (buff, "update '%s' set 'GEO'=NULL", table.c_str());
		strcpy (sql, buff);
		ret = sqlite3_exec (db_handle, sql, NULL, NULL, &err_msg); if (ret != SQLITE_OK) hadnle_sqlite_error(&err_msg, ret);
	}
	sprintf (buff, "SELECT AddGeometryColumn('%s', 'GEO', %d, '%s', '%s')", table.c_str(), srid, type.c_str(), dim.c_str());
	strcpy (sql, buff);
	ret = sqlite3_exec (db_handle, sql, NULL, NULL, &err_msg); if (ret != SQLITE_OK) hadnle_sqlite_error(&err_msg, ret);

	return ret;
}

void AddPointText(char *buff, const double& x, const double y, const int& srid)
{
	sprintf (buff, "GeomFromText('POINT (%1.6f %1.6f)', %d)",x, y, srid);
	return;
}


}}