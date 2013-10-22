#pragma once
#include "Geometry_Implementation.h"

namespace polaris
{
//---------------------------------------------------
// GEOMETRIC ELEMENTS FOR DRAWING
#pragma region 
	#pragma pack(push,1)
struct Quad_Element
{
	Point_3D<NT> _v1;
	Point_3D<NT> _v2;
	Point_3D<NT> _v3;
	Point_3D<NT> _v4;
};
#pragma pack(pop)
#pragma pack(push,1)
struct Quad_Element_Colored
{
	True_Color_RGBA<NT> _color;
	Point_3D<NT> _v1;
	Point_3D<NT> _v2;
	Point_3D<NT> _v3;
	Point_3D<NT> _v4;
};
#pragma pack(pop)


#pragma pack(push,1)
struct Line_Element
{
	Point_3D<NT> _v1;
	Point_3D<NT> _v2;
};
#pragma pack(pop)
#pragma pack(push,1)
struct Line_Element_Colored
{
	True_Color_RGBA<NT> _color;
	Point_3D<NT> _v1;
	Point_3D<NT> _v2;
};
#pragma pack(pop)


#pragma pack(push,1)
struct Point_Element
{
	Point_3D<NT> _point;
};
#pragma pack(pop)
#pragma pack(push,1)
struct Point_Element_Colored
{
	True_Color_RGBA<NT> _color;
	Point_3D<NT> _point;
};
#pragma pack(pop)
#pragma endregion
//-------------

}