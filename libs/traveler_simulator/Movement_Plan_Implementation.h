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
		implementation struct Trajectory_Unit_Implementation :public Polaris_Component<MasterType, INHERIT(Trajectory_Unit_Implementation), Data_Object>
		{
			m_data(int, delayed_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_interval_index, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, estimated_link_accepting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, intersection_delay_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_prototype(Null_Prototype, typename MasterType::link_type, link, NONE, NONE);

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


		//=====================================================================
		// BASIC MOVEMENT PLAN
		//---------------------------------------------------------------------
		implementation struct Movement_Plan_Implementation :public Polaris_Component<MasterType, INHERIT(Movement_Plan_Implementation), Data_Object>
		{
			Movement_Plan_Implementation()
			{
				int test = 1;
			}
			typedef Polaris_Component<MasterType, INHERIT(Movement_Plan_Implementation), Data_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;

			//m_static_prototype(Network,typename MasterType::network_type,network,NONE,NONE);
			m_prototype(Network, typename MasterType::network_type, network, NONE, NONE);

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

			m_prototype(Activity_Location, typename MasterType::activity_location_type, origin_location, NONE, NONE);
			m_prototype(Activity_Location, typename MasterType::activity_location_type, destination_location, NONE, NONE);

			//m_prototype(Null_Prototype,typename MasterType::zone_type>, origin_zone, NONE, NONE);
			//m_prototype(Null_Prototype,typename MasterType::zone_type>, destination_zone, NONE, NONE);

			m_prototype(Link_Components::Prototypes::Link, typename MasterType::link_type, origin, NONE, NONE);
			m_prototype(Link_Components::Prototypes::Link, typename MasterType::link_type, destination, NONE, NONE);

			member_component_and_feature_accessor(departed_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(planning_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(arrived_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::template Time_Implementation<NT>);
			member_component_and_feature_accessor(expected_travel_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::template Time_Implementation<NT>);

			m_prototype(Null_Prototype, typename MasterType::plan_type, plan, NONE, NONE);
			m_data(int, routed_travel_time, NONE, NONE);
			m_data(int, estimated_travel_time_when_departed, NONE, NONE);
			m_data(int, number_of_switches, NONE, NONE);
			m_data(int, estimated_time_of_arrival, NONE, NONE);
			m_data(float, route_length, NONE, NONE);
			m_data(int, entry_time, NONE, NONE);
			m_data(int, traveler_id, NONE, NONE);
			m_data(int, path_id, NONE, NONE); // update after completed trajectory is pushed to DB
			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode, NONE, NONE);
			m_data(float, experienced_gap, NONE, NONE);

			//TODO: Remove when done testing routing execution time
			m_data(int, routing_execution_time, NONE, NONE);

			template<typename TargetType>
			TargetType absolute_departure_time();
			tag_getter_as_available(absolute_departure_time);

			void arrive_to_destination();

			template<typename TargetType> void transfer_to_next_link(int delayed_time);

			void Display_Movement();

			void set_trajectory(std::deque<global_edge_id>& path_container, std::deque<float>& arrival_time_container);
			void clear_trajectory();
			template<typename TargetType> void update_route_length();

			int _current_trajectory_index;

			m_data(bool, valid_trajectory, NONE, NONE);
			m_data(bool, is_integrated, NONE, NONE);
			m_data(bool, is_multimodal, NONE, NONE);

			m_data(std::string, summary_string, NONE, NONE);
			m_data(std::string, detail_string, NONE, NONE);
		};

		//template<typename MasterType,typename InheritanceList>
		//Network<typename MasterType::network_type>* Movement_Plan_Implementation<MasterType,InheritanceList>::_network;

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
			typedef Demand_Components::Prototypes::Demand<typename MasterType::demand_type> _Demand_Interface;
			_trajectory_container[_current_trajectory_index]->template delayed_time<int>(0.0);
			this->template arrived_time<Simulation_Timestep_Increment>(((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>());

			// write to global demand if turned on
			((_Demand_Interface*)_global_demand)->Add_Trip_Record(this);
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

		template<typename MasterType, typename InheritanceList>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::set_trajectory(std::deque<global_edge_id>& path_container, std::deque<float>& arrival_time_container)
		{
			//TODO: check that this has been correctly translated!

			typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), Trajectory_Unit_Interface*> Trajectory_Container_Interface;

			typedef  Link_Components::Prototypes::Link< typename Trajectory_Unit_Interface::get_type_of(link)> Link_Interface;

			typedef  Network_Components::Prototypes::Network< typename get_type_of(network) > Network_Interface;


			Network_Interface* net = network<Network_Interface*>();



			Trajectory_Container_Interface& trajectory = trajectory_container<Trajectory_Container_Interface&>();
			//trajectory.clear();
			clear_trajectory();

			typename std::deque<global_edge_id>::iterator itr;
			typename std::deque<float>::iterator arrival_time_itr;

			for (itr = path_container.begin(), arrival_time_itr = arrival_time_container.begin(); itr != path_container.end(); itr++, arrival_time_itr++)
			{
				Trajectory_Unit_Interface* vehicle_trajectory_data = (Trajectory_Unit_Interface*)Allocate<typename Trajectory_Unit_Interface::Component_Type>();

				Link_Interface* link = net->template get_link_ptr< typename Trajectory_Unit_Interface::get_type_of(link) >(itr->edge_id);


				vehicle_trajectory_data->template Initialize<Link_Interface*>(link);

				if (arrival_time_itr != arrival_time_container.begin()) vehicle_trajectory_data->template estimated_link_accepting_time<int>(*(arrival_time_itr - 1));
				else vehicle_trajectory_data->template estimated_link_accepting_time<int>(0.0f);


				trajectory.push_back(vehicle_trajectory_data);
			}

			number_of_switches<int>(0.0);
			update_route_length<NT>();

			//ofstream paths("path_data.txt",ios::app);

			//for(Trajectory_Container_Interface::iterator itr = trajectory.begin();itr!=trajectory.end();itr++)
			//{
			//	Trajectory_Unit_Interface* trajectory_unit = (Trajectory_Unit_Interface*)(*itr);

			//	paths << trajectory_unit->link<Link_Interface*>()->template uuid<int>() << ":" << trajectory_unit->template estimated_link_accepting_time<int>() << ", ";
			//}

			//paths << endl;

			//paths.close();


			//typename TargetType::reverse_iterator itr;
			//typename std::vector<float>::reverse_iterator arrival_time_itr;
			//for(itr = path_container.rbegin(), arrival_time_itr = reversed_arrival_time_container.rbegin(); itr != path_container.rend(); itr++,arrival_time_itr++)
			//{
			//	_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
			//	vehicle_trajectory_data->template Initialize<typename TargetType::Component_Type::value_type*>((typename TargetType::Component_Type::value_type*)*itr);
			//	//vehicle_trajectory_data->Initialize<_Link_Interface*>((_Link_Interface*)*itr);
			//	if (arrival_time_itr != reversed_arrival_time_container.rbegin())
			//		vehicle_trajectory_data->template estimated_link_accepting_time<int>(*(arrival_time_itr /*- 1*/));
			//	else
			//		vehicle_trajectory_data->template estimated_link_accepting_time<int>(0.0f);
			//	trajectory.push_back(vehicle_trajectory_data);
			//}
		}
		//------ movement plan logging - handle at assignment intervals

		template<typename MasterType, typename InheritanceList>
		void Movement_Plan_Implementation<MasterType, InheritanceList>::clear_trajectory()
		{	
			typedef  Random_Access_Sequence< typename get_type_of(trajectory_container)> Trajectory_Container_Interface;
			typedef  Trajectory_Unit<get_component_type(Trajectory_Container_Interface)>  Trajectory_Unit_Interface;

			Trajectory_Container_Interface& trajectory = trajectory_container<Trajectory_Container_Interface&>();

			// Free the allocated memory in the trajectory, if exists
			for (auto itr = trajectory.begin(); itr != trajectory.end(); ++itr)
			{
				Free<get_component_type(Trajectory_Container_Interface)>(*itr);
			}
			trajectory.clear();

			typedef typename Trajectory_Container_Interface::Component_Type trajectory_container_type;
			trajectory_container_type().swap((trajectory_container_type&)trajectory);
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType> 
		void Movement_Plan_Implementation<MasterType, InheritanceList>::update_route_length()
		{
			typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

			typedef  Link_Components::Prototypes::Link< typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;

			_Trajectory_Container_Interface& trajectory = trajectory_container<_Trajectory_Container_Interface&>();
			route_length<float>(0.0);
			typename _Trajectory_Container_Interface::iterator itr;
			for (itr = trajectory.begin(); itr != trajectory.end() - 1; itr++)
			{
				_Trajectory_Unit_Interface* vehicle_trajectory_data = (_Trajectory_Unit_Interface*)(*itr);
				route_length<float&>() += vehicle_trajectory_data->template link<_Link_Interface*>()->template length<float>() / 5280.0;
			}
		}
		
		
		
		//=====================================================================
		// INTEGRATED MOVEMENT PLAN
		//---------------------------------------------------------------------
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


		//=====================================================================
		// RECORDING VERSION OF THE MOVEMENT PLAN
		//---------------------------------------------------------------------
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


		//TODO: Omer Multimodal Everything-------------------------------------------------------------------------------------------
		implementation struct Multimodal_Trajectory_Unit_Implementation :public Polaris_Component<MasterType, INHERIT(Multimodal_Trajectory_Unit_Implementation), Data_Object>
		{
			m_data(int, delayed_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, enter_interval_index, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_link_accepting_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, intersection_delay_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_prototype(Link_Components::Prototypes::Link, typename MasterType::link_type, link, NONE, NONE);

			template<typename TargetType> void Initialize(TargetType link_val);

			m_data(float, estimated_gen_cost, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(Link_Components::Types::Link_Type_Keys, link_mode, NONE, NONE);
			m_prototype(Transit_Vehicle_Trip_Components::Prototypes::Transit_Vehicle_Trip, typename MasterType::transit_vehicle_trip_type, transit_vehicle_trip, NONE, NONE);
			m_data(int, transit_vehicle_stop_sequence, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_arrival_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_wait_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_walk_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_ivt_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_car_time, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, estimated_wait_count, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, estimated_transfer_penalty, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Multimodal_Trajectory_Unit_Implementation<MasterType, InheritanceList>::Initialize(TargetType link_val)
		{
			_link = (Link_Components::Prototypes::Link<typename MasterType::link_type>*)(type_of(link)*)link_val;
			_enter_time = 0.0;
			_delayed_time = 0.0;
			_intersection_delay_time = 0.0;
		}

		implementation struct Multimodal_Movement_Plan_Implementation : public Movement_Plan_Implementation<MasterType, INHERIT(Multimodal_Movement_Plan_Implementation)>
		{
			
			typedef Implementations::Multimodal_Trajectory_Unit_Implementation<MasterType> multimodal_trajectory_unit_type;
			m_container(std::vector<multimodal_trajectory_unit_type*>, multimodal_trajectory_container, NONE, NONE);
			//int _current_trajectory_index;

			//==================================================================================================================
			/// current_trajectory_position
			//------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> TargetType current_trajectory_position(requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)));
			template<typename TargetType> TargetType current_trajectory_position(requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)));
			template<typename TargetType> void current_trajectory_position(TargetType val, requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)));
			template<typename TargetType> void current_trajectory_position(TargetType val, requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)));
			tag_getter_setter_as_available(current_trajectory_position);
						
			template<typename T> void set_multimodal_trajectory(
				std::deque< global_edge_id >& out_path,
				std::deque< float >& out_cost,
				std::deque<Link_Components::Types::Link_Type_Keys>& out_type,
				T& out_trip,
				std::deque<int>& out_seq,
				std::deque<float>& out_time,
				std::deque<float>& out_arr_time,
				std::deque<float>& out_wait_time,
				std::deque<float>& out_walk_time,
				std::deque<float>& out_ivt_time,
				std::deque<float>& out_car_time,
				std::deque<int>& out_wait_count,
				std::deque<float>& out_transfer_pen);

			void clear_trajectory();
			template<typename TargetType> void update_route_length();
		};

		//TODO: Omer
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)))
		{
			return (TargetType)_current_trajectory_index;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)))
		{
			TargetType pos = nullptr;
			if (_multimodal_trajectory_container.size() == 0) return nullptr;
			if (_current_trajectory_index >= _multimodal_trajectory_container.size())
			{
				cout << "Error: current_trajectory_index greater than trajectory size." << endl;
				cout << "index=" << _current_trajectory_index << ", size=" << _multimodal_trajectory_container.size() << endl;
				return nullptr;
			}

			try
			{
				pos = (TargetType)_multimodal_trajectory_container[_current_trajectory_index];
			}
			catch (std::exception& e)
			{
				cout << "Trajectory container pointer=" << &_multimodal_trajectory_container << ", size=" << _multimodal_trajectory_container.size() << ", Index=" << _current_trajectory_index << ", Exception: " << e.what() << endl;
			}

			return pos;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(TargetType val, requires(TargetType, check_2(TargetType, int, is_same) || check_2(TargetType, int&, is_same)))
		{
			_current_trajectory_index = val;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::current_trajectory_position(TargetType val, requires(TargetType, !check_2(TargetType, int, is_same) && !check_2(TargetType, int&, is_same)))
		{
			_current_trajectory_index = val;
		}

		template<typename MasterType, typename InheritanceList>
		template<typename T>
		void Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::set_multimodal_trajectory(
			std::deque< global_edge_id >& out_path,
			std::deque< float >& out_cost,
			std::deque<Link_Components::Types::Link_Type_Keys>& out_type,
			T& out_trip,
			std::deque<int>& out_seq,
			std::deque<float>& out_time,
			std::deque<float>& out_arr_time,
			std::deque<float>& out_wait_time,
			std::deque<float>& out_walk_time,
			std::deque<float>& out_ivt_time,
			std::deque<float>& out_car_time,
			std::deque<int>& out_wait_count,
			std::deque<float>& out_transfer_pen)
		{
			//TODO: check that this has been correctly translated!

			typedef  Multimodal_Trajectory_Unit<typename remove_pointer< typename get_type_of(multimodal_trajectory_container)::value_type>::type>  Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename get_type_of(multimodal_trajectory_container), Multimodal_Trajectory_Unit_Interface*> Multimodal_Trajectory_Container_Interface;

			typedef Link_Components::Prototypes::Link< typename Multimodal_Trajectory_Unit_Interface::get_type_of(link)> Link_Interface;

			typedef Network_Components::Prototypes::Network< typename get_type_of(network) > Network_Interface;

			typedef typename Multimodal_Trajectory_Unit_Interface::get_type_of(transit_vehicle_trip) Transit_Vehicle_Trip_Interface;

			Network_Interface* net = network<Network_Interface*>();


			Multimodal_Trajectory_Container_Interface& trajectory = multimodal_trajectory_container<Multimodal_Trajectory_Container_Interface&>();
			//trajectory.clear();
			clear_trajectory();

			typename std::deque<global_edge_id>::iterator itr;
			typename std::deque<float>::iterator arrival_time_itr;

			int other_itr = 0;
			for (itr = out_path.begin(); itr != out_path.end(); itr++, other_itr++)
			{
				Multimodal_Trajectory_Unit_Interface* vehicle_trajectory_data = (Multimodal_Trajectory_Unit_Interface*)Allocate<typename Multimodal_Trajectory_Unit_Interface::Component_Type>();

				Link_Interface* link = net->template get_link_ptr< typename Multimodal_Trajectory_Unit_Interface::get_type_of(link) >(itr->edge_id);
				vehicle_trajectory_data->template Initialize<Link_Interface*>(link);

				Link_Components::Types::Link_Type_Keys mode = out_type[other_itr];
				vehicle_trajectory_data->template link_mode<Link_Components::Types::Link_Type_Keys>(mode);

				Transit_Vehicle_Trip_Interface* trip = out_trip[other_itr];
				vehicle_trajectory_data->template transit_vehicle_trip<Transit_Vehicle_Trip_Interface*>(trip);

				vehicle_trajectory_data->template estimated_wait_count<int>(out_wait_count[other_itr]);
				vehicle_trajectory_data->template estimated_wait_time<float>(out_wait_time[other_itr]);
				vehicle_trajectory_data->template estimated_transfer_penalty<float>(out_transfer_pen[other_itr]);

				if (mode == Link_Components::Types::Link_Type_Keys::TRANSIT)
				{
					vehicle_trajectory_data->template transit_vehicle_stop_sequence<int>(out_seq[other_itr] - 1);
					if (vehicle_trajectory_data->template transit_vehicle_stop_sequence<int>() < 0)
					{
						assert(false);
						cout << "Sequence cannot be less than 0 for a transit trip!" << endl;
					}

				}
				else
				{
					vehicle_trajectory_data->template transit_vehicle_stop_sequence<int>(out_seq[other_itr]);
					if (vehicle_trajectory_data->template transit_vehicle_stop_sequence<int>() != -1)
					{
						assert(false);
						cout << "Sequence must be -1 for active modes!" << endl;
					}
				}

				if (other_itr != 0)
				{
					vehicle_trajectory_data->template estimated_gen_cost<float>(out_cost[other_itr - 1]);
					vehicle_trajectory_data->template estimated_link_accepting_time<float>(out_time[other_itr - 1]);
					vehicle_trajectory_data->template estimated_arrival_time<float>(out_arr_time[other_itr - 1]);
					vehicle_trajectory_data->template estimated_walk_time<float>(out_walk_time[other_itr - 1]);
					vehicle_trajectory_data->template estimated_ivt_time<float>(out_ivt_time[other_itr - 1]);
					vehicle_trajectory_data->template estimated_car_time<float>(out_car_time[other_itr - 1]);
				}
				else
				{
					vehicle_trajectory_data->template estimated_gen_cost<float>(0.0f);
					vehicle_trajectory_data->template estimated_link_accepting_time<float>(0.0f);
					vehicle_trajectory_data->template estimated_arrival_time<float>(departed_time<Time_Seconds>());
					vehicle_trajectory_data->template estimated_walk_time<float>(0.0f);
					vehicle_trajectory_data->template estimated_ivt_time<float>(0.0f);
					vehicle_trajectory_data->template estimated_car_time<float>(0.0f);
				}

				trajectory.push_back(vehicle_trajectory_data);
			}

			number_of_switches<int>(0.0);
			update_route_length<NT>();
		}

		template<typename MasterType, typename InheritanceList>
		void Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::clear_trajectory()
		{
			typedef  Random_Access_Sequence< typename get_type_of(multimodal_trajectory_container)> Multimodal_Trajectory_Container_Interface;
			typedef  Multimodal_Trajectory_Unit<get_component_type(Multimodal_Trajectory_Container_Interface)>  Multimodal_Trajectory_Unit_Interface;

			Multimodal_Trajectory_Container_Interface& trajectory = multimodal_trajectory_container<Multimodal_Trajectory_Container_Interface&>();

			// Free the allocated memory in the trajectory, if exists
			for (auto itr = trajectory.begin(); itr != trajectory.end(); ++itr)
			{
				Free<get_component_type(Multimodal_Trajectory_Container_Interface)>(*itr);
			}
			trajectory.clear();

			typedef typename Multimodal_Trajectory_Container_Interface::Component_Type multimodal_trajectory_container_type;
			multimodal_trajectory_container_type().swap((multimodal_trajectory_container_type&)trajectory);
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType> 
		void Multimodal_Movement_Plan_Implementation<MasterType, InheritanceList>::update_route_length()
		{
			typedef  Multimodal_Trajectory_Unit<typename remove_pointer< typename get_type_of(multimodal_trajectory_container)::value_type>::type>  _Multimodal_Trajectory_Unit_Interface;
			typedef  Random_Access_Sequence< typename get_type_of(multimodal_trajectory_container), _Multimodal_Trajectory_Unit_Interface*> _Multimodal_Trajectory_Container_Interface;

			typedef  Link_Components::Prototypes::Link< typename _Multimodal_Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;

			_Multimodal_Trajectory_Container_Interface& trajectory = multimodal_trajectory_container<_Multimodal_Trajectory_Container_Interface&>();
			route_length<float>(0.0);
			typename _Multimodal_Trajectory_Container_Interface::iterator itr;
			for (itr = trajectory.begin(); itr != trajectory.end() - 1; itr++)
			{
				_Multimodal_Trajectory_Unit_Interface* vehicle_trajectory_data = (_Multimodal_Trajectory_Unit_Interface*)(*itr);
				route_length<float&>() += vehicle_trajectory_data->template link<_Link_Interface*>()->template length<float>() / 5280.0;
			}
		}

		//TODO: Omer Multimodal everything end---------------------------------------------------------------------------------------

	}
}
