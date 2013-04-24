#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Mover_Implementation : public Polaris_Component<APPEND_CHILD(Person_Mover_Implementation),MasterType,Execution_Object,ParentType>
		{
			member_prototype(Prototypes::Person,Parent_Person,typename MasterType::person_type,none,none);
			member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,Movement,typename MasterType::movement_plan_type,none,none);
			member_data(bool, Movement_Scheduled, check(ReturnValueType,is_integral), check(SetValueType,is_integral));
		};

		implementation struct Person_Implementation : public Polaris_Component<APPEND_CHILD(Person_Implementation),MasterType,Execution_Object,ParentType>
		{
			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			member_prototype(Vehicle_Components::Prototypes::Vehicle_Prototype, vehicle,typename MasterType::vehicle_type,none,none);
			member_prototype(Routing_Components::Prototypes::Routing_Prototype, router,typename MasterType::routing_type,none,none);
			member_prototype(Prototypes::Person_Mover, Moving_Faculty,Implementations::Person_Mover_Implementation<MasterType>, none, none);
		
			member_prototype(Prototypes::Person_Planner, Planning_Faculty, typename MasterType::person_planner_type,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(Prototypes::Person_Properties, Properties, typename MasterType::person_properties_type,none,check_2(ComponentType,CallerType, Is_Same_Entity));
			member_prototype(Prototypes::Person_Properties, Static_Properties,typename MasterType::person_static_properties_type,none,none);

			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, scenario_reference, typename MasterType::scenario_type, none, none);
			member_prototype(Network_Components::Prototypes::Network_Prototype, network_reference, typename MasterType::network_type, none, none);

			// Agent ID
			member_data(long,uuid,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));

			// First iteration  - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(First_Iteration, Value, Basic_Units::Prototypes::Time_Prototype, Basic_Units::Implementations::Time_Implementation<NT>);

			// Record of completed activities (stores a simplified subset of activity data)
			member_container(vector<typename MasterType::activity_record_type*>, activity_record_container, none, none);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef Prototypes::Person<ComponentType> this_itf;
			define_component_interface(generator_itf,typename type_of(Planning_Faculty)::type_of(Activity_Generator),Prototypes::Activity_Generator,ComponentType);
			define_component_interface(destination_choice_itf,typename type_of(Planning_Faculty)::type_of(Destination_Chooser),Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(timing_choice_itf,typename type_of(Planning_Faculty)::type_of(Timing_Chooser),Prototypes::Activity_Timing_Chooser,ComponentType);
			define_container_and_value_interface(zones_container_interface, zone_interface, typename network_reference_interface::get_type_of(zones_container),Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype,ComponentType);
			define_container_and_value_interface(locations_container_interface, location_interface, typename network_reference_interface::get_type_of(activity_locations_container),Containers::Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype,ComponentType);

			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			feature_implementation void Initialize(TargetType id)
			{	
				// Set the initial iteration to process
				this->template First_Iteration<ComponentType,CallerType,Time_Minutes>(1.0);

				// Create and Initialize the Properties faculty
				_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
				_Properties->template Initialize<void>();
				_Properties->template Parent_Person<ComponentType*>(this);

				// Create and Initialize the Planner faculty and its subcomponents
				_Planning_Faculty = (Planning_Faculty_interface*)Allocate<type_of(Planning_Faculty)>();	
				_Planning_Faculty->template Parent_Person<ComponentType*>(this);
				_Planning_Faculty->template Initialize<NULLTYPE>();
				generator_itf* generator = (generator_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Activity_Generator)>();
				generator->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Activity_Generator<generator_itf*>(generator);
				destination_choice_itf* destination_chooser = (destination_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Destination_Chooser)>();
				destination_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Destination_Chooser<destination_choice_itf*>(destination_chooser);
				timing_choice_itf* timing_chooser = (timing_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Timing_Chooser)>();
				timing_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
				_Planning_Faculty->template Timing_Chooser<timing_choice_itf*>(timing_chooser);

				// Create and Initialize the routing faculty
				_router=(router_interface*)Allocate<type_of(router)>();
				_router->template traveler<ComponentType*>(this);
				_router->template network<network_reference_interface*>(_network_reference);	

				// Moving faculty
				_Moving_Faculty = (Moving_Faculty_interface*)Allocate<Implementations::Person_Mover_Implementation<MasterType>>();
				_Moving_Faculty->Parent_Person<ComponentType*>(this);

				// Create and Initialize the vehicle		
				_vehicle = (vehicle_interface*)Allocate<type_of(vehicle)>(); 	
				_vehicle->template uuid<int>(id);
				_vehicle->template internal_id<int>(id);
				_vehicle->template traveler<ComponentType*>(this);

				// Add basic traveler properties							
				this->template uuid<ComponentType,ComponentType,int>(id);
			}
			feature_implementation void Initialize(typename TargetType::ParamType id, typename TargetType::Param2Type home_zone)
			{
				this->Initialize<ComponentType,CallerType, TargetType::ParamType>(id);
				_Properties->template Initialize<Target_Type<NT,void,TargetType::Param2Type> >(home_zone);
			}
			tag_feature_as_available(Initialize);	

			feature_implementation void Choose_Work_Location()
			{
				// interface to this
				this_itf* pthis = (this_itf*)this;


				// first, make sure person is worker, if not exit
				EMPLOYMENT_STATUS status = _Static_Properties->Employment_Status<EMPLOYMENT_STATUS>();
				if (status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK && status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					pthis->Work_Location<int>(-1);
					return;
				}


				// Get the expected travel time from the persons static properties
				Time_Minutes ttime = _Static_Properties->Journey_To_Work_Travel_Time<Time_Minutes>();

				// if minimimal travel time, assign the home location as the work location
				if (ttime < 2)
				{
					pthis->Work_Location<int>(pthis->Home_Location<int>());
					return;
				}

				//=========================================================
				// Find available zones within the specified target range of the given work travel time
				//---------------------------------------------------------
				zones_container_interface* zones = _network_reference->zones_container<zones_container_interface*>();
				zones_container_interface::iterator z_itr;
				vector<zone_interface*> temp_zones;
				vector<float> temp_zone_probabilities;
				zone_interface* orig = pthis->Home_Location<zone_interface*>();

				// loop through all zones, store those within +- 2 min of estimated work travel time that have available work locations
				float time_range_to_search = 2.0;
				int employment = 0;
				while (temp_zones.size() == 0)
				{
					for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
					{
						zone_interface* zone = z_itr->second;
						Time_Minutes t = _network_reference->Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys> >(orig->uuid<int>(),zone->uuid<int>(), Vehicle_Components::Types::SOV);
						if (t > ttime - time_range_to_search && t < ttime + time_range_to_search && zone->work_locations<locations_container_interface*>()->size() > 0 && zone->employment<int>() > 0)
						{
							employment += zone->employment<int>();
							temp_zones.push_back(zone);
						}
					}
					// expand the search time radius if no available zones found
					if (time_range_to_search >= 20) break;
					time_range_to_search += time_range_to_search;
					
				}
				// calculate probabilities
				float cum_prob = 0;
				for (vector<zone_interface*>::iterator t_itr = temp_zones.begin(); t_itr != temp_zones.end(); ++t_itr)
				{
					cum_prob += (*t_itr)->employment<float>() / employment;
					temp_zone_probabilities.push_back(cum_prob);
				}

				//=========================================================
				// select zone
				//---------------------------------------------------------
				float r = Uniform_RNG.Next_Rand<float>();
				zone_interface* selected_zone = nullptr;
				vector<zone_interface*>::iterator t_itr;
				vector<float>::iterator p_itr;
				for (t_itr = temp_zones.begin(), p_itr = temp_zone_probabilities.begin(); t_itr != temp_zones.end(); ++t_itr, ++p_itr)
				{
					if (r<*p_itr) 
					{
						selected_zone = *t_itr;
						break;
					}
				}
				
				//=========================================================
				// Select location from within the zone
				//---------------------------------------------------------
				// set work location to home if no valid locations found
				if (selected_zone == nullptr)
				{
					pthis->Work_Location<int>(pthis->Home_Location<int>());
					//THROW_WARNING("WARNING: no valid work zone found for person id: " << pthis->uuid<int>() << ", setting work location to home location.");
				}

				// select work location from within available work locations in the zone
				else
				{
					locations_container_interface* work_locations = selected_zone->work_locations<locations_container_interface*>();
					float size = work_locations->size();
					int index = (int)(Uniform_RNG.Next_Rand<float>()*size);
					location_interface* work_loc = (*work_locations)[index];
					pthis->Work_Location<int>(work_loc->internal_id<int>());
				}
			}
			tag_feature_as_available(Choose_Work_Location);

			feature_implementation void Choose_School_Location()
			{
				// interface to this
				this_itf* pthis = (this_itf*)this;
				zone_interface* selected_zone = nullptr;


				//=========================================================
				// first, make sure person is student, if not exit
				//---------------------------------------------------------
				SCHOOL_ENROLLMENT status = _Static_Properties->School_Enrollment<SCHOOL_ENROLLMENT>();
				if (status != SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC && status != SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					pthis->School_Location<int>(-1);
					return;
				}

				//=========================================================
				// Find available zones within the specified target range of the given work travel time
				//---------------------------------------------------------
				zones_container_interface* zones = _network_reference->zones_container<zones_container_interface*>();
				zones_container_interface::iterator z_itr;
				vector<zone_interface*> temp_zones;
				vector<float> temp_zone_probabilities;
				zone_interface* orig = pthis->Home_Location<zone_interface*>();

				//=========================================================
				// if origin zone has school locations, select, else search
				//---------------------------------------------------------
				if (orig->school_locations<locations_container_interface*>()->size() > 0)
				{
					selected_zone = orig;
				}
				else
				{
					// loop through all zones, store those within +- 2 min of estimated work travel time that have available work locations
					float time_range_to_search = 15.0;
					int school_locations = 0;
					while (temp_zones.size() == 0)
					{
						for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
						{
							zone_interface* zone = z_itr->second;
							Time_Minutes t = _network_reference->Get_LOS<Target_Type<NT,Time_Minutes,int,Vehicle_Components::Types::Vehicle_Type_Keys> >(orig->uuid<int>(),zone->uuid<int>(), Vehicle_Components::Types::SOV);
							if (t < time_range_to_search && zone->school_locations<locations_container_interface*>()->size() > 0)
							{
								school_locations += (int)zone->school_locations<locations_container_interface*>()->size();
								temp_zones.push_back(zone);
							}
						}
						// expand the search time radius if no available zones found
						if (time_range_to_search >= 60) break;
						time_range_to_search += time_range_to_search;
					
					}
					// calculate probabilities
					float cum_prob = 0;
					for (vector<zone_interface*>::iterator t_itr = temp_zones.begin(); t_itr != temp_zones.end(); ++t_itr)
					{
						cum_prob += (*t_itr)->school_locations<locations_container_interface*>()->size() / school_locations;
						temp_zone_probabilities.push_back(cum_prob);
					}

					//=========================================================
					// select zone
					//---------------------------------------------------------
					float r = Uniform_RNG.Next_Rand<float>();
					zone_interface* selected_zone = nullptr;
					vector<zone_interface*>::iterator t_itr;
					vector<float>::iterator p_itr;
					for (t_itr = temp_zones.begin(), p_itr = temp_zone_probabilities.begin(); t_itr != temp_zones.end(); ++t_itr, ++p_itr)
					{
						if (r<*p_itr) 
						{
							selected_zone = *t_itr;
							break;
						}
					}
				}
				
				//=========================================================
				// Select location from within the zone
				//---------------------------------------------------------
				// set school location to home if no valid locations found
				if (selected_zone == nullptr)
				{
					pthis->School_Location<int>(pthis->Home_Location<int>());
				}

				// select school location from within available school locations in the zone
				else
				{
					locations_container_interface* school_locations = selected_zone->school_locations<locations_container_interface*>();
					float size = school_locations->size();
					int index = (int)(Uniform_RNG.Next_Rand<float>()*size);
					location_interface* loc = (*school_locations)[index];
					pthis->School_Location<int>(loc->internal_id<int>());
				}
			}
			tag_feature_as_available(Choose_School_Location);

			feature_implementation void arrive_at_destination()
			{

			}
			tag_feature_as_available(arrive_at_destination);
		};

	}
}