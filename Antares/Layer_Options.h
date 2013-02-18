//*********************************************************
//	Layer_Options.h - Graphical Layer Options
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Layer_Options - layer options definition
//---------------------------------------------------------

prototype struct Layer_Options
{
	tag_as_prototype;

	feature_accessor(canvas,none,none);

	feature_prototype typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<CallerType,TargetType>(name);
	}
};