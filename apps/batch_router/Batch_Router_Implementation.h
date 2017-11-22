#pragma once

#include "Batch_Router_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Routing_Prototype.h"


namespace Batch_Router_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Routed_Trip_Implementation : public Polaris_Component<MasterType,INHERIT(Routed_Trip_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Routed_Trip_Implementation),Execution_Object>::Component_Type ComponentType;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Trip_Conditional(ComponentType* _this, Event_Response& response)
			{
				typedef Prototypes::Routed_Trip<ComponentType> _This_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_This_Interface* this_ptr = (_This_Interface*)_pthis;

				this_ptr->Get_Results<NT>();

				response.next._iteration = END;
				response.next._sub_iteration = END;
			}

			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);

			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, origin, NONE, NONE);
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, destination, NONE, NONE);
			typedef origin_interface location_interface;

			m_data(Vehicle_Components::Types::Vehicle_Type_Keys, mode, NONE,NONE);

			member_component_and_feature_accessor(departure_time, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

			m_data(bool, link_failure,NONE,NONE);
			m_data(bool, turn_failure,NONE,NONE);

			m_data(int, ID, NONE, NONE);

			m_data(_lock, write_lock, NONE, NONE);

			m_container(concat(std::vector<pair<bool,location_interface*>>),origin_routing_results, NONE, NONE);
			m_container(concat(std::vector<pair<bool,location_interface*>>),destination_routing_results, NONE, NONE);

			typedef strip_modifiers(network_reference_type) network_itf;
			typedef strip_modifiers(router_type) router_itf;
			typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_itf)> zone_itf;
			typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
			typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
			typedef Random_Access_Sequence<typename location_itf::get_type_of(origin_links)> links_itf;
			typedef Link_Components::Prototypes::Link<get_component_type(links_itf)> link_itf;
			typedef Random_Access_Sequence<typename link_itf::get_type_of(outbound_turn_movements)> turns_itf;
			typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(turns_itf)> turn_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename router_itf::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Random_Access_Sequence<typename _Movement_Plan_Interface::get_type_of(trajectory_container)> _Trajectory_Container_Interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<get_component_type(_Trajectory_Container_Interface)> _Trajectory_Unit_Interface;

			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(string mode, int Trip_Id, NetworkType network_ref, LocationType orig, LocationType dest, TimeType depart, requires(LocationType, check(LocationType, is_pointer) && check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network_reference)> _Network_Interface;
				typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;
				typedef  Link_Components::Prototypes::Link<typename MasterType::link_type>  _Link_Interface;
				typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;
				typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::data_type>::type>  _Zone_Interface;
				typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;
				typedef  Turn_Movement_Components::Prototypes::Movement<typename remove_pointer< typename _Link_Interface::get_type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
				typedef  Random_Access_Sequence< typename _Link_Interface::get_type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;
				typedef  Routing_Components::Prototypes::Routing< get_type_of(router) > _Routing_Interface;
				typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename MasterType::movement_plan_type> _Movement_Plan_Interface;
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

				this->network_reference<_Network_Interface*>(network_ref);
				_Network_Interface* network = network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();
				_ID = Trip_Id;

				// Validate departure time
				int departed_time = GLOBALS::Time_Converter.Convert_Value<TimeType, Simulation_Timestep_Increment>(depart);
				this->departure_time<TimeType>(depart);
				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();
				if (departed_time < simulation_start_time || departed_time >= simulation_end_time)
				{					
					results_by_thread[0] << Trip_Id << ": Trip start time is out of bounds - skipped.";
					return;
				}

				// Validate that origin/destination pair is routable
				_Activity_Location_Interface* origin_activity_location = orig;
				this->origin<_Activity_Location_Interface*>(orig);
				_Activity_Location_Interface* destination_activity_location = dest;
				this->destination<_Activity_Location_Interface*>(dest);
				_Link_Interface* origin_link = origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
				_Link_Interface* destination_link = destination_activity_location->template destination_links<_Links_Container_Interface&>()[0];
				if (origin_link->template internal_id<int>() == destination_link->template internal_id<int>() || (origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
				{
					results_by_thread[0] << Trip_Id << "Origin/Destination pair is not routable - skipped.";
					return;
				}

				// Allocate linked classes
				_Routing_Interface* router = (_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
				_Movement_Plan_Interface* movement_plan = (_Movement_Plan_Interface*)Allocate<typename _Movement_Plan_Interface::Component_Type>();
				this->router<_Routing_Interface*>(router);
	
				// Assign values to sub-classes used in routing
				movement_plan->network<_Network_Interface*>(network);
				movement_plan->template origin<_Link_Interface*>(origin_link);
				movement_plan->template destination<_Link_Interface*>(destination_link);
				movement_plan->template origin<_Activity_Location_Interface*>(orig);
				movement_plan->template destination<_Activity_Location_Interface*>(dest);
				movement_plan->template departed_time<Time_Seconds>(departed_time);
				movement_plan->template initialize_trajectory<NULLTYPE>();
				if (strcmp(mode.c_str(),"rail")==0 || strcmp(mode.c_str(), "Rail")==0 || strcmp(mode.c_str(), "RAIL")==0)			movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::RAIL);
				else if (strcmp(mode.c_str(), "bus")==0 || strcmp(mode.c_str(), "Bus") == 0 || strcmp(mode.c_str(), "BUS") == 0)	movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::BUS);
				else if (strcmp(mode.c_str(), "walk")==0 || strcmp(mode.c_str(), "Walk") == 0 || strcmp(mode.c_str(), "WALK") == 0)	movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::WALK);
				else if (strcmp(mode.c_str(), "bike") == 0 || strcmp(mode.c_str(), "Bike") == 0 || strcmp(mode.c_str(), "BIKE") == 0)	movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE);
				else if (strcmp(mode.c_str(), "bicycle") == 0 || strcmp(mode.c_str(), "Bicycle") == 0 || strcmp(mode.c_str(), "BICYCLE") == 0)	movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::BICYCLE);
				else if (strcmp(mode.c_str(), "pnr") == 0 || strcmp(mode.c_str(), "Pnr") == 0 || strcmp(mode.c_str(), "PNR") == 0)	movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::PARK_AND_RIDE);
				else movement_plan->mode(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);

				router->template network<_Network_Interface*>(network);
				router->Attach_New_Movement_Plan<typename _Movement_Plan_Interface::Component_Type>(movement_plan);

				// Get a random timestep in which to perform route calculation
				Simulation_Timestep_Increment dt = GLOBALS::Uniform_RNG.Next_Rand<double>() * (END - 6 - iteration());
				Simulation_Timestep_Increment t = GLOBALS::Simulation_Time.Future_Time<Simulation_Timestep_Increment, Simulation_Timestep_Increment>(dt);

				// Schedule routing at timestep t + 1
				router->Schedule_Route_Computation(departed_time, t + 1);

				// Load the trip conditional at timestep t + 5, which fills the trip results after routing is completed
				//TODO Omer: Check t + 2 vs t+ 5 does not affect things badly.
				//Load_Event<ComponentType>(Trip_Conditional, t + 5, Types::SUB_ITERATIONS::INITIALIZE);
				Load_Event<ComponentType>(Trip_Conditional, t + 2, Types::SUB_ITERATIONS::INITIALIZE);
			}
			template<typename TargetType> void Get_Results()
			{
				_Movement_Plan_Interface* movement = this->_router->movement_plan<_Movement_Plan_Interface*>();

				if (movement->valid_trajectory<bool>())
				{
					
					summary_by_thread[__thread_id] << __thread_id << "\t" << movement->summary_string<std::string>() ;
					details_by_thread[__thread_id] << movement->detail_string<std::string>();					
					results_by_thread[__thread_id] << movement->routed_travel_time<float>() << ": ";

					_Trajectory_Container_Interface* trajectory = movement->trajectory_container<_Trajectory_Container_Interface*>();

					for (_Trajectory_Container_Interface::iterator itr = trajectory->begin(); itr != trajectory->end(); ++itr)
					{
						_Trajectory_Unit_Interface* tu = (_Trajectory_Unit_Interface*)*itr;
						link_itf* link = tu->link<link_itf*>();
						results_by_thread[__thread_id]<<_ID<<": (" << link->dbid<int>()<<"."<<link->direction<int>()<<") "<<tu->estimated_link_accepting_time<int>()<<", ";
					}
					results_by_thread[__thread_id] << endl;
				}
				else
				{
					results_by_thread[__thread_id] << _ID <<": Error, movement was not routable."<<endl;
					summary_by_thread[__thread_id] << __thread_id << "\t" << movement->summary_string<std::string>();
				}

				object_count_by_thread[__thread_id]++;

				if (object_count_by_thread[__thread_id] >= 100)
				{
					LOCK(_write_lock);
					fw_output.Write(results_by_thread[__thread_id]);
					fw_mm_sp_summary.Write_NoDelim(summary_by_thread[__thread_id]);
					fw_mm_sp_details.Write_NoDelim(details_by_thread[__thread_id]);
					object_count_by_thread[__thread_id] = 0;
					UNLOCK(_write_lock);
				}

				
			}
			
		};



	}

}
