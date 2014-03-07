//*********************************************************
//	Control_Panel.h - Container Panel for Controls
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Layer_Options.h"

//---------------------------------------------------------
//	Control_Panel - control panel class definition
//---------------------------------------------------------

prototype struct Control_Panel
{
	tag_as_prototype;

	accessor(layer_options, NONE, NONE);
	accessor(canvas, NONE, NONE);
};
