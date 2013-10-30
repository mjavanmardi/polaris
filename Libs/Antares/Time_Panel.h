//*********************************************************
//	Time_Panel.h - Time Navigation Control
//*********************************************************

#pragma once
#include "Dependencies.h"

//---------------------------------------------------------
//	Time_Panel - time panel class definition
//---------------------------------------------------------

prototype struct Time_Panel
{
	tag_as_prototype;

	template<typename TargetType> void Update_Time(int updated_time)
	{
		this_component()->Update_Time(updated_time);
	}

	template<typename TargetType> bool Is_Running()
	{
		return this_component()->Is_Running<ComponentType,TargetType>();
	}

	accessor(play,NONE,NONE);
	accessor(play_button,NONE,NONE);
	accessor(pause_button,NONE,NONE);
	accessor(sizer,NONE,NONE);
	accessor(time_display,NONE,NONE);
};