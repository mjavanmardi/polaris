//*********************************************************
//	Information_Page.h - Plotting Canvas
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Information_Page - information page class definition
//---------------------------------------------------------

prototype struct Information_Page
{
	tag_as_prototype;

	feature_accessor(layer,none,none);

	feature_prototype void Draw_Layer(int start_iteration, int end_iteration)
	{
		this_component()->Draw_Layer<ComponentType,CallerType,TargetType>(start_iteration,end_iteration);
	}
};