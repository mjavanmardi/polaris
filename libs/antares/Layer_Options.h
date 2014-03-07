//*********************************************************
//	Layer_Options.h - Graphical Layer Options
//*********************************************************

#pragma once
#include "Dependencies.h"

//---------------------------------------------------------
//	Layer_Options - layer options definition
//---------------------------------------------------------

prototype struct Layer_Options
{
	tag_as_prototype;

	accessor(canvas, NONE, NONE);

	void Allocate_New_Layer(string& name)
	{
		this_component()->Allocate_New_Layer(name);
	}

	template<typename TargetType> void Toggle_Named_Layer(string& name,bool check_state)
	{
		this_component()->Toggle_Named_Layer<TargetType>(name,check_state);
	}
};
