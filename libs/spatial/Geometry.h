#pragma once
#include <vector>
#include <map>
#include "Io/Io_db.h"
#include "endian.h"
#include "data_structures.h"

namespace polaris
{
namespace io
{	

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
inline void ExtractPoint(GeoPoint& point, char *wkb, int &offset, char& endian, char& endian_arch)
{
	point.x = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.y = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
}

inline void ExtractPoint(GeoPointZ& point, char *wkb, int &offset, char& endian, char& endian_arch)
{
	point.x = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.y = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.z = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
}
inline void ExtractPoint(GeoPointM& point, char *wkb, int &offset, char& endian, char& endian_arch)
{
	point.x = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.y = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.m = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
}

inline void ExtractPoint(GeoPointZM& point, char *wkb, int &offset, char& endian, char& endian_arch)
{
	point.x = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.y = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.z = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
	point.m = import64(wkb+offset, endian, endian_arch);
	offset += sizeof(double);
}

template <typename PointType>
Point<PointType> PointFromWkb(char *wkb)
{
	char endian_arch = 0; //architecture endian
	char endian; //blob endian
	if (*(wkb + 0) == 0x01)
		endian = 0;
	else if (*(wkb + 0) == 0x00)
		endian = 1;
	PointType point;
	Point<PointType> pt;
	int offset = sizeof(char);
	int geo_type = import32(wkb+offset, endian, endian_arch);
	offset += sizeof(int);
	ExtractPoint(point, wkb, offset, endian, endian_arch);
	pt.coords = point;
	return pt;
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
		ExtractPoint(point, wkb, offset, endian, endian_arch);
		line.coords.push_back(point);
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
			ExtractPoint(point, wkb, offset, endian, endian_arch);
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
// 
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
}}
