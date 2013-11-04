#pragma once
#include <vector>
namespace polaris {
namespace io {
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
//building block for polygon
template <typename PointType>
struct LineRing {
	bool clockwise; //true if the ring has clockwise direction and false otherwise
	std::vector<GeoPoint> coords;
	LineRing<PointType> *next; //used to implement MultyLineRing as a linked list
};
template <typename PointType>
struct Polygon {
	LineRing<PointType> exterior;
	std::vector<LineRing<PointType> > interiors;
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
	std::vector<Point<PointType> > points;
	std::vector<LineString<PointType> > linestrings;
	std::vector<LineRing<PointType> > LR;
};
//traits
template <typename PointType>
struct is_3d {
	static const bool value = false;
};
template <typename PointType>
struct is_4d {
	static const bool value = false;
};
template <>
struct is_3d<GeoPointZ> {
	static const bool value = true;
};
template <>
struct is_3d<GeoPointM> {
	static const bool value = true;
};
template <>
struct is_4d<GeoPointZM> {
	static const bool value = true;
};

//enum GeomType {Point, LineString, Polygon};
//GeomType TypeFromWkb(char *wkb)
//{}
//template <typename GeometryType>
//GeometryType GeomFromWkb(char *wkb)
//{}
typedef GeometryCollection<GeoPoint> GeometryCollectionPoint;
typedef LineString<GeoPoint> LineString2D;
typedef Polygon<GeoPoint> Polygon2D;
typedef LineRing<GeoPoint> LineRing2D;
typedef Point<GeoPoint> Point2D;
}}