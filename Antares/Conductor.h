//*********************************************************
//	Conductor.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Conductor - conductor class definition
//---------------------------------------------------------

prototype struct Conductor
{
	tag_as_prototype;

	feature_prototype void Initialize()
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>();
	}

	feature_accessor(pause,none,none);
	feature_accessor(canvas,none,none);
	feature_accessor(information_panel,none,none);
};