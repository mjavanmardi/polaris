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

	accessor(canvas, NONE, NONE);

	template<typename TargetType> typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<TargetType>(name);
	}

	template<typename TargetType> void Toggle_Named_Layer(string& name,bool check_state)
	{
		this_component()->Toggle_Named_Layer<TargetType>(name,check_state);
	}
};
