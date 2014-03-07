//*********************************************************
//	Geometry.h - Prototypes for Geometrical Shapes
//*********************************************************

#pragma once
#include "Dependencies.h"

namespace polaris
{
//---------------------------------------------------------
//	Point - 2D or 3D point
//---------------------------------------------------------

prototype struct Point
{
	tag_as_prototype;

	accessor(x, NONE, NONE);
	accessor(y, NONE, NONE);
	accessor(z, NONE, NONE);
};

//---------------------------------------------------------
//	Color_Prototype - rgba color
//---------------------------------------------------------

prototype struct Color
{
	tag_as_prototype;

	accessor(r, NONE, NONE);
	accessor(g, NONE, NONE);
	accessor(b, NONE, NONE);
	accessor(a, NONE, NONE);
};

//---------------------------------------------------------
//	Rectangle - Rectangle oriented in 2D or 3D Flexible API
//---------------------------------------------------------

prototype struct Rectangle
{
	tag_as_prototype;
	
	accessor(xmin, NONE, NONE);
	accessor(ymin, NONE, NONE);
	accessor(zmin, NONE, NONE);

	accessor(xmax, NONE, NONE);
	accessor(ymax, NONE, NONE);
	accessor(zmax, NONE, NONE);

	accessor(length, NONE, NONE);
	accessor(width, NONE, NONE);
	accessor(height, NONE, NONE);

	template<typename TargetType> void reset(){this_component()->reset<TargetType>();}
};
}
