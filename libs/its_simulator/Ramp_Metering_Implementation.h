#pragma once
#include "Ramp_Metering_Prototype.h"

namespace Ramp_Metering_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Ramp_Metering_Implementation:public Polaris_Component<MasterType,INHERIT(Ramp_Metering_Implementation),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Ramp_Metering_Implementation),Execution_Object>::ComponentType ComponentType;
			
			//ramp
			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Null_Prototype,typename MasterType::link_type, on_ramp_link, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::link_type, downstream_freeway_link, NONE, NONE);
			m_data(bool, operation_status, NONE, NONE);

			//detector
			m_data(float, position_first_detector_on_freeway, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, position_second_detector_on_freeway, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, downstream_freeway_detector_length, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//parameter
			m_data(int, starting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, ending_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, metering_updating_interval_length, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, alpha, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, beta, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, downstream_freeway_link_occupancy, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			static float _average_vehicle_length;
			static float _minimum_ramp_flow_rate;

			template<typename TargetType> void ramp_metering_update()
			{
				// Make Interfaces
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network<typename MasterType::network_type> _Network_Interface;

				typedef  Link_Components::Prototypes::Link< type_of(downstream_freeway_link)> _Link_Interface;

				// Figure out time bounds for the signal vs. the simulation interval
				int t_start = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				if (_starting_time <= t_start && _ending_time >= t_end && _operation_status == true)
				{
					///update freeway occupancy - we need to implement a detector in the simulation part
					_downstream_freeway_link_occupancy += float(downstream_freeway_link<_Link_Interface*>()->template num_vehicles_on_link<int>()) * _average_vehicle_length;	//in feet

					///update metering
					if (((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() > _metering_updating_interval_length && ((((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())% _metering_updating_interval_length == 0)
					{//
						///occupancy calculation
						_downstream_freeway_link_occupancy /= (_metering_updating_interval_length) / float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
						_downstream_freeway_link_occupancy /= float(downstream_freeway_link<_Link_Interface*>()->template num_lanes<int>()) ;
						_downstream_freeway_link_occupancy /= _downstream_freeway_detector_length;	//in feet
					
						_downstream_freeway_link_occupancy = min(_downstream_freeway_link_occupancy, 1.0f);

						///ALINEA
						float ramp_maximum_flow_rate = on_ramp_link<_Link_Interface*>()->template maximum_flow_rate<float>();
						//cout << "ramp meter " << _internal_id << " updated from " << ramp_maximum_flow_rate;
						float next_maximum_flow_rate =  ramp_maximum_flow_rate + _alpha * (_beta - _downstream_freeway_link_occupancy);

						///
						float original_maximum_flow_rate = on_ramp_link<_Link_Interface*>()->template original_maximum_flow_rate<float>();
						original_maximum_flow_rate *= on_ramp_link<_Link_Interface*>()->template capacity_adjustment_factor_due_to_weather<float>();
						original_maximum_flow_rate *= on_ramp_link<_Link_Interface*>()->template capacity_adjustment_factor_due_to_accident<float>();
						ramp_maximum_flow_rate = min (next_maximum_flow_rate, original_maximum_flow_rate);
						ramp_maximum_flow_rate = max (ramp_maximum_flow_rate, _minimum_ramp_flow_rate);
						//cout << " to " <<  ramp_maximum_flow_rate << endl;
						on_ramp_link<_Link_Interface*>()->template maximum_flow_rate<float>(ramp_maximum_flow_rate);
						///
						_downstream_freeway_link_occupancy= 0.0f;
					}
				}
				else
				{//original maximum_flow_rate
					float original_maximum_flow_rate = on_ramp_link<_Link_Interface*>()->template original_maximum_flow_rate<float>();
					original_maximum_flow_rate *= on_ramp_link<_Link_Interface*>()->template capacity_adjustment_factor_due_to_weather<float>();
					original_maximum_flow_rate *= on_ramp_link<_Link_Interface*>()->template capacity_adjustment_factor_due_to_accident<float>();

					on_ramp_link<_Link_Interface*>()->template maximum_flow_rate<float>(original_maximum_flow_rate);
				}
			}
			
			template<typename TargetType> static void Initialize_Type(){}

			template<typename TargetType> void Initialize()
			{
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network<typename MasterType::network_type> _Network_Interface;
				_operation_status = true;
				//TODO
//load_event(ComponentType, ComponentType::Ramp_Metering_Conditional,ComponentType::Ramp_Metering,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION,NULLTYPE);
				
				this->template Load_Event<ComponentType>(&ComponentType::Ramp_Metering_Conditional,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION);
			}

			static void Ramp_Metering_Conditional(ComponentType* _this,Event_Response& response)
			{
				//cout << "Ramp meter firing: " << _iteration << "," << sub_iteration() << " : " << Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION << endl;

				typedef Ramp_Metering<ComponentType> _Ramp_Metering_Interface;
				typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network<typename MasterType::network_type> _Network_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Ramp_Metering_Interface* _this_ptr=(_Ramp_Metering_Interface*)_this;
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION)
				{
					//((typename MasterType::ramp_metering_type*)_this)->Swap_Event((Event)&Ramp_Metering<NULLTYPE>);
					//response.result=true;
					_this->Ramp_Metering_Event();
					response.next._iteration=iteration() + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION;
				}
				else
				{
					cout << "Should never reach here in ramp metering conditional!" << endl;
					assert(false);
				}
			}

			void Ramp_Metering_Event()
			{
				//step 1: update ramp metering
				this->template ramp_metering_update<NULLTYPE>();
			}

			template<typename TargetType> bool is_enabled()
			{
				return _operation_status;			
			}
			template<typename TargetType> void enable(bool status)
			{
				_operation_status = status;			
			}
			template<typename TargetType> float meter_flow_ratio()
			{
				typedef  Link_Components::Prototypes::Link< type_of(on_ramp_link)> _Link_Interface;
				float maximum_flow_rate = on_ramp_link<_Link_Interface*>()->template maximum_flow_rate<float>();
				float original_maximum_flow_rate = on_ramp_link<_Link_Interface*>()->template original_maximum_flow_rate<float>();
				float ratio = maximum_flow_rate / original_maximum_flow_rate;
				return ratio;			
			}
		};
		template<typename MasterType,typename InheritanceList>
		float Ramp_Metering_Implementation<MasterType,InheritanceList>::_average_vehicle_length = 20.0;
		template<typename MasterType,typename InheritanceList>
		float Ramp_Metering_Implementation<MasterType,InheritanceList>::_minimum_ramp_flow_rate = 0.15;

	}
}
