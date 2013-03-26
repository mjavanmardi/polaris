#ifndef GEOMETRY
#define GEOMETRY
#include <vector>
#include <map>
#include "Supply.h"
#include "database.h"

namespace polaris
{
namespace io
{
	struct LineStringProperties
	{
		short endian_type;
		int geo_type;
		int n_points;
	};


	static int point_callback(void *points_, int argc, char **argv, char **azColName)
	{
		std::map<int, shape_geometry> *points = (std::map<int, shape_geometry>*) points_;
		shape_geometry pt;
		char* geo_column;
		double x;
		double y;		
		int link = atoi(argv[0]);
		if (argv[1] == NULL)
			return 0;
		geo_column = argv[1];
		short endian_type;
		int geo_type;
		int offset = 0;
		endian_type = *(char*)geo_column; 
		offset += sizeof(char);
		geo_type = *(int*)(geo_column+offset);
		offset += sizeof(int);
		x = *(double*)(geo_column+offset);
		offset += sizeof(double);
		y = *(double*)(geo_column+offset);
		offset += sizeof(double);
		pt.x = x;
		pt.y = y;
		pt.z = 0.0;
		(*points)[link] = pt;
		return 0;

	}

	static int shapes_callback(void *shapes_, int argc, char **argv, char **azColName)
	{
		int i;
		std::map<int, std::vector<shape_geometry>>* shapes = (std::map<int, std::vector<shape_geometry>> *) shapes_;
		std::vector<shape_geometry> points;
		shape_geometry pt;
		char* geo_column;
		short endian_type;
		int geo_type;
		int n_points;
		double x;
		double y;
		LineStringProperties lsp;
		int offset = 0;

		int link = atoi(argv[0]);

		if (argv[1] == NULL)
			return 0;
		geo_column = argv[1];
		lsp = *(LineStringProperties*)geo_column; 
		endian_type = *(char*)geo_column; 
		offset += sizeof(char);
		geo_type = *(int*)(geo_column+offset);
		offset += sizeof(int);
		n_points = *(int*)(geo_column+offset);
		offset += sizeof(int);
		for (i = 1; i <= n_points; i++)
		{
			x = *(double*)(geo_column+offset);
			offset += sizeof(double);
			y = *(double*)(geo_column+offset);
			offset += sizeof(double);
			pt.x = x;
			pt.y = y;
			pt.z = 0.0;
			points.push_back(pt);
		}
		(*shapes)[link] = points;
		return 0;
	}
	static std::map<int, std::vector<shape_geometry>> GetLinkShapes(const string& db_name)
	{
		std::map<int, std::vector<shape_geometry>> result;
		std::vector<shape_geometry> shape_points;
		int ret;
		char *err_msg = NULL;
		char sql[2048];
		sqlite3* db_handle;
		db_handle = open_spatialite_database(db_name);
		strcpy(sql, "Select LINK, AsBinary(GEO) from LINK WHERE GEO is not NULL");
		ret = sqlite3_exec(db_handle, sql, shapes_callback, &result, &err_msg);
		return result;
	}

	static std::map<int, shape_geometry> GetLinkPoints(const string& db_name)
	{
		std::map<int, shape_geometry> result;
		int ret;
		char *err_msg = NULL;
		char sql[2048];
		sqlite3* db_handle;
		db_handle = open_spatialite_database(db_name);
		strcpy(sql, "Select LINK, AsBinary(PointOnSurface(GEO)) from LINK WHERE GEO is not NULL");
		ret = sqlite3_exec(db_handle, sql, point_callback, &result, &err_msg);
		return result;
	}
}
}
#endif