#ifndef GEOMETRY
#define GEOMETRY
#include <vector>
#include <map>
#include "Supply.h"
#include "database.h"
#include "endian.h"

namespace polaris
{
namespace io
{	
	//see http://en.wikipedia.org/wiki/Well-known_text for specifications of different point types
	//another document to look at: http://edndoc.esri.com/arcsde/9.0/general_topics/wkb_representation.htm
	//MySQL page on WKB: http://dev.mysql.com/doc/refman/5.5/en//gis-wkb-format.html
	//ISO specification: http://jtc1sc32.org/doc/N1101-1150/32N1107-WD13249-3--spatial.pdf
	//another page from IBM: http://publib.boulder.ibm.com/infocenter/db2luw/v8/index.jsp?topic=/com.ibm.db2.udb.doc/opt/rsbp4121.htm
	struct GeoPoint {
		double x;
		double y;
	};
	struct GeoPointZ {
		double x;
		double y;
		double z;
	};
	struct GeoPointM {
		double x;
		double y;
		double m;
	};
	struct GeoPointZM {
		double x;
		double y;
		double z;
		double m;
	};
	template <typename PointType>
	struct Point {
		PointType coords;
		Point<PointType> *next; //for MultiPoint
	};
	template <typename PointType>
	struct LineString {
		std::vector<PointType> coords;
		LineString<PointType> *next;
	};
	//bulding block for polygon
	template <typename PointType>
	struct LineRing {
		bool clockwise; //true if the ring has clockwise direction and false otherwise
		std::vector<GeoPoint> coords;
		LineRing<PointType> *next; //used to implement MultyLineRing as a linked list
	};
	template <typename PointType>
	struct Polygon {
		LineRing<PointType> exterior;
		std::vector<LineRing<PointType>> interiors;
		Polygon<PointType> *next; //used to implement MultyPolygon as a linked list
	};
	template <typename PointType>
	struct GeometryCollection
	{
		bool init;
		int srid;
		char endian_arch; //architecture endian
		char endian; //blob endian
		char *blob; //WKB buffer
		unsigned long size;
		unsigned long offset;
		std::vector<Point<PointType>> points;
		std::vector<LineString<PointType>> linestrings;
		std::vector<LineRing<PointType>> LR;
	};

	//enum GeomType {Point, LineString, Polygon};
	//GeomType TypeFromWkb(char *wkb)
	//{}
	//template <typename GeometryType>
	//GeometryType GeomFromWkb(char *wkb)
	//{}
	typedef GeometryCollection<GeoPoint> GeometryCollectionPoint;
	typedef LineString<GeoPoint> LS;
	typedef Polygon<GeoPoint> POLY;
	typedef LineRing<GeoPoint> LR;
	bool InitGeometryCollection(GeometryCollectionPoint &geo, char *wkb)
	{
		geo.init = false;
		int size = sizeof(wkb);
		if (size < 5)
			return false;
		if (*(wkb + 0) == 0x01)
			geo.endian = 1;
		else if (*(wkb + 0) == 0x00)
			geo.endian = 0;
		else
			return false;
		return true;
	}
	template <typename PointType>
	LineString<PointType> LineStringFromWkb(char *wkb)
	{
		char endian_arch = 0; //architecture endian
		char endian; //blob endian
		if (*(wkb + 0) == 0x01)
			endian = 0;
		else if (*(wkb + 0) == 0x00)
			endian = 1;
		int points;
		int iv;
		double x;
		double y;
		PointType point;
		LineString<PointType> line;
		//int size = sizeof(wkb);
		int offset = sizeof(char) + sizeof(int);
		//if (size < offset + 4)
		//return line;
		points = import32(wkb + offset, endian, endian_arch);
		offset += 4;
		//if (size < offset + (16 * points))
		//return line;
		for (iv = 0; iv < points; iv++)
		{
			x = import64(wkb + offset, endian,	endian_arch);
			y = import64 (wkb + (offset + 8), endian, endian_arch);
			point.x = x;
			point.y = y;
			line.coords.push_back(point);
			offset += 16;
		}
		return line;
	}

