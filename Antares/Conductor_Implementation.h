//*********************************************************
//	Conductor_Implementation.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Conductor.h"

//---------------------------------------------------------
//	Conductor_Implementation - conductor class definition
//---------------------------------------------------------

implementation class Conductor_Implementation : public Polaris_Component<APPEND_CHILD(Conductor_Implementation),MasterType,Execution_Object,ParentType>
{
public:
	member_prototype(Canvas,canvas,typename MasterType::type_of(canvas),none,none);
	member_prototype(Information_Panel,information_panel,typename MasterType::type_of(information_panel),none,none);

	feature_implementation void Initialize()
	{
		Load_Event<Conductor_Implementation>(&advance_simulation_condition<NULLTYPE>,&advance_simulation<NULLTYPE>,0,Scenario_Components::Types::END_OF_ITERATION+1);
		_pause=true;
	}

	declare_feature_conditional(advance_simulation_condition)
	{
		response.result=true;
		response.next._iteration=_iteration+1;
		response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
	}

	declare_feature_event(advance_simulation)
	{
		Conductor_Implementation* pthis=(Conductor_Implementation*)_this;

		if(pthis->_pause)
		{
			while(pthis->_pause) Sleep(10);
		}
		else
		{
			//Sleep(100);
		}

		pthis->_canvas->Refresh<NULLTYPE>();

		pthis->_information_panel->Render<NULLTYPE>();
	}

	member_data(bool,pause,none,none);

};