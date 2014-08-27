//*********************************************************
//	Conductor.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Dependencies.h"

//---------------------------------------------------------
//	Conductor - conductor class definition
//---------------------------------------------------------

prototype struct Conductor
{
	tag_as_prototype;

	template<typename TargetType> void Initialize()
	{
		this_component()->Initialize<TargetType>();
	}

	accessor(pause, NONE, NONE);
	accessor(canvas, NONE, NONE);
	accessor(information_panel, NONE, NONE);
	accessor(time_panel, NONE, NONE);
	accessor(next_iteration,NONE,NONE);
};
