#pragma once
#include "Movement_Plan_Prototype.h"


namespace Movement_Plan_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Trajectory_Unit_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Trajectory_Unit_Implementation),MasterType,Data_Object,ParentType>
		{
			member_data(int, delayed_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, enter_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, estimated_link_accepting_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_component(typename MasterType::link_type, link, none, none);

			feature_implementation void Initialize(TargetType link_val)
			{
				_link = (type_of(link)*)link_val;
				_enter_time = 0.0;
				_delayed_time=0.0;
			}

		};

		implementation struct Polaris_Movement_Plan_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Movement_Plan_Implementation),MasterType,Data_Object,ParentType>
		{
			typedef Implementations::Polaris_Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;
			member_container(vector<trajectory_unit_type*>, trajectory_container, none, none);
			
			//==================================================================================================================
			/// current_trajectory_position
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation TargetType current_trajectory_position(requires(check_2(TargetType,int,is_same) || check_2(TargetType,int&,is_same)))
			{
				return (TargetType)_current_trajectory_index;
			}
			feature_implementation TargetType current_trajectory_position(requires(!check_2(TargetType,int,is_same) && !check_2(TargetType,int&,is_same)))
			{
				return (TargetType)_trajectory_container[_current_trajectory_index];
			}
			feature_implementation void current_trajectory_position(TargetType val,requires(check_2(TargetType,int,is_same) || check_2(TargetType,int&,is_same)))
			{
				_current_trajectory_index=val;
			}
			tag_getter_setter_as_available(current_trajectory_position);
			int _current_trajectory_index;
			//------------------------------------------------------------------------------------------------------------------

			member_component(typename MasterType::activity_location_type, origin_location, none, none);
			member_component(typename MasterType::activity_location_type, destination_location, none, none);

			member_component(typename MasterType::zone_type, origin_zone, none, none);
			member_component(typename MasterType::zone_type, destination_zone, none, none);

			member_component(typename MasterType::link_type, origin, none, none);
			member_component(typename MasterType::link_type, destination, none, none);

			member_component_and_feature_accessor(departed_time,Value,Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(planning_time,Value,Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(arrived_time,Value,Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(expected_travel_time,Value,Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>);

			member_component(typename MasterType::plan_type, plan, none, none);
			member_data(int, routed_travel_time, none, none);
			member_data(int, estimated_travel_time_when_departed, none, none);
			member_data(int, number_of_switches, none, none);
			member_data(int, estimated_time_of_arrival, none, none);
			member_data(bool, is_integrated, none, none);

			template<typename ComponentType, typename CallerType, typename ReturnValueType>
			ReturnValueType absolute_departure_time()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				return (ReturnValueType)(int(departed_time<ComponentType,CallerType,Simulation_Timestep_Increment>()) + ((_Scenario_Interface*)_global_scenario)->template simulation_start_time<int>());
			}
			tag_getter_as_available(absolute_departure_time);

			feature_implementation void arrive_to_destination()
			{

				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type,ComponentType> _Network_Interface;
				_trajectory_container[_current_trajectory_index]->_delayed_time = 0.0;
				this->template arrived_time<ComponentType,CallerType,Simulation_Timestep_Increment>( ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() );
			}

			feature_implementation void transfer_to_next_link(int delayed_time)
			{

				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type,ComponentType> _Network_Interface;
				typedef Movement_Plan_Components::Prototypes::Trajectory_Unit_Prototype<typename MasterType::trajectory_unit_type,ComponentType> _Trajectory_Unit_Interface;

				if (_current_trajectory_index != -1)
				{
					_trajectory_container[_current_trajectory_index]->_delayed_time = delayed_time;
				}
				
				((Movement_Plan_Prototype<ComponentType,ComponentType>*)this)->template advance_trajectory<_Trajectory_Unit_Interface*>();
				_trajectory_container[_current_trajectory_index]->_enter_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>();
			}

			member_data(bool, valid_trajectory,none,none);
		};

		implementation struct Polaris_Integrated_Movement_Plan_Implementation : public Polaris_Movement_Plan_Implementation<MasterType,ParentType, APPEND_CHILD(Polaris_Integrated_Movement_Plan_Implementation)>
		{
			typedef Polaris_Movement_Plan_Implementation<MasterType,ParentType, APPEND_CHILD(Polaris_Integrated_Movement_Plan_Implementation)> Base_Type;
			typedef typename Base_Type::Component_Type ComponentType;
			member_prototype(Activity_Components::Prototypes::Activity_Planner, destination_activity_reference, typename MasterType::activity_type,none,none);
			feature_implementation void arrive_to_destination()
			{
				Base_Type* bthis = (Base_Type*)this;

				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type,ComponentType> _Network_Interface;
				//bthis->_trajectory_container[bthis->_current_trajectory_index]->_delayed_time = 0.0;
				bthis->template arrived_time<ComponentType,CallerType,Simulation_Timestep_Increment>( ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() );

				if (Base_Type::_is_integrated)
				{
					Simulation_Timestep_Increment ttime = bthis->template arrived_time<ComponentType,CallerType,Simulation_Timestep_Increment>() - bthis->template departed_time<ComponentType,CallerType,Simulation_Timestep_Increment>();
					this->_destination_activity_reference->template Actual_Travel_Time<Simulation_Timestep_Increment>(ttime);
					this->_destination_activity_reference->template Arrive_At_Activity<NT>();
				}
			}
		};

		implementation struct Polaris_Movement_Plan_Record_Implementation : public Polaris_Component<APPEND_CHILD(Polaris_Movement_Plan_Record_Implementation),MasterType,Data_Object,ParentType>
		{
			typedef typename Polaris_Movement_Plan_Implementation<MasterType,ParentType, APPEND_CHILD(Polaris_Movement_Plan_Record_Implementation)>::Component_Type ComponentType;
			
			// Initialize the record with an existing movement plan
			feature_implementation void Initialize(typename TargetType::ParamType movement_to_copy)
			{
				// get interface to the input parameter
				typedef Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type> movement_itf;
				movement_itf* move = (movement_itf*)movement_to_copy;

				// interace to the movement plan trajectory
				define_container_and_value_interface(trajectory_container_itf,trajectory_itf,typename movement_itf::get_type_of(trajectory_container),Containers::Random_Access_Sequence_Prototype,Prototypes::Trajectory_Unit_Prototype,ComponentType);				
				trajectory_container_itf* trajectory = move->trajectory_container<trajectory_container_itf*>();

				// Extract the link pointer from the trajectory unit and store in the movement_plan_record trajectory container
				for (typename trajectory_container_itf::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
				{
					trajectory_itf* traj_unit = *itr;
					_trajectory_container.push_back(traj_unit->template link<typename MasterType::link_type*>());
				}

				// copy valid movement at time of creation
				_valid_trajectory = move->valid_trajectory<bool>();

				// copy pointer to the activity reference from original movement plan
				_destination_activity_reference = move->destination_activity_reference<destination_activity_reference_interface*>();
			}	 
			tag_feature_signature_as_available(Initialize, 1);

			member_prototype(Activity_Components::Prototypes::Activity_Planner, destination_activity_reference, typename MasterType::activity_type,none,none);
			member_container(vector<typename MasterType::link_type*>, trajectory_container, none, none);
			member_data(bool, valid_trajectory,none,none);
		};

	}
}