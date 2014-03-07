//*********************************************************
//	Control_Dialog.h - Dialog used for Model Control
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Layer_Options.h"

//---------------------------------------------------------
//	Control_Dialog - control dialog class definition
//---------------------------------------------------------

prototype struct Control_Dialog
{
	tag_as_prototype;

	template<typename TargetType> void Push_Schema(typename TargetType::ParamType attributes_schema,typename TargetType::Param2Type dropdown_schema)
	{
		this_component()->Push_Schema<TargetType>(attributes_schema,dropdown_schema);
	}

	template<typename TargetType> void Push_Attributes(typename TargetType::ParamType attributes)
	{
		this_component()->Push_Attributes<TargetType>(attributes);
	}

	template<typename TargetType> void ShowModal()
	{
		this_component()->ShowModal();
	}

	accessor(submission_callback, NONE, NONE);
	accessor(selected_object, NONE, NONE);
};
