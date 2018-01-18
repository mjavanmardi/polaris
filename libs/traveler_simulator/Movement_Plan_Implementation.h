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
		implementation struct Trajectory_Unit_Implementation:public Polaris_Component<MasterType,INHERIT(Trajectory_Unit_Implementation),Data_Object>
		{
			m_data(int, delayed_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_interval_index, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, estimated_link_accepting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, intersection_delay_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic)); 
			
			m_prototype(Null_Prototype,typename MasterType::link_type, link, NONE, NONE);

			template<typename TargetType> void Initialize(TargetType link_val);

		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Trajectory_Unit_Implementation<MasterType, InheritanceList>::Initialize(TargetType link_val)
		{
			_link = (Null_Prototype<typename MasterType::link_type>*)(type_of(link)*)link_val;
			_enter_time = 0.0;
			_delayed_time = 0.0;
			_intersection_delay_time = 0.0;
		}

		implementation struct Movement_Plan_Implementation:public Polaris_Component<MasterType,INHERIT(Movement_Plan_Implementation),Data_Object>
		{
			typedef Polaris_Component<MasterType,INHERIT(Movement_Plan_Implementation),Data_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;

			m_static_prototype(Network,typename MasterType::network_type,network,NONE,NONE);

			typedef Implementations::Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;
			m_container(std::vector<trajectory_unit_type*>, trajectory_container, NONE, NONE);
			
			//==================================================================================================================
			/// current_trajectory_position
			//------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> TargetType current_trajectory_position(requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)));
			template<typename TargetType> TargetType current_trajectory_position(requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)));
			template<typename TargetType> void current_trajectory_position(TargetType val, requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)));
			template<typename TargetType> void current_trajectory_position(TargetType val, requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)));
			tag_getter_setter_as_available(current_trajectory_position);

			//------------------------------------------------------------------------------------------------------------------

			m_prototype(Activity_Location,typename MasterType::activity_location_type, origin_location, NONE, NONE);
			m_prototype(Activity_Location,typename MasterType::activity_location_type, destination_location, NONE, NONE);

			//m_prototype(Null_Prototype,typename MasterType::zone_type>, origin_zone, NONE, NONE);
			//m_prototype(Null_Prototype,typename MasterType::zone_type>, destination_zone, NONE, NONE);

			m_prototype(Link,typename MasterType::link_type, origin, NONE, NONE);
			m_prototype(Link,typename MasterType::link_type, destination, NONE, NONE);

			member_component_and_feature_accessor(departed_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(planning_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(arrived_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(expected_travel_time,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::template Time_Implementation<NT>);

			m_prototype(Null_Prototype,typename MasterType::plan_type, plan, NONE, NONE);
			m_data(int, routed_travel_time, NONE, NONE);
			m_data(int, estimated_travel_time_when_departed, NONE, NONE);
			m_data(int, number_of_switches, NONE, NONE);
			m_data(int, estimated_time_of_arrival, NONE, NONE);
			m_data(float, route_length, NONE, NONE);
			m_data(int, entry_time, NONE, NONE);
			m_data(int, traveler_id, NONE, NONE);
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode, NONE, NONE);

			//TODO: Remove when done testing routing execution time
			m_data(int, routing_execution_time, NONE, NONE);

			template<typename TargetType>
			TargetType absolute_departure_time();
			tag_getter_as_available(absolute_departure_time);

			void arrive_to_destination();

			template<typename TargetType> void transfer_to_next_link(int delayed_time);

			void Display_Movement();
			
			int _current_trajectory_index;
			m_data(bool, valid_trajectory, NONE, NONE);
			m_data(bool, is_integrated, NONE, NONE);

			m_data(std::string, summary_string, NONE, NONE);
			m_data(std::string, detail_string, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList>
		Network<typename MasterType::network_type>* Movement_Plan_Implementation<MasterType,InheritanceList>::_network;

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)))
		{
			return (TargetType)_current_trajectory_index;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)))
		{
			TargetType pos = nullptr;
			if (_trajectory_container.size() == 0) return nullptr;
			if (_current_trajectory_index >= _trajectory_container.size())
			{
				cout << "Error: current_trajectory_index greater than trajectory size." << endl;
				cout << "index=" << _current_trajectory_index << ", size=" << _trajectory_container.size() << endl;
				return nullptr;
			}

			try
			{
				pos = (TargetType)_trajectory_container[_current_trajectory_index];
			}
			catch (std::exception& e)
			{
				cout << "Trajectory container pointer=" << &_trajectory_container << ", size=" << _trajectory_container.size() << ", Index=" << _current_trajectory_index << ", Exception: " << e.what() << endl;
			}

			return pos;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(TargetType val, requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)))
		{
			_current_trajectory_index = val;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(TargetType val, requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)))
		{
			_current_trajectory_index = val;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Movement_Plan_Implementation<MasterType, InheritanceList>::absolute_departure_time()
		{
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			return (TargetType)(int(departed_time<Simulation_Timestep_Increment>()) + ((_Scenario_Interface*)_global_scenario)->template simulation_start_time<int>());
		}

		template<typename MasterType, typename InheritanceList>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::arrive_to_destination()
		{

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			_trajectory_container[_current_trajectory_index]->template delayed_time<int>(0.0);
			this->template arrived_time<Simulation_Timestep_Increment>(((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>());
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::transfer_to_next_link(int delayed_time)
		{

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename MasterType::trajectory_unit_type> _Trajectory_Unit_Interface;

			if (_current_trajectory_index != -1)
			{
				_trajectory_container[_current_trajectory_index]->delayed_time(delayed_time);
			}

			((Movement_Plan<ComponentType>*)this)->template advance_trajectory<_Trajectory_Unit_Interface*>();
			//_trajectory_container[_current_trajectory_index]->_enter_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>();
			_trajectory_container[_current_trajectory_index]->enter_time(iteration());
		}

		template<typename MasterType, typename InheritanceList>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::Display_Movement()
		{
			cout << endl << "---------------------------------------" << endl;
			cout << "Ptr=" << this << endl;
			cout << "Departure=" << this->__departed_time._Value << endl;
			cout << "Origin=" << this->_origin_location << endl;
			cout << "Destination=" << this->_destination_location << endl;
			cout << "Trajectory length=" << this->_trajectory_container.size() << endl;
			cout << "Plan time=" << this->__planning_time._Value << endl;
			cout << "---------------------------------------" << endl;
		}

		implementation struct Integrated_Movement_Plan_Implementation : public Movement_Plan_Implementation<MasterType, INHERIT(Integrated_Movement_Plan_Implementation)>
		{
			typedef Movement_Plan_Implementation<MasterType, INHERIT(Integrated_Movement_Plan_Implementation)> Base_Type;
			typedef typename Base_Type::Component_Type ComponentType;

			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, destination_activity_reference, NONE, NONE);
			void arrive_to_destination();
		};
		
		template<typename MasterType, typename InheritanceList>
		void Integrated_Movement_Plan_Implementation<MasterType, InheritanceList>::arrive_to_destination()
		{
			Base_Type* bthis = (Base_Type*)this;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			//bthis->_trajectory_container[bthis->_current_trajectory_index]->_delayed_time = 0.0;
			bthis->template arrived_time<Simulation_Timestep_Increment>(((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>());

			if (Base_Type::_is_integrated && this->_destination_activity_reference != nullptr)
			{
				Simulation_Timestep_Increment ttime = bthis->template arrived_time<Simulation_Timestep_Increment>() - bthis->template departed_time<Simulation_Timestep_Increment>();
				this->_destination_activity_reference->template Actual_Travel_Time<Simulation_Timestep_Increment>(ttime);
				this->_destination_activity_reference->Arrive_At_Activity();
			}
		}

		implementation struct Movement_Plan_Record_Implementation : public Polaris_Component<MasterType,INHERIT(Movement_Plan_Record_Implementation),Data_Object>
		{
			typedef typename Movement_Plan_Implementation<MasterType, INHERIT(Movement_Plan_Record_Implementation)>::Component_Type ComponentType;

// TODO: this does not compile
			// Initialize the record with an existing movement plan
			template<typename TargetType> void Initialize(TargetType movement_to_copy);

			m_prototype(Null_Prototype, typename MasterType::activity_type, destination_activity_reference, NONE, NONE);
			m_container(std::vector<typename MasterType::link_type*>, trajectory_container, NONE, NONE);
			m_data(bool, valid_trajectory, NONE, NONE);
		};
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Movement_Plan_Record_Implementation<MasterType, InheritanceList>::Initialize(TargetType movement_to_copy)
		{
			// get interface to the input parameter
			typedef Prototypes::Movement_Plan<typename MasterType::movement_plan_type> movement_itf;
			movement_itf* move = (movement_itf*)movement_to_copy;

			// interace to the movement plan trajectory
			typedef Prototypes::Trajectory_Unit<typename remove_pointer<typename movement_itf::get_type_of(trajectory_container)::value_type>::type> trajectory_itf;
			typedef Random_Access_Sequence<typename movement_itf::get_type_of(trajectory_container), trajectory_itf*> trajectory_container_itf;

			trajectory_container_itf* trajectory = move->trajectory_container<trajectory_container_itf*>();

			// Extract the link pointer from the trajectory unit and store in the movement_plan_record trajectory container
			for (typename trajectory_container_itf::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
			{
				trajectory_itf* traj_unit = *itr;
				_trajectory_container.push_back(traj_unit->template link<typename MasterType::link_type*>());
			}

			// copy valid movement at time of creation
			_valid_trajectory = move->template valid_trajectory<bool>();

			// copy pointer to the activity reference from original movement plan
			//_destination_activity_reference = move->template destination_activity_reference<destination_activity_reference_interface*>();
		}

	}
}
