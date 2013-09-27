//*********************************************************
//	Information_Panel.h - Information and Plotting
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Information_Page.h"

//---------------------------------------------------------
//	Information_Panel - information panel class definition
//---------------------------------------------------------

prototype struct Information_Panel
{
	tag_as_prototype;
	
	void Render()
	{
		this_component()->Render();
	}
	
	//template<typename TargetType> typename TargetType::ReturnType Allocate_New_Layer(typename TargetType::ParamType name)
	//{
	//	return (TargetType::ReturnType)this_component()->Allocate_New_Layer<ComponentType,TargetType>(name);
	//}

	//accessor(plotwindow,NONE,NONE);
};