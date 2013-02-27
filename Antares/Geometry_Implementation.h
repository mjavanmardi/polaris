//*********************************************************
//	Geometry_Implementation.h - Implementations for Geometrical Shapes
//*********************************************************

#pragma once
#include "Geometry.h"

//---------------------------------------------------------
//	Point_2D - 2D point
//---------------------------------------------------------

implementation struct Point_2D : public Polaris_Component<APPEND_CHILD(Point_2D),MasterType,NULLTYPE>
{
	member_data(float,x,none,none);
	member_data(float,y,none,none);
};

//---------------------------------------------------------
//	Point_3D - 3D point
//---------------------------------------------------------

implementation struct Point_3D : public Polaris_Component<APPEND_CHILD(Point_3D),MasterType,NULLTYPE>
{
	Point_3D& operator= (Point_3D& setval)
	{
		_x=setval._x;
		_y=setval._y;
		_z=setval._z;
		return *this;
	};

	member_data(float,x,none,none);
	member_data(float,y,none,none);
	member_data(float,z,none,none);
};

//---------------------------------------------------------
//	Rectangle_XY - Rectangle Oriented in XY
//---------------------------------------------------------

implementation struct Rectangle_XY : public Polaris_Component<APPEND_CHILD(Rectangle_XY),MasterType,NULLTYPE>
{
	member_data(float,xmin,none,none);
	member_data(float,ymin,none,none);

	member_data(float,xmax,none,none);
	member_data(float,ymax,none,none);

	feature_implementation void reset()
	{
		_xmin=FLT_MAX;
		_xmax=-FLT_MAX;
		_ymin=FLT_MAX;
		_ymax=-FLT_MAX;
	}

	feature_implementation TargetType length()
	{
		return (TargetType)( _ymax-_ymin );
	}
	
	feature_implementation TargetType width()
	{
		return (TargetType)( _xmax-_xmin );
	}

	feature_implementation TargetType height()
	{
		return 0;
	}
};

//---------------------------------------------------------
//	True_Color_RGB - rgba color implementation
//---------------------------------------------------------

implementation struct True_Color_RGB : public Polaris_Component<APPEND_CHILD(True_Color_RGB),MasterType,NULLTYPE>
{
	member_data(unsigned char,r,none,none);
	member_data(unsigned char,g,none,none);
	member_data(unsigned char,b,none,none);
};

//---------------------------------------------------------
//	True_Color_RGBA - rgba color implementation
//---------------------------------------------------------

implementation struct True_Color_RGBA : public Polaris_Component<APPEND_CHILD(True_Color_RGBA),MasterType,NULLTYPE>
{
	True_Color_RGBA():_r(0),_g(0),_b(0),_a(255){}
	True_Color_RGBA(unsigned char rval, unsigned char gval, unsigned char bval, unsigned char aval):_r(rval),_g(gval),_b(bval),_a(aval){}
	
	True_Color_RGBA& operator= (const True_Color_RGBA& setval)
	{
		_r=setval._r;
		_g=setval._g;
		_b=setval._b;
		_a=setval._a;
		return *this;
	};

	member_data(unsigned char,r,none,none);
	member_data(unsigned char,g,none,none);
	member_data(unsigned char,b,none,none);
	member_data(unsigned char,a,none,none);
};

//---------------------------------------------------------
//	Test point in polygon, Antares version
//---------------------------------------------------------

static inline int X_Sort_Function (const void *rec1, const void *rec2)
{
	float r1, r2;

	r1 = *((float *) rec1);
	r2 = *((float *) rec2);

	if (r1 > r2) return (1);
	if (r1 < r2) return (-1);
	return (0);
}

static inline bool In_Polygon(vector<Point_3D<NULLTYPE>>& points, float x, float y)
{
	int num;
	double x1, y1, x2, y2, xlist [100];
	bool first, close_flag;

	vector<Point_3D<NULLTYPE>>::iterator point_itr, end_itr;
	
	if (points.size () < 3) return (false);

	num = 0;
	first = true;
	close_flag = false;
	end_itr = --points.end ();
	x1 = y1 = 0.0;

	for (point_itr = points.begin (); num < 100; point_itr++) {
		if (point_itr < points.end ()) {
			x2 = point_itr->_x;
			y2 = point_itr->_y;
		} else {
			if (!close_flag) break;
			x2 = end_itr->_x;
			y2 = end_itr->_y;
			close_flag = false;
		}
		if (first) {
			first = false;
			close_flag = !(end_itr->_x == x2 && end_itr->_y == y2);
			end_itr = point_itr;
		} else {
			if (y1 == y2) {
				if (y == y1) {
					if (x1 < x2) {
						if (x1 <= x && x <= x2) return (true);
					} else {
						if (x2 <= x && x <= x1) return (true);
					}
				}
			} else if (y1 < y2) {
				if (y1 <= y && y < y2) {
					xlist [num++] = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
				}
			} else {
				if (y2 < y && y <= y1) {
					xlist [num++] = x2 + (y - y2) * (x1 - x2) / (y1 - y2);
				}
			}
		}
		x1 = x2;
		y1 = y2;
	}
	if (num < 2) return (false);

	if (num == 2) {
		if (xlist [0] < xlist [1]) {
			if (xlist [0] <= x && x <= xlist [1]) return (true);
		} else {
			if (xlist [1] <= x && x <= xlist [0]) return (true);
		}
	} else {
		qsort (xlist, num, sizeof (double), X_Sort_Function);

		for (int i=0; i < num; i+=2) {
			if (xlist [i] <= x && x <= xlist [i+1]) return (true);
		}
	}
	return (false);
}