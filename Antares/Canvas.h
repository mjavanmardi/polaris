//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Antares_Layer.h"

//---------------------------------------------------------
//	Canvas - canvas class definition
//---------------------------------------------------------

enum ANTARES_MODE {NO_MODE,NAVIGATE,IDENTIFY};

prototype struct Canvas
{
	tag_as_prototype;

	feature_prototype void Initialize()
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>();
	}

	feature_prototype void Refresh()
	{
		this_component()->Refresh();
	}
	
	feature_prototype void Set_Mode(ANTARES_MODE mode)
	{
		this_component()->Set_Mode<ComponentType,CallerType,TargetType>(mode);
	}

	feature_prototype typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Allocate_New_Layer<ComponentType,CallerType,TargetType>(name);
	}
	
	feature_prototype typename TargetType::ReturnType Select_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Select_Layer<ComponentType,CallerType,TargetType>(name);
	}
	
	feature_prototype typename TargetType::ReturnType Toggle_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Toggle_Layer<ComponentType,CallerType,TargetType>(name);
	}

	feature_prototype typename TargetType::ReturnType Scale_Coordinates(typename TargetType::ParamType coordinates)
	{
		return (TargetType::ReturnType)this_component()->Scale_Coordinates<ComponentType,CallerType,TargetType>(coordinates);
	}

	feature_accessor(time_panel,none,none);
	feature_accessor(information_panel,none,none);
	feature_accessor(attributes_panel,none,none);
	feature_accessor(control_panel,none,none);
	feature_accessor(layer_options,none,none);

	feature_accessor(graphical_network,none,none);
};