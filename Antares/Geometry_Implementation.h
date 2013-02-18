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

	True_Color_RGBA& operator= (True_Color_RGBA& setval)
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