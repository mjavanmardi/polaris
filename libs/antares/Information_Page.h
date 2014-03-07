//*********************************************************
//	Information_Page.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Dependencies.h"

//---------------------------------------------------------
//	Information_Page - information page class definition
//---------------------------------------------------------

prototype struct Information_Page
{
	tag_as_prototype;

	accessor(layer, NONE, NONE);

	template<typename TargetType> void Draw_Layer(int start_iteration, int end_iteration)
	{
		this_component()->Draw_Layer<TargetType>(start_iteration,end_iteration);
	}
};
