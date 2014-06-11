#pragma once
#include "data_structures.h"
#include "sql_callback.h"

namespace polaris {
namespace io {
//returns map link_id --> link shape
static std::map<int, LineString2D> GetLinkShapes(const string& db_name)
{
	
	std::map<int, LineString2D> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy(sql, "Select LINK, AsBinary(GEO) from LINK WHERE GEO is not NULL");
	ret = sqlite3_exec(db_handle, sql, int_linestring_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}

static std::map<int, shape_geometry> GetLinkPoints(const string& db_name)
{
	std::map<int, shape_geometry> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy(sql, "Select LINK, AsBinary(PointOnSurface(GEO)) from LINK WHERE GEO is not NULL");
	ret = sqlite3_exec(db_handle, sql, int_point_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}

static std::map<std::string, shape_geometry> GetPointGeometry(const string& db_prefix, const string& table_name, const string& column_name, const string& pk_name, int epsg, const string& db_suffix="")
{
	std::map<std::string, shape_geometry> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	string db_name;
	db_name.append(db_prefix);
	db_name.append(db_suffix);
	//db_handle = open_spatialite_database(db_prefix, false, db_suffix);
	db_handle = open_spatialite_database(db_prefix, false);
	sprintf (sql, "Select %s, Transform(\"%s\", %i) from %s where %s is not null", pk_name, column_name, epsg, table_name, column_name);
	
	ret = sqlite3_exec(db_handle, sql, point_callback, &result, &err_msg);
	std::cout << sql;
	assert(ret == SQLITE_OK);
	ret = sqlite3_close(db_handle);
	return result;
}

static std::map<int, shape_geometry> GetCountyPoint(const string& db_name, const string& county_name)
{
	std::map<int, shape_geometry> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	sprintf (sql, "Select PK_UID, AsBinary(PointOnSurface(Geometry)) from Counties WHERE Name=\"%s\"", county_name.c_str());
	ret = sqlite3_exec(db_handle, sql, int_point_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}


static std::map<int, shape_geometry> GetLinkPoint(const string& db_name, const int& link_id)
{
	std::map<int, shape_geometry> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	sprintf (sql, "Select Link, AsBinary(PointOnSurface(GEO)) from Link WHERE link=\"%d\"",link_id);
	ret = sqlite3_exec(db_handle, sql, int_point_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}

static int link_callback(void *links_, int argc, char **argv, char **azColName)
{
	std::vector<int> *links = (std::vector<int>*) links_;
	int link = atoi(argv[0]);
	links->push_back(link);
	return 0;
}
static std::vector<int> GetLinksInsideDepotPolygon(const std::string& db_name)
{
	std::vector<int> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy(sql, "select Link.Link from Link, DepotPoly where Within(Link.GEO, DepotPoly.Geometry) and Link.TYPE in (\"FREEWAY\", \"EXPRESSWAY\")");
	ret = sqlite3_exec(db_handle, sql, link_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}

static std::vector<int> GetLinksInsideWeatherPolygon(const std::string& db_name)
{
	std::vector<int> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy(sql, "select Link.Link from Link, WeatherPoly where Within(Link.GEO, WeatherPoly.Geometry)");
	ret = sqlite3_exec(db_handle, sql, link_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}
static std::vector<int> GetLinksInsideCounty(const std::string& db_name, const std::string& county_name)
{
	std::vector<int> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	sprintf (sql, "SELECT Link.Link FROM Counties, Link where Counties.Name=\"%s\" and MbrWithin(Link.GEO, Counties.\"Geometry\")", county_name.c_str());
	ret = sqlite3_exec(db_handle, sql, link_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}
static std::vector<int> GetLinksInsideZip(const std::string& db_name, const std::string& zip)
{
	std::vector<int> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	sprintf (sql, "SELECT Link.Link FROM Zip, Link where Zip.ZCTA5CE=\"%s\" and MbrWithin(Link.GEO, Zip.Geometry)", zip.c_str());
	ret = sqlite3_exec(db_handle, sql, link_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}


static std::map<std::string, Polygon2D> GetCountyPolygons(const std::string& db_name)
{
	std::map<std::string, Polygon2D> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy (sql, "SELECT NAME, AsBinary(Geometry) FROM Counties");
	ret = sqlite3_exec(db_handle, sql, polygon_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}

std::map<int, Polygon2D> GetDepotPolygons(const std::string& db_name)
{
	std::map<int, Polygon2D> result;
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	sqlite3* db_handle;
	db_handle = open_spatialite_database(db_name, false);
	strcpy (sql, "SELECT PK_UID, AsBinary(Geometry) FROM DepotPoly");
	ret = sqlite3_exec(db_handle, sql, int_polygon_callback, &result, &err_msg);
	ret = sqlite3_close(db_handle);
	return result;
}	

void GetMicrosoftEventPoints(sqlite3* db_handle, std::vector<std::vector<double> >&  out_points)
{
	std::map<int, shape_geometry> result;
	std::vector<double> pt(2, 0);
	int ret;
	char *err_msg = NULL;
	char sql[2048];
	strcpy (sql, "SELECT incidentId, AsBinary(geo) FROM Microsoft_Event");
	ret = sqlite3_exec(db_handle, sql, int_point_callback, &result, &err_msg);
	for (auto it = result.begin(); it!=result.end(); ++it)
	{
		pt[0] = it->second.x;
		pt[1] = it->second.y;
		//std::cout << pt[0] << ", " << pt[1] << "\n";
		out_points.push_back(pt);
	}
}
}}