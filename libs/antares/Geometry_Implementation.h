//*********************************************************
//	Geometry_Implementation.h - Implementations for Geometrical Shapes
//*********************************************************

#pragma once
#include "Geometry.h"

//---------------------------------------------------------
//	Point_2D - 2D point
//---------------------------------------------------------

implementation struct Point_2D : public Polaris_Component<MasterType,INHERIT(Point_2D),NULLTYPE>
{
	m_data(float,x, NONE, NONE);
	m_data(float,y, NONE, NONE);
};

//---------------------------------------------------------
//	Point_3D - 3D point
//---------------------------------------------------------

implementation struct Point_3D : public Polaris_Component<MasterType,INHERIT(Point_3D),NULLTYPE>
{
	Point_3D& operator= (Point_3D& setval)
	{
		_x=setval._x;
		_y=setval._y;
		_z=setval._z;
		return *this;
	};

	m_data(float,x, NONE, NONE);
	m_data(float,y, NONE, NONE);
	m_data(float,z, NONE, NONE);
};

//---------------------------------------------------------
//	Rectangle_XY - Rectangle Oriented in XY
//---------------------------------------------------------

implementation struct Rectangle_XY : public Polaris_Component<MasterType,INHERIT(Rectangle_XY),NULLTYPE>
{
	m_data(float,xmin, NONE, NONE);
	m_data(float,ymin, NONE, NONE);

	m_data(float,xmax, NONE, NONE);
	m_data(float,ymax, NONE, NONE);

	template<typename TargetType> void reset()
	{
		_xmin=FLT_MAX;
		_xmax=-FLT_MAX;
		_ymin=FLT_MAX;
		_ymax=-FLT_MAX;
	}

	template<typename TargetType> TargetType length()
	{
		return (TargetType)( _ymax-_ymin );
	}
	
	template<typename TargetType> TargetType width()
	{
		return (TargetType)( _xmax-_xmin );
	}

	template<typename TargetType> TargetType height()
	{
		return 0;
	}
};

//---------------------------------------------------------
//	True_Color_RGB - rgba color implementation
//---------------------------------------------------------

implementation struct True_Color_RGB : public Polaris_Component<MasterType,INHERIT(True_Color_RGB),NULLTYPE>
{
	m_data(unsigned char,r, NONE, NONE);
	m_data(unsigned char,g, NONE, NONE);
	m_data(unsigned char,b, NONE, NONE);
};

//---------------------------------------------------------
//	True_Color_RGBA - rgba color implementation
//---------------------------------------------------------

implementation struct True_Color_RGBA : public Polaris_Component<MasterType,INHERIT(True_Color_RGBA),NULLTYPE>
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

	m_data(unsigned char,r, NONE, NONE);
	m_data(unsigned char,g, NONE, NONE);
	m_data(unsigned char,b, NONE, NONE);
	m_data(unsigned char,a, NONE, NONE);
};

//---------------------------------------------------------
//	Determine distance between point C and line segment AB
//---------------------------------------------------------

static inline void Point_Segment_Distance(Point_3D<NULLTYPE>& A, Point_3D<NULLTYPE>& B, Point_3D<NULLTYPE>& C, float& dist)
{
	//---- u is used to find the projection onto AB ---

	float u=((C._x-A._x)*(B._x-A._x)+(C._y-A._y)*(B._y-A._y))/((A._x-B._x)*(A._x-B._x)+(A._y-B._y)*(A._y-B._y));
	
	if(u<0 || u>1)
	{
		//---- if u is not between 0 and 1, test distance to endpoints ---

		float tmp_dist;

		//---- to A ---

		dist=sqrt((A._x-C._x)*(A._x-C._x)+(A._y-C._y)*(A._y-C._y));
		
		//---- to B ---

		tmp_dist=sqrt((B._x-C._x)*(B._x-C._x)+(B._y-C._y)*(B._y-C._y));

		if(tmp_dist<dist) dist=tmp_dist;
	}
	else
	{
		//---- if u is between 0 and 1, compute distance to projection ---

		float x=A._x+u*(B._x-A._x);
		float y=A._y+u*(B._y-A._y);

		dist=sqrt((x-C._x)*(x-C._x)+(y-C._y)*(y-C._y));
	}
}

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

static inline bool In_Polygon(std::vector<Point_3D<NULLTYPE>>& points, float x, float y)
{
	int num;
	double x1, y1, x2, y2, xarray [100];
	bool first, close_flag;

	std::vector<Point_3D<NULLTYPE>>::iterator point_itr, end_itr;
	
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
					xarray [num++] = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
				}
			} else {
				if (y2 < y && y <= y1) {
					xarray [num++] = x2 + (y - y2) * (x1 - x2) / (y1 - y2);
				}
			}
		}
		x1 = x2;
		y1 = y2;
	}
	if (num < 2) return (false);

	if (num == 2) {
		if (xarray [0] < xarray [1]) {
			if (xarray [0] <= x && x <= xarray [1]) return (true);
		} else {
			if (xarray [1] <= x && x <= xarray [0]) return (true);
		}
	} else {
		qsort (xarray, num, sizeof (double), X_Sort_Function);

		for (int i=0; i < num; i+=2) {
			if (xarray [i] <= x && x <= xarray [i+1]) return (true);
		}
	}
	return (false);
}

//---------------------------------------------------------
//	Generic 3 point normal calculation with winding abc
//---------------------------------------------------------

static inline void Compute_Fast_Normal(Point_3D<NT>& a, Point_3D<NT>& b, Point_3D<NT>& c, Point_3D<NT>& result)
{
	result._x=(b._y-a._y)*(c._z-a._z)-(b._z-a._z)*(c._y-a._y);
	result._y=(b._z-a._z)*(c._x-a._x)-(b._x-a._x)*(c._z-a._z);
	result._z=(b._x-a._x)*(c._y-a._y)-(b._y-a._y)*(c._x-a._x);

	float unit_length=sqrt(result._x*result._x+result._y*result._y+result._z*result._z);
	
	result._x/=unit_length;
	result._y/=unit_length;
	result._z/=unit_length;
}