	template <typename PointType>
	Polygon<PointType> PolygonFromWkb(char *wkb)
	{
		int offset = 0;
		char endian_arch = 0; //architecture endian
		char endian; //blob endian
		if (*(wkb + 0) == 0x01)
			endian = 0;
		else if (*(wkb + 0) == 0x00)
			endian = 1;
		offset += sizeof(char);
		int type = import32(wkb, endian, endian_arch);
		offset += sizeof(int);
		int rings;
		int nverts;
		int iv;
		int ib;
		double x;
		double y;
		Polygon<PointType> polyg;
		LineRing<PointType> ring;
		PointType point;
		rings = import32 (wkb + offset, endian, endian_arch);
		offset += 4;
		for (ib = 0; ib < rings; ib++)
			{

			nverts = import32 (wkb + offset, endian, endian_arch);
			offset += 4;
			ring.coords.clear();
			ring.next = NULL;
			for (iv = 0; iv < nverts; iv++)
			{
				x = import64 (wkb + offset, endian, endian_arch);
				y = import64 (wkb + (offset + 8), endian, endian_arch);
				offset += 16;
				point.x = x;
				point.y = y;
				ring.coords.push_back(point);
			}
			if (ib == 0)
			{
				polyg.exterior = ring;
			}
			else
				polyg.interiors.push_back(ring);
			}
		return polyg;
	}
	void ParseLineString(GeometryCollectionPoint &geo, char *wkb)
	{

		/* decodes a LINESTRING from WKB */
		if (!geo.init)
			if (!InitGeometryCollection(geo, wkb))
				return;
		int points;
		int iv;
		double x;
		double y;
		GeoPoint point;
		LineString<GeoPoint> line;
		geo.size = sizeof(wkb);
		geo.blob = wkb;
		if (geo.size < geo.offset + 4)
		return;
		points = import32(geo.blob + geo.offset, geo.endian, geo.endian_arch);
		geo.offset += 4;
		if (geo.size < geo.offset + (16 * points))
		return;
		for (iv = 0; iv < points; iv++)
		{
			x = import64(geo.blob + geo.offset, geo.endian,	geo.endian_arch);
			y = import64 (geo.blob + (geo.offset + 8), geo.endian, geo.endian_arch);
			point.x = x;
			point.y = y;
			line.coords.push_back(point);
			geo.offset += 16;
		}
		geo.linestrings.push_back(line);
	}

	
	static int point_callback(void *points_, int argc, char **argv, char **azColName)
	{
		std::map<int, shape_geometry> *points = (std::map<int, shape_geometry>*) points_;
		shape_geometry pt;
		char* geo_column;
		double x, xt;
		double y;		
		int link = atoi(argv[0]);
		if (argv[1] == NULL)
			return 0;
		geo_column = argv[1];
		short endian_type;
		int geo_type;
		int geo_typet;
		int offset = 0;
		endian_type = *(char*)geo_column; 
		offset += sizeof(char);
		geo_type = *(int*)(geo_column+offset);
		geo_typet = import32(geo_column+offset, 0,0);
		offset += sizeof(int);
		x = *(double*)(geo_column+offset);
		xt = import64(geo_column+offset, 0, 0);
		offset += sizeof(double);
		y = *(double*)(geo_column+offset);
		offset += sizeof(double);
		pt.x = x;
		pt.y = y;
		pt.z = 0.0;
		(*points)[link] = pt;
		return 0;

	}

	static int int_polygon_callback(void *polygons_, int argc, char **argv, char **azColName)
	{
		std::map<int, POLY>* polygons = (std::map<int, POLY> *) polygons_;
		POLY poly = PolygonFromWkb<GeoPoint>(argv[1]);
		int id = atoi(argv[0]);	
		(*polygons)[id] = poly;
		return 0;
	}


