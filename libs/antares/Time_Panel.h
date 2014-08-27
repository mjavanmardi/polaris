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
		return this_component()->Is_Running<TargetType>();
	}

	template<typename TargetType> void Enable_Time_Navigation(TargetType object_to_reschedule_when_time_changes)
	{
		this_component()->Enable_Time_Navigation();
		this->object_to_reschedule<TargetType>(object_to_reschedule_when_time_changes);
	}

	accessor(play, NONE, NONE);
	accessor(play_button, NONE, NONE);
	accessor(pause_button, NONE, NONE);
	accessor(sizer, NONE, NONE);
	accessor(time_display, NONE, NONE);

	accessor(canvas, NONE, NONE);

	accessor(delay, NONE, NONE);

	accessor(object_to_reschedule,NONE,NONE);
	accessor(reschedule_callback,NONE,NONE);
};
