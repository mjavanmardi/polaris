//*********************************************************
//	Attributes_Panel.h - Container Panel for Attributes
//*********************************************************

#pragma once
#include "Antares_Includes.h"

//---------------------------------------------------------
//	Attributes_Panel - attributes panel class definition
//---------------------------------------------------------

prototype struct Attributes_Panel
{
	tag_as_prototype;

	//feature_prototype void Push_Schema(typename TargetType::ParamType schema)
	//{
	//	this_component()->Push_Schema<ComponentType,CallerType,TargetType>(schema);
	//}

	feature_prototype void Push_Attributes(typename TargetType::ParamType attributes)
	{
		this_component()->Push_Attributes<ComponentType,CallerType,TargetType>(attributes);
	}
};