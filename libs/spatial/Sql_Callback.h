#pragma once
#include "geometry.h"
namespace polaris {
namespace io {

static int int_point_callback(void *points_, int argc, char **argv, char **azColName)
{
	std::map<int, shape_geometry> *points = (std::map<int, shape_geometry>*) points_;
	shape_geometry pt;
	Point2D db_pt = PointFromWkb<GeoPoint>(argv[1]);
	int id = atoi(argv[0]);
	pt.x = db_pt.coords.x;
	pt.y = db_pt.coords.y;
	pt.z = 0.0;
	(*points)[id] = pt;
	return 0;
}

static int point_callback(void *points_, int argc, char **argv, char **azColName)
{
	assert(argc >= 2);
	std::map<std::string, shape_geometry> *points = (std::map<std::string, shape_geometry>*) points_;
	shape_geometry pt;
	Point2D db_pt = PointFromWkb<GeoPoint>(argv[1]);
	std::string id(argv[0]);	
	pt.x = db_pt.coords.x;
	pt.y = db_pt.coords.y;
	pt.z = 0.0;
	(*points)[id] = pt;
	return 0;
}

static int int_polygon_callback(void *polygons_, int argc, char **argv, char **azColName)
{
	std::map<int, Polygon2D>* polygons = (std::map<int, Polygon2D> *) polygons_;
	Polygon2D poly = PolygonFromWkb<GeoPoint>(argv[1]);
	int id = atoi(argv[0]);	
	(*polygons)[id] = poly;
	return 0;
}
static int polygon_callback(void *polygons_, int argc, char **argv, char **azColName)
{
	std::map<std::string, Polygon2D>* polygons = (std::map<std::string, Polygon2D> *) polygons_;
	Polygon2D poly = PolygonFromWkb<GeoPoint>(argv[1]);
	std::string name(argv[0]);	
	(*polygons)[name] = poly;
	return 0;
}
static int int_linestring_callback(void *shapes_, int argc, char **argv, char **azColName)
{
	std::map<int, LineString2D>* shapes = (std::map<int, LineString2D> *) shapes_;
	LineString2D points = LineStringFromWkb<GeoPoint>(argv[1]);
	int link = atoi(argv[0]);	
	(*shapes)[link] = points;
	return 0;
}
}}