	static int polygon_callback(void *polygons_, int argc, char **argv, char **azColName)
	{
		std::map<std::string, POLY>* polygons = (std::map<std::string, POLY> *) polygons_;
		POLY poly = PolygonFromWkb<GeoPoint>(argv[1]);
		std::string name(argv[0]);	
		(*polygons)[name] = poly;
		return 0;
	}

	static int shapes_callback(void *shapes_, int argc, char **argv, char **azColName)
	{
		std::map<int, LS>* shapes = (std::map<int, LS> *) shapes_;
		LS points = LineStringFromWkb<GeoPoint>(argv[1]);
		int link = atoi(argv[0]);	
		(*shapes)[link] = points;
		return 0;
	}


	//static int shapes_callback(void *shapes_, int argc, char **argv, char **azColName)
	//{
	//	int i;
	//	std::map<int, std::vector<shape_geometry>>* shapes = (std::map<int, std::vector<shape_geometry>> *) shapes_;
	//	std::vector<shape_geometry> points;
	//	shape_geometry pt;
	//	char* geo_column;
	//	short endian_type;
	//	int geo_type;
	//	int n_points;
	//	double x;
	//	double y;
	//	int offset = 0;

	//	int link = atoi(argv[0]);

	//	if (argv[1] == NULL)
	//		return 0;
	//	geo_column = argv[1]; 
	//	endian_type = *(char*)geo_column; 
	//	offset += sizeof(char);
	//	geo_type = *(int*)(geo_column+offset);
	//	offset += sizeof(int);
	//	n_points = *(int*)(geo_column+offset);
	//	offset += sizeof(int);
	//	for (i = 1; i <= n_points; i++)
	//	{
	//		x = *(double*)(geo_column+offset);
	//		offset += sizeof(double);
	//		y = *(double*)(geo_column+offset);
	//		offset += sizeof(double);
	//		pt.x = x;
	//		pt.y = y;
	//		pt.z = 0.0;
	//		points.push_back(pt);
	//	}
	//	(*shapes)[link] = points;
	//	return 0;
	//}
	
	//rerurns map link_id --> link shape
	static std::map<int, LS> GetLinkShapes(const string& db_name)
	{
		
		std::map<int, LS> result;
		int ret;
		char *err_msg = NULL;
		char sql[2048];
		sqlite3* db_handle;
		db_handle = open_spatialite_database(db_name, false);
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
		db_handle = open_spatialite_database(db_name, false);
		strcpy(sql, "Select LINK, AsBinary(PointOnSurface(GEO)) from LINK WHERE GEO is not NULL");
		ret = sqlite3_exec(db_handle, sql, point_callback, &result, &err_msg);
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
		sprintf (sql, "SELECT Link.Link FROM Counties, Link where Counties.Name=\"%s\" and Within(Link.GEO, Counties.\"Geometry\")", county_name.c_str());
		ret = sqlite3_exec(db_handle, sql, link_callback, &result, &err_msg);
		return result;
	}

	static std::map<std::string, POLY> GetCountyPolygons(const std::string& db_name)
	{
		std::map<std::string, POLY> result;
		int ret;
		char *err_msg = NULL;
		char sql[2048];
		sqlite3* db_handle;
		db_handle = open_spatialite_database(db_name, false);
		strcpy (sql, "SELECT NAME, AsBinary(Geometry) FROM Counties");
		ret = sqlite3_exec(db_handle, sql, polygon_callback, &result, &err_msg);
		return result;
	}

	std::map<int, POLY> GetDepotPolygons(const std::string& db_name)
	{
		std::map<int, POLY> result;
		int ret;
		char *err_msg = NULL;
		char sql[2048];
		sqlite3* db_handle;
		db_handle = open_spatialite_database(db_name, false);
		strcpy (sql, "SELECT PK_UID, AsBinary(Geometry) FROM DepotPoly");
		ret = sqlite3_exec(db_handle, sql, int_polygon_callback, &result, &err_msg);
		return result;
	}

	
}
}
#endif