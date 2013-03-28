//*********************************************************
//	Control_Dialog.h - Dialog used for Model Control
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Layer_Options.h"

//---------------------------------------------------------
//	Control_Dialog - control dialog class definition
//---------------------------------------------------------

prototype struct Control_Dialog
{
	tag_as_prototype;

	feature_prototype void Push_Schema(typename TargetType::ParamType attributes_schema,typename TargetType::Param2Type dropdown_schema)
	{
		this_component()->Push_Schema<ComponentType,CallerType,TargetType>(attributes_schema,dropdown_schema);
	}

	feature_prototype void Push_Attributes(typename TargetType::ParamType attributes)
	{
		this_component()->Push_Attributes<ComponentType,CallerType,TargetType>(attributes);
	}

	feature_prototype void ShowModal()
	{
		this_component()->ShowModal();
	}

	feature_accessor(submission_callback,none,none);
	feature_accessor(selected_object,none,none);
};