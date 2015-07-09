#pragma once
#include "User_Space_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Traffic_Simulator_Concepts.h"

namespace Batch_Router_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			PATH_BUILDING = 21,
			UPDATE = 22,
			GET_RESULTS=23
		};
	}

	namespace Prototypes
	{

		prototype struct Routed_Trip
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Trip_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Routed_Trip<ComponentType> _This_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_This_Interface* this_ptr=(_This_Interface*)_pthis;

				this_ptr->Get_Results<NT>();

				response.next._iteration = END;
				response.next._sub_iteration = END;
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(network_reference,NONE,NONE);
			accessor(router,NONE,NONE);
			accessor(origin,NONE,NONE);
			accessor(destination,NONE,NONE);
			accessor(mode,NONE,NONE);
			accessor(departure_time,NONE,NONE);
			accessor(result_travel_time,NONE,NONE);
			accessor(result_distance,NONE,NONE);
			accessor(result_trajectory,NONE,NONE);
			accessor(link_failure,NONE,NONE);
			accessor(turn_failure,NONE,NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(int Trip_Id, NetworkType network_ref, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,check(LocationType, is_pointer) && check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
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
				_Network_Interface* network=network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();
				
				
				// Validate departure time
				int departed_time = GLOBALS::Time_Converter.Convert_Value<TimeType,Simulation_Timestep_Increment>(depart);
				this->departure_time<TimeType>(depart);
				int simulation_start_time = scenario->template simulation_start_time<int>();
				int simulation_end_time = scenario->template simulation_end_time<int>();
				if (departed_time < simulation_start_time || departed_time >= simulation_end_time)
				{
					cout<<"Trip start time is out of bounds. Trip "<<Trip_Id<<" is skipped.";
					return;
				}

				// Validate that origin/destination pair is routable
				_Activity_Location_Interface* origin_activity_location = orig;
				this->origin<_Activity_Location_Interface*>(orig);
				_Activity_Location_Interface* destination_activity_location = dest;
				this->destination<_Activity_Location_Interface*>(dest);
				_Link_Interface* origin_link = origin_activity_location->template origin_links<_Links_Container_Interface&>()[0];
				_Link_Interface* destination_link = destination_activity_location->template destination_links<_Links_Container_Interface&>()[0];
				if (origin_link->template internal_id<int>() == destination_link->template internal_id<int>()  || (origin_link->template outbound_turn_movements<_Movements_Container_Interface&>().size() == 0 || destination_link->template inbound_turn_movements<_Movements_Container_Interface&>().size() == 0))
				{
					cout<<"Origin/Destination pair is not routable. Trip "<<Trip_Id<<" is skipped.";
					return;
				}

				// Allocate linked classes
				_Routing_Interface* router=(_Routing_Interface*)Allocate<typename _Routing_Interface::Component_Type>();
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

				router->template network<_Network_Interface*>(network);
				router->Attach_New_Movement_Plan<typename _Movement_Plan_Interface::Component_Type>(movement_plan);
				
				// Get a random timestep in which to perform route calculation
				Simulation_Timestep_Increment dt = GLOBALS::Uniform_RNG.Next_Rand<double>() * (END - 6 -iteration());
				Simulation_Timestep_Increment t = GLOBALS::Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(dt);

				// Schedule routing at timestep t + 1
				router->Schedule_Route_Computation(departed_time, t+1);

				// Load the trip conditional at timestep t + 5, which fills the trip results after routing is completed
				this_component()->Load_Event<ComponentType>(Trip_Conditional,t+5,Types::SUB_ITERATIONS::INITIALIZE);
			}			
			template<typename NetworkType, typename LocationType, typename TimeType> void Initialize(int Trip_Id, NetworkType network, LocationType orig, LocationType dest, TimeType depart, requires(LocationType,!check(LocationType, is_pointer) || !check(strip_modifiers(LocationType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType is not a valid location interface");
			}			
			
			template<typename TargetType> void Get_Results()
			{
				this_component()->Get_Results<TargetType>();
			}
		};

	}
}

