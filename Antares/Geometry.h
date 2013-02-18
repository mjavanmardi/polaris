//*********************************************************
//	Geometry.h - Prototypes for Geometrical Shapes
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Point - 2D or 3D point
//---------------------------------------------------------

prototype struct Point_Prototype
{
	tag_as_prototype;

	feature_accessor(x,none,none);
	feature_accessor(y,none,none);
	feature_accessor(z,none,none);
};

//---------------------------------------------------------
//	Color_Prototype - rgba color
//---------------------------------------------------------

prototype struct Color_Prototype
{
	tag_as_prototype;

	feature_accessor(r,none,none);
	feature_accessor(g,none,none);
	feature_accessor(b,none,none);
	feature_accessor(a,none,none);
};

//---------------------------------------------------------
//	Rectangle - Rectangle oriented in 2D or 3D Flexible API
//---------------------------------------------------------

prototype struct Rectangle_Prototype
{
	tag_as_prototype;
	
	feature_accessor(xmin,none,none);
	feature_accessor(ymin,none,none);
	feature_accessor(zmin,none,none);

	feature_accessor(xmax,none,none);
	feature_accessor(ymax,none,none);
	feature_accessor(zmax,none,none);

	feature_accessor(length,none,none);
	feature_accessor(width,none,none);
	feature_accessor(height,none,none);

	feature_prototype void reset(){this_component()->reset<CallerType,TargetType>();}
};