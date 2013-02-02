//*********************************************************
//	Geometry_Implementation.h - Implementations for Geometrical Shapes
//*********************************************************

#pragma once
#include "Geometry.h"

//---------------------------------------------------------
//	Point_2D - 2D point
//---------------------------------------------------------

implementation struct Point_2D : public Polaris_Component_Class<Point_2D,MasterType,NULLTYPE>
{
	member_data(float,x,none,none);
	member_data(float,y,none,none);
};

//---------------------------------------------------------
//	Point_3D - 3D point
//---------------------------------------------------------

implementation struct Point_3D : public Polaris_Component_Class<Point_3D,MasterType,NULLTYPE>
{
	member_data(float,x,none,none);
	member_data(float,y,none,none);
	member_data(float,z,none,none);
};

//---------------------------------------------------------
//	Rectangle_XY - Rectangle Oriented in XY
//---------------------------------------------------------

implementation struct Rectangle_XY : public Polaris_Component_Class<Rectangle_XY,MasterType,NULLTYPE>
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