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
	m_prototype(Canvas,typename MasterType::canvas_type,canvas, NONE, NONE);
	m_prototype(Information_Panel,typename MasterType::information_panel_type,information_panel, NONE, NONE);
	m_prototype(Time_Panel,typename MasterType::time_panel_type,time_panel, NONE, NONE);

	LARGE_INTEGER _start_timer;
	LARGE_INTEGER _end_timer;
	LARGE_INTEGER _frequency;

	unsigned int _num_records;
	float* _step_time_moving_average;
	float _step_time_moving_sum;
	unsigned int _step_time_index;
	

	template<typename TargetType> void Initialize()
	{
		_next_iteration = -1;

		QueryPerformanceFrequency(&_frequency);
		
		_num_records = 30;

		_step_time_moving_average = new float[_num_records];
		
		for(unsigned int i=0;i<_num_records;i++) _step_time_moving_average[i] = 0.0f;

		_step_time_index = 0;
		_step_time_moving_sum = 0.0f;

		//Load_Event<Conductor_Implementation>(&advance_simulation,0,Scenario_Components::Types::END_OF_ITERATION+2);
		Load_Event<Conductor_Implementation>(&advance_simulation,0,USHRT_MAX);

		_pause=true;
	}

	//static void advance_simulation_condition(ComponentType* _this,Event_Response& response)
	//{
	//	response.result=true;
	//	response.next._iteration=_iteration+1;
	//	response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+2;
	//}

	void Speed_Control()
	{
		QueryPerformanceCounter(&_end_timer);

		int step_time = (((float)_end_timer.QuadPart - (float)_start_timer.QuadPart)/((float)_frequency.QuadPart))*1000;

		_step_time_moving_sum -= _step_time_moving_average[_step_time_index];

		_step_time_moving_average[_step_time_index] = (float)step_time;
		
		if( ++_step_time_index == _num_records )
		{
			_step_time_index = 0;
		}

		_step_time_moving_sum += (float)step_time;

		float historical_delay = _step_time_moving_sum/((float)_num_records);

		int requested_delay = _time_panel->delay<int>();

		if(requested_delay == -1)
		{

		}
		else if(requested_delay == 0)
		{
			//cout << "Selected Delay: " << historical_delay*1.2f << "," << step_time << endl;

			if( historical_delay*1.1f - (float)step_time > 0.0f )
			{
				Sleep( (int)(historical_delay*1.1f - step_time) );
			}
		}
		else
		{
			//cout << "Selected Delay: " << requested_delay << "," << step_time << endl;

			if( requested_delay - step_time > 0 )
			{
				Sleep( requested_delay - step_time );
			}
		}
	}

	static void advance_simulation(Conductor_Implementation* _this,Event_Response& response)
	{
		Conductor_Implementation* pthis=(Conductor_Implementation*)_this;
		if (pthis->_next_iteration >= 0)
		{
			response.next._iteration=pthis->_next_iteration;
			pthis->_next_iteration = -1;
		}
		else
		{
			response.next._iteration=iteration()+1;
		}

		
		//response.next._sub_iteration=Scenario_Components::Types::END_OF_ITERATION+2;
		response.next._sub_iteration=USHRT_MAX;

		

		if(pthis->_pause)
		{
			while(pthis->_pause) Sleep(10);
		}
		else
		{
			pthis->Speed_Control();
		}
		
		QueryPerformanceCounter(&pthis->_start_timer);

		pthis->_canvas->Refresh<NULLTYPE>();

		pthis->_information_panel->Render<NULLTYPE>();
	}

	m_data(bool,pause, NONE, NONE);
	m_data(int, next_iteration, NONE, NONE);

};
