//*********************************************************
//	Conductor_Implementation.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Conductor.h"

//---------------------------------------------------------
//	Conductor_Implementation - conductor class definition
//---------------------------------------------------------

implementation class Conductor_Implementation : public Polaris_Component<MasterType,INHERIT(Conductor_Implementation),Execution_Object>
{
public:
	m_prototype(Canvas<typename MasterType::type_of(canvas)>,canvas,NONE,NONE);
	m_prototype(Information_Panel<typename MasterType::type_of(information_panel)>,information_panel,NONE,NONE);

	void Initialize()
	{
		//Load_Event<Conductor_Implementation>(&advance_simulation,0,Scenario_Components::Types::END_OF_ITERATION+2);
		Load_Event<Conductor_Implementation>(&advance_simulation,0,USHRT_MAX);
		_pause=true;
	}

	//declare_feature_conditional(advance_simulation_condition)
	//{
	//	response.result=true;
	//	response.next._iteration=iteration()+1;
	//	response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+2;
	//}

	//declare_feature_event(advance_simulation)

	static void advance_simulation(Conductor_Implementation* _this,Event_Response& response)
	{
		response.next._iteration=iteration()+1;
		//response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+2;
		response.next._sub_iteration=USHRT_MAX;

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

		pthis->_information_panel->Render();
	}

	m_data(bool,pause,NONE,NONE);

};