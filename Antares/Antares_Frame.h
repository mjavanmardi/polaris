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

	feature_accessor(control_panel,none,none);
	feature_accessor(time_panel,none,none);
	feature_accessor(information_panel,none,none);
	feature_accessor(canvas,none,none);
	feature_accessor(aui_manager,none,none);
	feature_accessor(conductor,none,none);
	feature_accessor(db_name,none,none);
	feature_accessor(simulation,none,none);
};