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

	feature_prototype void Initialize(typename TargetType::ParamType xmin,typename TargetType::Param2Type ymin,typename TargetType::Param3Type xmax,typename TargetType::Param4Type ymax)
	{
		this_component()->Initialize<ComponentType,CallerType,TargetType>(xmin,ymin,xmax,ymax);
	}

	feature_prototype void Refresh()
	{
		this_component()->Refresh();
	}
	
	//feature_prototype void Set_Mode(ANTARES_MODE mode)
	//{
	//	this_component()->Set_Mode<ComponentType,CallerType,TargetType>(mode);
	//}

	feature_prototype typename TargetType::ReturnType Select_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Select_Layer<ComponentType,CallerType,TargetType>(name);
	}
	
	feature_prototype typename TargetType::ReturnType Toggle_Layer(typename TargetType::ParamType name,typename TargetType::Param2Type checked)
	{
		return (TargetType::ReturnType)this_component()->Toggle_Layer<ComponentType,CallerType,TargetType>(name,checked);
	}

	feature_prototype int Build_Texture(int width, int height, unsigned char* data)
	{
		return this_component()->Build_Texture<ComponentType,CallerType,TargetType>(width,height,data);
	}

	feature_accessor(time_panel,none,none);
	feature_accessor(information_panel,none,none);
	feature_accessor(attributes_panel,none,none);
	feature_accessor(control_panel,none,none);
	feature_accessor(layer_options,none,none);

	feature_accessor(graphical_network,none,none);
};