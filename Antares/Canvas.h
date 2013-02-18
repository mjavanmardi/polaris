//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Antares_Layer.h"

//---------------------------------------------------------
//	Canvas - canvas class definition
//---------------------------------------------------------

prototype struct Canvas
{
	tag_as_prototype;

	feature_prototype void Initialize()
	{
		this_component()->Initialize<CallerType,TargetType>();
	}

	feature_prototype void Refresh()
	{
		this_component()->Refresh();
	}
	
	feature_prototype typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<CallerType,TargetType>(name);
	}
	
	feature_prototype typename TargetType::ReturnType Select_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Select_Layer<CallerType,TargetType>(name);
	}
	
	feature_prototype typename TargetType::ReturnType Toggle_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Toggle_Layer<CallerType,TargetType>(name);
	}

	feature_accessor(time_panel,none,none);
	feature_accessor(information_panel,none,none);
	feature_accessor(control_panel,none,none);
	feature_accessor(layer_options,none,none);

	feature_accessor(graphical_network,none,none);
};