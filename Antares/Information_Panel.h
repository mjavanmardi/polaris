//*********************************************************
//	Information_Panel.h - Information and Plotting
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Information_Panel - information panel class definition
//---------------------------------------------------------

prototype struct Information_Panel
{
	tag_as_prototype;

	feature_prototype void Plot()
	{
		this_component()->Plot();
	}

	feature_accessor(plotwindow,none,none);
};