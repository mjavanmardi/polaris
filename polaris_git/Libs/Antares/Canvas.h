//*********************************************************
//	Canvas.h - Visualization Panel
//*********************************************************

#pragma once
#include "Dependencies.h"
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

	template<typename TargetType> void Refresh()
	{
		this_component()->Refresh();
	}
	
	//template<typename TargetType> void Set_Mode(ANTARES_MODE mode)
	//{
	//	this_component()->Set_Mode<ComponentType,TargetType>(mode);
	//}

	void Select_Layer(int name)
	{
		return this_component()->Select_Layer(name);
	}
	
	void Toggle_Layer(int name,bool checked)
	{
		return this_component()->Toggle_Layer(name,checked);
	}

	template<typename TargetType> int Build_Texture(int width, int height, unsigned char* data)
	{
		return this_component()->Build_Texture<ComponentType,TargetType>(width,height,data);
	}

	accessor(time_panel,NONE,NONE);
	accessor(information_panel,NONE,NONE);
	accessor(attributes_panel,NONE,NONE);
	accessor(control_panel,NONE,NONE);
	accessor(layer_options,NONE,NONE);

	accessor(graphical_network,NONE,NONE);
};