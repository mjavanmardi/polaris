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
		implementation struct Polaris_Ramp_Metering_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Ramp_Metering_Implementation),MasterType,Execution_Object,ParentType>
		{
			//ramp
			member_data(int, internal_id, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_component(typename MasterType::link_type, on_ramp_link, none, none);
			member_component(typename MasterType::link_type, downstream_freeway_link, none, none);

			//detector
			member_data(float, position_first_detector_on_freeway, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, position_second_detector_on_freeway, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, downstream_freeway_detector_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//parameter
			member_data(int, starting_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, ending_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, metering_updating_interval_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, alpha, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, beta, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, downstream_freeway_link_occupancy, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			static float _average_vehicle_length;
			static float _minimum_ramp_flow_rate;

			feature_implementation void ramp_metering_update()
			{
				// Make Interfaces
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;

				define_component_interface(_Link_Interface, type_of(downstream_freeway_link), Link_Components::Prototypes::Link_Prototype, ComponentType);

				// Figure out time bounds for the signal vs. the simulation interval
				int t_start = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>()%(24*60*60);
				int t_end = t_start + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				if (_starting_time <= t_start && _ending_time >= t_end)
				{
					///update freeway occupancy - we need to implement a detector in the simulation part
					_downstream_freeway_link_occupancy += float(downstream_freeway_link<ComponentType,CallerType,_Link_Interface*>()->template num_vehicles_on_link<int>()) * _average_vehicle_length;	//in feet

					///update metering
					if (((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() > _metering_updating_interval_length && ((((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())% _metering_updating_interval_length == 0)
					{//
						///occupancy calculation
						_downstream_freeway_link_occupancy /= (_metering_updating_interval_length) / float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
						_downstream_freeway_link_occupancy /= float(downstream_freeway_link<ComponentType,CallerType,_Link_Interface*>()->template num_lanes<int>()) ;
						_downstream_freeway_link_occupancy /= _downstream_freeway_detector_length;	//in feet
					
						_downstream_freeway_link_occupancy = min(_downstream_freeway_link_occupancy, 1.0f);

						///ALINEA
						float ramp_maximum_flow_rate = on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template maximum_flow_rate<float>();
						//cout << "ramp meter " << _internal_id << " updated from " << ramp_maximum_flow_rate;
						float next_maximum_flow_rate =  ramp_maximum_flow_rate + _alpha * (_beta - _downstream_freeway_link_occupancy);

						///
						float original_maximum_flow_rate = on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template original_maximum_flow_rate<float>();
						original_maximum_flow_rate *= on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template capacity_adjustment_factor_due_to_weather<float>();
						original_maximum_flow_rate *= on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template capacity_adjustment_factor_due_to_accident<float>();
						ramp_maximum_flow_rate = min (next_maximum_flow_rate, original_maximum_flow_rate);
						ramp_maximum_flow_rate = max (ramp_maximum_flow_rate, _minimum_ramp_flow_rate);
						//cout << " to " <<  ramp_maximum_flow_rate << endl;
						on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template maximum_flow_rate<float>(ramp_maximum_flow_rate);
						///
						_downstream_freeway_link_occupancy= 0.0f;
					}
				}
				else
				{//original maximum_flow_rate
					float original_maximum_flow_rate = on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template original_maximum_flow_rate<float>();
					original_maximum_flow_rate *= on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template capacity_adjustment_factor_due_to_weather<float>();
					original_maximum_flow_rate *= on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template capacity_adjustment_factor_due_to_accident<float>();

					on_ramp_link<ComponentType,CallerType,_Link_Interface*>()->template maximum_flow_rate<float>(original_maximum_flow_rate);
				}
			}
			
			feature_implementation void Initialize()
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				load_event(ComponentType,ComponentType::Ramp_Metering_Conditional,Ramp_Metering,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION,NULLTYPE);
			}

			declare_feature_conditional(Ramp_Metering_Conditional)
			{
				typedef Ramp_Metering_Prototype<ComponentType, ComponentType> _Ramp_Metering_Interface;
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Ramp_Metering_Interface* _this_ptr=(_Ramp_Metering_Interface*)_this;
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION)
				{
					((typename MasterType::ramp_metering_type*)_this)->Swap_Event((Event)&Ramp_Metering<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION;

				}
				else
				{
					cout << "Should never reach here in ramp metering conditional!" << endl;
					assert(false);
				}
			}

			declare_feature_event(Ramp_Metering)
			{
				typedef Ramp_Metering_Prototype<ComponentType, ComponentType> _Ramp_Metering_Interface;
				_Ramp_Metering_Interface* _this_ptr=(_Ramp_Metering_Interface*)_this;
				//step 1: update ramp metering
				_this_ptr->template ramp_metering_update<NULLTYPE>();
			}
		};
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Polaris_Ramp_Metering_Implementation<MasterType,ParentType,InheritanceList>::_average_vehicle_length = 20.0;
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Polaris_Ramp_Metering_Implementation<MasterType,ParentType,InheritanceList>::_minimum_ramp_flow_rate = 0.15;

	}
}