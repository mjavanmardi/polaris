//*********************************************************
//	Conductor_Implementation.h - Controls Visualization Progression
//*********************************************************

#pragma once
#include "Conductor.h"

//---------------------------------------------------------
//	Conductor_Implementation - conductor class definition
//---------------------------------------------------------

implementation class Conductor_Implementation : public Polaris_Component_Class<Conductor_Implementation,MasterType,Execution_Object,ParentType>
{
public:
	feature_implementation void Initialize()
	{
		Load_Register<Conductor_Implementation>(&advance_simulation_condition<NULLTYPE>,&advance_simulation<NULLTYPE>,0,Scenario_Components::Types::END_OF_ITERATION);
		_pause=true;
	}

	declare_feature_conditional(advance_simulation_condition)
	{
		response.result=true;
		response.next._iteration=_iteration+1;
		response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION;
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
			Sleep(100);
		}

		cout << _iteration << endl;
	}

	member_data(bool,pause,none,none);
};