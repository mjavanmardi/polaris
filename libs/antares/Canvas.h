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

	void Initialize(float xmin,float ymin,float xmax,float ymax)
	{
		this_component()->Initialize(xmin,ymin,xmax,ymax);
	}
	
	template<typename TargetType> void Start_Recording()
	{
		this_component()->Start_Recording();
	}
	
	template<typename TargetType> void Stop_Recording()
	{
		this_component()->Stop_Recording();
	}

	template<typename TargetType> void Refresh()
	{
		this_component()->Refresh();
	}
	
	//feature_prototype void Set_Mode(ANTARES_MODE mode)
	//{
	//	this_component()->Set_Mode<ComponentType,CallerType,TargetType>(mode);
	//}

	template<typename TargetType> typename TargetType::ReturnType Select_Layer(typename TargetType::ParamType name)
	{
		return (TargetType::ReturnType)this_component()->Select_Layer<TargetType>(name);
	}
	
	template<typename TargetType> typename TargetType::ReturnType Toggle_Layer(typename TargetType::ParamType name,typename TargetType::Param2Type checked)
	{
		return (TargetType::ReturnType)this_component()->Toggle_Layer<TargetType>(name,checked);
	}

	template<typename TargetType> int Build_Texture(int width, int height, unsigned char* data)
	{
		return this_component()->Build_Texture<TargetType>(width,height,data);
	}

	accessor(time_panel, NONE, NONE);
	accessor(information_panel, NONE, NONE);
	accessor(attributes_panel, NONE, NONE);
	accessor(control_panel, NONE, NONE);
	accessor(layer_options, NONE, NONE);
	
	accessor(scale, NONE, NONE);
	accessor(x_rotation, NONE, NONE);
	accessor(z_rotation, NONE, NONE);
	accessor(x_translation, NONE, NONE);
	accessor(y_translation, NONE, NONE);

	accessor(spatial_change, NONE, NONE);

	accessor(graphical_network, NONE, NONE);
};
