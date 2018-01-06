#pragma once
#include "Traveler_Simulator_Includes.h"
//#include "activity_simulator/Activity_Prototype.h"
//#include "Activity_Location_Prototype.h"
//#include "Link_Prototype.h"
//#include "Zone_Prototype.h"
#include "routing/Routing.h"

namespace Movement_Plan_Components
{

	namespace Prototypes
	{

		prototype struct Trajectory_Unit ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			template<typename TargetType> void Initialize(TargetType link_ptr)
			{
				this_component()->template Initialize<TargetType>(link_ptr);
			}

			accessor(link, NONE, NONE);
			accessor(delayed_time, NONE, NONE);
			accessor(enter_time, NONE, NONE);
			accessor(enter_interval_index, NONE, NONE);
			accessor(estimated_link_accepting_time, NONE, NONE);
			accessor(intersection_delay_time, NONE, NONE);
			template<typename TargetType> TargetType exit_time()
			{
				return (TargetType)(enter_time<int>() + delayed_time<float>());
			}
		};


		prototype struct Movement_Plan ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(network, NONE, NONE);

			accessor(trajectory_container, NONE, NONE);

			accessor(current_trajectory_position, NONE, NONE);
			accessor(departed_time, NONE, NONE);
			accessor(arrived_time, NONE, NONE);
			accessor(planning_time, NONE, NONE);
			accessor(plan, NONE, NONE);
			accessor(valid_trajectory, NONE, NONE);
			accessor(destination_activity_reference, NONE, NONE);
			accessor(routed_travel_time, NONE, NONE);
			accessor(estimated_time_of_arrival, NONE, NONE);
			accessor(estimated_travel_time_when_departed, NONE, NONE);
			accessor(number_of_switches, NONE, NONE);
			accessor(absolute_departure_time, NONE, NONE);
			accessor(is_integrated, NONE, NONE);
			accessor(route_length, NONE, NONE);
			accessor(entry_time, NONE, NONE);
			accessor(traveler_id, NONE,NONE);
			accessor(mode, NONE, NONE);

			//TODO: Remove when done testing routing execution time
			accessor(routing_execution_time, NONE, NONE);
			accessor(summary_string, NONE, NONE);
			accessor(detail_string, NONE, NONE);

			void Free_Movement()
			{
				if (this == nullptr) return;
				this->clear_trajectory();
				Free<Component_Type>(this_component());
			}

