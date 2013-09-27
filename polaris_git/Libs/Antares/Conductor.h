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

	void Initialize()
	{
		this_component()->Initialize();
	}

	accessor(pause,NONE,NONE);
	accessor(canvas,NONE,NONE);
	accessor(information_panel,NONE,NONE);
};