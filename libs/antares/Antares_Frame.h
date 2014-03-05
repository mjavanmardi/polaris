//*********************************************************
//	Antares_Frame.h - Parent Frame
//*********************************************************

#pragma once
#include "Conductor.h"
#include "Control_Panel.h"
#include "Time_Panel.h"
#include "Information_Panel.h"
#include "Attributes_Panel.h"
#include "Canvas.h"

//---------------------------------------------------------
//	Antares - aui initialization
//---------------------------------------------------------

prototype struct Antares
{
	tag_as_prototype;

	accessor(control_panel, NONE, NONE);
	accessor(time_panel, NONE, NONE);
	accessor(information_panel, NONE, NONE);
	accessor(canvas, NONE, NONE);
	accessor(aui_manager, NONE, NONE);
	accessor(conductor, NONE, NONE);
};