			template<typename LocationType> bool Update_Locations(LocationType orig, LocationType dest, requires(LocationType,check(strip_modifiers(LocationType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				typedef Prototypes::Movement_Plan<ComponentType> this_itf;
				
				typedef Network_Components::Prototypes::Network< typename get_type_of(network)> _network_itf;
				typedef Random_Access_Sequence< typename _network_itf::get_type_of(activity_locations_container)> _activity_locations_container_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_activity_locations_container_itf)>  _activity_location_itf;
				typedef Random_Access_Sequence< typename _activity_location_itf::get_type_of(origin_links)> _links_container_itf;
				typedef Link_Components::Prototypes::Link<get_component_type(_links_container_itf)>  _link_itf;
				typedef Random_Access_Sequence< typename _link_itf::get_type_of(outbound_turn_movements)> _turns_container_itf;
				typedef Turn_Movement_Components::Prototypes::Movement<get_component_type(_turns_container_itf)>  _turn_itf;

				// continue if a valid movement is specified
				if (orig != nullptr && dest != nullptr) 
				{
					// If the trip is valid, assign to a movement plan and add to the schedule
					if (orig->template origin_links<_links_container_itf&>().size() != 0 && dest->template origin_links<_links_container_itf&>().size() != 0)
					{		
						// add attributes to plan
						this->template origin<_activity_location_itf*>(orig);
						this->template destination<_activity_location_itf*>(dest);
						this->template origin<_link_itf*>(orig->template origin_links<_links_container_itf&>().at(0));
						this->template destination<_link_itf*>(dest->template origin_links<_links_container_itf&>().at(0));

						if (this->template origin<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0 || this->template destination<_link_itf*>()->template outbound_turn_movements<_turns_container_itf*>()->size() == 0)
						{
							_link_itf* o_link =this->template origin<_link_itf*>();
							_link_itf* d_link =this->template destination<_link_itf*>();
							THROW_EXCEPTION("ERROR: cannot route trip as orig or dest links do not have valid turn movements: orig_link="<<o_link->template dbid<int>() << ", dir="<<o_link->template direction<int>()<<" : dest_link="<< d_link->template dbid<int>()<<", dir="<<o_link->template direction<int>());
							return false;
						}
					}
					else
					{
						this->template origin<_activity_location_itf*>(orig);
						this->template destination<_activity_location_itf*>(dest);
						THROW_WARNING("WARNING: movement from " << orig->template uuid<int>() << " to " << dest->template uuid<int>() << ", can not happen as no origin / destination links are available for the locations.");
						return false;
					}
				}
				else
				{
					THROW_WARNING("Null origin or destination values specified");
					return false;
				}
				return true;
			}

			// overloaded origin and destination, depending on targetType
			template<typename TargetType>
			void origin(TargetType activity_location,requires(TargetType,check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				this_component()->template origin_location<TargetType>(activity_location);
			}
			template<typename TargetType> TargetType origin(requires(TargetType,check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				return this_component()->template origin_location<TargetType>();
			}
			template<typename TargetType> void destination(TargetType activity_location, requires(TargetType,check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				this_component()->template destination_location<TargetType>(activity_location);
			}
			template<typename TargetType> TargetType destination(requires(TargetType,check(strip_modifiers(TargetType),Activity_Location_Components::Concepts::Is_Activity_Location)))
			{
				return this_component()->template destination_location<TargetType>();
			}
			template<typename TargetType>
			void origin(TargetType link, requires(TargetType,check(strip_modifiers(TargetType),Link_Components::Concepts::Is_Basic_Link)))
			{
				this_component()->template origin<TargetType>(link);
			}
			template<typename TargetType> TargetType origin(requires(TargetType,check(strip_modifiers(TargetType),Link_Components::Concepts::Is_Basic_Link)))
			{
				return this_component()->template origin<TargetType>();
			}
			template<typename TargetType> void destination(TargetType link, requires(TargetType,check(strip_modifiers(TargetType),Link_Components::Concepts::Is_Basic_Link)))
			{
				this_component()->template destination<TargetType>(link);
			}
			template<typename TargetType> TargetType destination(requires(TargetType,check(strip_modifiers(TargetType),Link_Components::Concepts::Is_Basic_Link)))
			{
				return this_component()->template destination<TargetType>();
			}
			/*template<typename TargetType> void origin(TargetType zone, requires(TargetType,check(strip_modifiers(TargetType),Zone_Components::Concepts::Is_Zone)))
			{
				this_component()->template origin_zone<TargetType>(zone);
			}*/
			template<typename TargetType> TargetType origin(requires(TargetType,check(strip_modifiers(TargetType),Zone_Components::Concepts::Is_Zone)))
			{
				typedef  Activity_Location_Components::Prototypes::Activity_Location< typename get_type_of(origin_location)> _Location_Interface;
				_Location_Interface* origin_loc = this->origin<_Location_Interface*>();

				return origin_loc->template zone<TargetType>();
			}
			/*template<typename TargetType> void destination(TargetType zone, requires(TargetType,check(strip_modifiers(TargetType),Zone_Components::Concepts::Is_Zone)))
			{
				this_component()->template destination_zone<TargetType>(zone);
			}*/
			template<typename TargetType> TargetType destination(requires(TargetType,check(strip_modifiers(TargetType),Zone_Components::Concepts::Is_Zone)))
			{
				typedef  Activity_Location_Components::Prototypes::Activity_Location< typename get_type_of(origin_location)> _Location_Interface;
				_Location_Interface* dest_loc = this->destination<_Location_Interface*>();

				return dest_loc->template zone<TargetType>();
			}

#ifndef EXCLUDE_DEMAND 
			define_feature_exists_check(Initialize, Initialize_exists);
			template<typename TargetType> void Initialize(requires(TargetType,check(ComponentType,Initialize_exists)))
			{
				this_component()->template Initialize<TargetType>();
			}
			template<typename TargetType> void Initialize(requires(TargetType,!check(ComponentType,Initialize_exists)))
			{
				assert_check(ComponentType,Initialize_exists, "No Initialize method defined in component.");
			}
			template<typename TargetType> void Initialize(TargetType movement_to_copy, requires(TargetType,check(ComponentType,Initialize_exists)))
			{
				this_component()->template Initialize<TargetType>(movement_to_copy);
			}
			template<typename TargetType> void Initialize(TargetType movement_to_copy, requires(TargetType,!check(ComponentType,Initialize_exists)))
			{
				assert_check(ComponentType,Initialize_exists, "No Initialize method defined in component.");
			}
#endif
			void set_trajectory(std::deque<global_edge_id>& path_container, std::deque<float>& arrival_time_container)
			{
				//TODO: check that this has been correctly translated!

				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), Trajectory_Unit_Interface*> Trajectory_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename Trajectory_Unit_Interface::get_type_of(link)> Link_Interface;

				typedef  Network_Components::Prototypes::Network< typename get_type_of(network) > Network_Interface;

				
				Network_Interface* net = network<Network_Interface*>();

				

				Trajectory_Container_Interface& trajectory=trajectory_container<Trajectory_Container_Interface&>();
				//trajectory.clear();
				clear_trajectory();

				typename std::deque<global_edge_id>::iterator itr;
				typename std::deque<float>::iterator arrival_time_itr;

				for(itr = path_container.begin(), arrival_time_itr = arrival_time_container.begin(); itr != path_container.end(); itr++,arrival_time_itr++)
				{
					Trajectory_Unit_Interface* vehicle_trajectory_data=(Trajectory_Unit_Interface*)Allocate<typename Trajectory_Unit_Interface::Component_Type>();

					Link_Interface* link = net->template get_link_ptr< typename Trajectory_Unit_Interface::get_type_of(link) >( itr->edge_id );
					

					vehicle_trajectory_data->template Initialize<Link_Interface*>( link );

					if (arrival_time_itr != arrival_time_container.begin()) vehicle_trajectory_data->template estimated_link_accepting_time<int>( *(arrival_time_itr /*- 1*/) );
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

			void update_trajectory(std::deque<global_edge_id>& path_container, std::deque<float>& arrival_time_container)
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
				typedef  Network_Components::Prototypes::Network< typename get_type_of(network) > Network_Interface;

				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();
				
				//erase 
				//trajectory.erase(trajectory.begin() + current_trajectory_position<int&>() + 1,trajectory.end());
				clear_trajectory(current_trajectory_position<int&>() + 1);
				
				// add the time entering the current link to the relative estimated arrival time for the new trajectory_unit links
				int stored_ttime = trajectory[current_trajectory_position<int&>()]->template enter_time<int>() - (int)this->departed_time<Time_Seconds>();

				typename std::deque<global_edge_id>::iterator path_itr;
				typename std::deque<float>::iterator arrival_time_itr;
				
				Network_Interface* net = network<Network_Interface*>();

				for(path_itr = path_container.begin() + 1, arrival_time_itr = arrival_time_container.begin() + 1; path_itr != path_container.end(); path_itr++,arrival_time_itr++)
				{
					_Link_Interface* link = net->template get_link_ptr< typename _Trajectory_Unit_Interface::get_type_of(link) >( path_itr->edge_id );

					_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
					vehicle_trajectory_data->template estimated_link_accepting_time<int>(*(arrival_time_itr/* - 1*/) + stored_ttime);
					vehicle_trajectory_data->template Initialize<_Link_Interface*>( link );
					trajectory.push_back(vehicle_trajectory_data);
				}


				//typename TargetType::reverse_iterator itr;
				//typename std::vector<float>::reverse_iterator arrival_time_itr;
				//for(itr = path_container.rbegin() + 1, arrival_time_itr = reversed_arrival_time_container.rbegin() + 1; itr != path_container.rend(); itr++,arrival_time_itr++)
				//{
				//	_Trajectory_Unit_Interface* vehicle_trajectory_data=(_Trajectory_Unit_Interface*)Allocate<typename _Trajectory_Unit_Interface::Component_Type>();
				//	vehicle_trajectory_data->template estimated_link_accepting_time<int>(*(arrival_time_itr/* - 1*/) + stored_ttime);
				//	vehicle_trajectory_data->template Initialize<typename TargetType::Component_Type::value_type*>((typename TargetType::Component_Type::value_type*)*itr);
				//	trajectory.push_back(vehicle_trajectory_data);
				//}
				number_of_switches<int&>()++;
				update_route_length<NT>();
			}

			void clear_trajectory()
			{
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container)> Trajectory_Container_Interface;
				typedef  Trajectory_Unit<get_component_type(Trajectory_Container_Interface)>  Trajectory_Unit_Interface;

				Trajectory_Container_Interface& trajectory=trajectory_container<Trajectory_Container_Interface&>();

				// Free the allocated memory in the trajectory, if exists
				for (auto itr = trajectory.begin(); itr != trajectory.end(); ++itr)
				{
					Free<get_component_type(Trajectory_Container_Interface)>(*itr);
				}
				trajectory.clear();

				typedef typename Trajectory_Container_Interface::Component_Type trajectory_container_type;
				trajectory_container_type().swap((trajectory_container_type&)trajectory);
			}
			void clear_trajectory(int offset)
			{
				// get interface to trajectory
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container)> Trajectory_Container_Interface;
				typedef  Trajectory_Unit<get_component_type(Trajectory_Container_Interface)>  Trajectory_Unit_Interface;
				Trajectory_Container_Interface& trajectory=trajectory_container<Trajectory_Container_Interface&>();

				// validate offset
				if (offset >= trajectory.size()) THROW_EXCEPTION("Error, invalid begining offset for clearing the trajectory, must be less than the container size.");

				// Free the allocated memory in the trajectory, if exists
				for (auto itr = trajectory.begin()+offset; itr != trajectory.end(); ++itr)
				{
					Free<get_component_type(Trajectory_Container_Interface)>(*itr);
				}

				//erase 
				trajectory.erase(trajectory.begin()+offset,trajectory.end());
			}

			template<typename TargetType> void update_route_length()
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				typedef  Link_Components::Prototypes::Link< typename _Trajectory_Unit_Interface::get_type_of(link)> _Link_Interface;
				
				_Trajectory_Container_Interface& trajectory=trajectory_container<_Trajectory_Container_Interface&>();
				route_length<float>(0.0);
				typename _Trajectory_Container_Interface::iterator itr;
				for (itr = trajectory.begin(); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* vehicle_trajectory_data = (_Trajectory_Unit_Interface*)(*itr);
					route_length<float&>() += vehicle_trajectory_data->template link<_Link_Interface*>()->template length<float>() / 5280.0;
				}
			}

			template<typename TargetType> void advance_trajectory()
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;


				current_trajectory_position<int&>()++;
			}

			template<typename TargetType> void initialize_trajectory()
			{
				current_trajectory_position<int>(-1);
				valid_trajectory<bool>(false);
			}
			
			template<typename TargetType> void load()
			{
				this_component()->template load<TargetType>();
			}

			template<typename TargetType> TargetType next_link()
			{

				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				if((size_t)current_trajectory_position<int>() < trajectory_container<_Trajectory_Container_Interface&>().size() - 1)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)trajectory_container<_Trajectory_Container_Interface&>()[current_trajectory_position<int>() + 1];
					return trajectory_unit->template link<TargetType>();
				}
				else 
				{
					return nullptr;
				}
			}

			template<typename TargetType> TargetType current_link()
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;


				if(current_trajectory_position<int>() != -1)
				{
					_Trajectory_Unit_Interface* current_pos = current_trajectory_position<_Trajectory_Unit_Interface*>();

					if (current_pos == nullptr) return nullptr;
					
					TargetType link = nullptr;
					try
					{
						link = current_pos->template link<TargetType>();
					}
					catch (std::exception& e)
					{
						cout << "current_pos pointer="<<current_pos<<", Exception: "<< e.what() << endl;
					}
					return link;
				}
				else
				{
					return nullptr;
				}
			}
			
			template<typename TargetType> TargetType trajectory_size()
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				_Trajectory_Container_Interface* trajectory = this->trajectory_container<_Trajectory_Container_Interface*>();
				return (TargetType)(trajectory->size());
			}

			void arrive_to_destination()
			{
				this_component()->arrive_to_destination();
			}

			template<typename TargetType> void transfer_to_next_link(int delayed_time)
			{
				this_component()->template transfer_to_next_link<TargetType>(delayed_time);
			}

			template<typename TargetType> TargetType get_current_link_enter_time()
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				return (TargetType)current_trajectory_position<_Trajectory_Unit_Interface*>()->template enter_time<TargetType>();
			}

			template<typename TargetType> void update_current_link_intersection_delay(int intersection_delay)
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				current_trajectory_position<_Trajectory_Unit_Interface*>()->template intersection_delay_time<int&>() += intersection_delay;
			}

			template<typename TargetType> void set_current_link_intersection_delay(int intersection_delay)
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				current_trajectory_position<_Trajectory_Unit_Interface*>()->template intersection_delay_time<int>(intersection_delay);
			}
			template<typename TargetType> int get_route_link_exit_time(int trajectory_unit_index)
			{
				typedef  Trajectory_Unit<typename remove_pointer< typename get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
				typedef  Random_Access_Sequence< typename get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

				int route_link_exit_time = 0;
				if (trajectory_unit_index < trajectory_container<_Trajectory_Container_Interface&>().size()-1)
				{
					route_link_exit_time = trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index+1]->template enter_time<int>();
				}
				else
				{
					route_link_exit_time = trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index]->template enter_time<int>() + trajectory_container<_Trajectory_Container_Interface&>()[trajectory_unit_index]->template delayed_time<int>();
				}
				return route_link_exit_time;
			}

			void Display_Movement()
			{
				this_component()->Display_Movement();
			}			

		};
	}
}

using namespace Movement_Plan_Components::Prototypes;
