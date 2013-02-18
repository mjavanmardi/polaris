//*********************************************************
//	Control_Panel.h - Container Panel for Controls
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Layer_Options.h"

//---------------------------------------------------------
//	Control_Panel - control panel class definition
//---------------------------------------------------------

prototype struct Control_Panel
{
	tag_as_prototype;

	feature_accessor(layer_options,none,none);
	feature_accessor(canvas,none,none);
};