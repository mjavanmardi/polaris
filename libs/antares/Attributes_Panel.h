//*********************************************************
//	Attributes_Panel.h - Container Panel for Attributes
//*********************************************************

#pragma once
#include "Dependencies.h"

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

	void Push_Attributes(std::vector<pair<string,string>>& attributes)
	{
		this_component()->Push_Attributes(attributes);
	}
};
