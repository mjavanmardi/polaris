#pragma once

#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{

		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Scheduler_Implementation : public Polaris_Component<MasterType,INHERIT(General_Person_Scheduler_Implementation),Execution_Object,true>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(General_Person_Scheduler_Implementation),Execution_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Person_Components::Prototypes::Person< typename MasterType::person_type>, Parent_Person, NONE, NONE);
			m_data(int, Activity_Count, NONE, NONE);

			//Containers for activity planning events and movement planning events
			m_container(boost::container::list<typename MasterType::activity_type*>,Activity_Container, NONE, NONE);
			m_container(boost::container::list<typename MasterType::movement_plan_type*>,Movement_Plans_Container, NONE, NONE);

			// Interface definitions
			typedef  Household_Components::Prototypes::Household< typename type_of(Parent_Person)::type_of(Household)> _Household_Interface;
			typedef  Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef  Network_Components::Prototypes::Network< typename type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer< typename _Network_Interface::get_type_of(activity_locations_container)::value_type>::type>  _Activity_Location_Interface;
			typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container), _Activity_Location_Interface*> _Activity_Locations_Container_Interface;

			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Activity_Location_Interface::get_type_of(origin_links)::value_type>::type>  _Link_Interface;
			typedef  Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links), _Link_Interface*> _Links_Container_Interface;

			typedef  Zone_Components::Prototypes::Zone<typename remove_pointer< typename _Network_Interface::get_type_of(zones_container)::value_type>::type>  _Zone_Interface;
			typedef  Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container), _Zone_Interface*> _Zones_Container_Interface;

			typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
			typedef Back_Insertion_Sequence< type_of(Activity_Container),Activity_Plan*> Activity_Plans;

			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer<typename  type_of(Movement_Plans_Container)::value_type>::type> Movement_Plan;
			typedef Back_Insertion_Sequence< type_of(Movement_Plans_Container),Movement_Plan*> Movement_Plans;

			typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer< typename type_of(Parent_Person)::type_of(Activity_Record_Container)::value_type>::type> Activity_Record;
			typedef Back_Insertion_Sequence< typename type_of(Parent_Person)::type_of(Activity_Record_Container),Activity_Record*> Activity_Records;

			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> At_Home_Activity_Plan;
			// member features
			template<typename TargetType> void Initialize()
			{

			} tag_feature_as_available(Initialize);

			// scheduling features - move to Person_Scheduler eventually
			template<typename TargetType> TargetType current_movement_plan(requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				Movement_Plans* movement_plans = this->template Movement_Plans_Container< Movement_Plans*>();
				typename Movement_Plans::iterator itr;
				if ((itr = movement_plans->find(iteration())) != movement_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_movement_plan);
			template<typename TargetType> TargetType current_activity_plan(requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;
				if ((itr = activity_plans->find(iteration())) != activity_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_activity_plan);
			template<typename TargetType> typename TargetType::ReturnType previous_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
//TODO
//				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
				Time_Seconds max_previous = 0;
				Activity_Plan* act;
				Activity_Plan* previous = nullptr;
				
				// search for the closest preceeding activity
				for (itr = activity_plans->begin(); itr != activity_plans->end(); ++itr)
				{
					act = (Activity_Plan*)(*itr);
					if (act->template Start_Is_Planned<bool>() && act->template Start_Time<Time_Seconds>() < start_time && act->template Start_Time<Time_Seconds>() > max_previous)
					{
						max_previous = act->template Start_Time<Time_Seconds>();
						previous = act;
					}
				}
				return (typename TargetType::ReturnType)previous;
			}
			tag_feature_signature_as_available(previous_activity_plan,1);
			template<typename TargetType> typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
//TODO
//				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
				Time_Seconds min_next = END;
				Activity_Plan* act;
				Activity_Plan* next = nullptr;
				
				// search for the closest following activity
				for (itr = activity_plans->begin(); itr != activity_plans->end(); ++itr)
				{
					act = (Activity_Plan*)(*itr);
					if (act->template Start_Is_Planned<bool>() && act->template Start_Time<Time_Seconds>() > start_time && act->template Start_Time<Time_Seconds>() < min_next)
					{
						min_next = act->template Start_Time<Time_Seconds>();
						next = act;
					}
				}
				return (typename TargetType::ReturnType)next;
			}
			tag_feature_signature_as_available(next_activity_plan,1);
			template<typename TargetType> typename TargetType next_activity_plan_by_act(typename TargetType current_act)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				Activity_Plan* current = (Activity_Plan*)current_act;

				// convert start time of current act to seconds
				Time_Seconds start_time = current->template Start_Time<Time_Seconds>();
				Time_Seconds min_next = END;
				Activity_Plan* act;
				
				Activity_Plan* next = nullptr;
				
				// search for the closest following activity
				for (itr = activity_plans->begin(); itr != activity_plans->end(); ++itr)
				{
					act = (Activity_Plan*)(*itr);
					if (act->template Start_Is_Planned<bool>() && act->template Start_Time<Time_Seconds>() >= start_time && act->template Start_Time<Time_Seconds>() < min_next)
					{
						if (act != current)
						{
							min_next = act->template Start_Time<Time_Seconds>();
							next = act;
						}
					}
				}
				return (TargetType)next;
			}
			tag_feature_signature_as_available(next_activity_plan,1);

			template<typename TargetType> typename TargetType::ReturnType previous_location(typename TargetType::ParamType current_activity)
			{
				PUSH_TO_STACK("previous_location");

				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Parent_Person_interface* person = (Parent_Person_interface*)_Parent_Person;

				// if the start time of the activity has not been planned set the previous location to home
				if (!act->template Start_Is_Planned<bool>()) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				Time_Seconds start = act->template Start_Time<Time_Seconds>();

//TODO
//				Activity_Plan* prev_act = this->previous_activity_plan<Target_Type<NT,Activity_Plan*,Time_Seconds>>(act->template Start_Time<Time_Seconds>());

				// if no previous activity, person is at home
				if (prev_act == nullptr)
				{
					POP_FROM_STACK;
					return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				}

				// if previous act location is not planned treat person as if at home
				if (!prev_act->template Location_Is_Planned<bool>())
				{
					POP_FROM_STACK;
					return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				}

				// otherwise, determine if person is at home or still at a previous activity
				_Activity_Location_Interface* orig = prev_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* dest = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if current location is in the process of being planned, use previous location as only deflected time matters
				if (dest == nullptr) 
				{
					POP_FROM_STACK;
					return (typename TargetType::ReturnType)orig;
				}

				// check if a stop at home will fit prior to activity
				Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
//TODO
//				Time_Seconds ttime_prev_to_home = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(orig, _Parent_Person->template Home_Location<_Activity_Location_Interface*>(),SOV,start);
//TODO
//				Time_Seconds ttime_home_to_this = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(),dest, SOV,start);
//TODO
//				Time_Seconds ttime_prev_to_this = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(orig, dest,SOV,start);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = min((float)ttime_prev_to_home,(float)ttime_home_to_this);			
				if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				POP_FROM_STACK;
				return (typename TargetType::ReturnType)orig;
			}
			tag_feature_signature_as_available(previous_location,1);
			template<typename TargetType> typename TargetType::ReturnType next_location(typename TargetType::ParamType current_activity)
			{
				PUSH_TO_STACK("next_location");

				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();

				// if the start time of the activity has not been planned set the next location to home
				if (!act->template Start_Is_Planned<bool>())
				{
					POP_FROM_STACK;
					return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				}
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

//TODO
//				//Activity_Plan* next_act = this->next_activity_plan<Target_Type<NT,Activity_Plan*,Time_Seconds>>(act->template Start_Time<Time_Seconds>());
				Activity_Plan* next_act = this->next_activity_plan_by_act<Activity_Plan*>(act);

				// if no next activity, person goes home
				if (next_act == nullptr)
				{
					POP_FROM_STACK;
					return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				}

				// if next act location is not planned treat person as if returning home
				if (!next_act->template Location_Is_Planned<bool>()) 
				{
					POP_FROM_STACK;
					return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				}

				// otherwise, determine if person goes home or to next activity
				_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* orig = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if act is currently being planned (i.e. is_planned is true, but location pointer is null) then use next location
				if (orig == nullptr)
				{
					POP_FROM_STACK;
					return (typename TargetType::ReturnType)dest;
				}

				// check if a stop at home will fit prior to activity
				Time_Seconds time_after = next_act->template Start_Time<Time_Seconds>() - end;
//TODO
//				Time_Seconds ttime_this_to_home = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(orig, _Parent_Person->template Home_Location<_Activity_Location_Interface*>(),SOV,end);
//TODO
//				Time_Seconds ttime_home_to_next = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(),dest, SOV,end);
//TODO
//				Time_Seconds ttime_this_to_next = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds>>(orig, dest,SOV,end);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = min((float)ttime_this_to_home,(float)ttime_home_to_next);			
				if (ttime_this_to_home + min_home_time + ttime_home_to_next  < time_after) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				POP_FROM_STACK;
				return (typename TargetType::ReturnType)dest;
			}
			tag_feature_signature_as_available(next_location,1);

			// Simplified timing conflict resolution - only modifying the new activity
			template<typename TargetType> bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans)
			{
				PUSH_TO_STACK("Resolve_Timing_Conflict");

				bool at_home_activity_modified = false;

				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Movement_Plan* move = act->template movement_plan<Movement_Plan*>();


		
				// if the start time of the activity has not been planned no way to identify timing conflicts
				if (!act->template Start_Is_Planned<bool>())
				{
					POP_FROM_STACK;
					return false;
				}

				// else get the start and end times for the activity
				Time_Seconds start = act->template Start_Time<Time_Seconds>();
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

				// get the activity location for calculating approximate travel times to from surrounding activities
				_Activity_Location_Interface* loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				if (act->template Location_Is_Planned<bool>()) loc = act->template Location<_Activity_Location_Interface*>();

				// Get the surrounding activities in the schedule
//TODO
//				Activity_Plan* prev_act = this->previous_activity_plan<Target_Type<NT,Activity_Plan*,Time_Seconds>>(start);
				Activity_Plan* next_act = this->next_activity_plan_by_act<Activity_Plan*>(act);
				Movement_Plan* next_move = nullptr;
				if (next_act != nullptr) next_move = next_act->template movement_plan<Movement_Plan*>();

				Time_Seconds ttime_prev;
				Time_Seconds ttime_next;
				
				//=======================================================================
				// find maximum end time of the new activity
				Time_Seconds end_max;
				_Activity_Location_Interface* next_loc=nullptr;
				if (next_act == nullptr) end_max = END;
				else
				{
					// if next activity location is known, get the expected travel time from current to next, else assume 20 minutes				
					if (next_act->template Location_Is_Planned<bool>())
					{
						next_loc = next_act->template Location<_Activity_Location_Interface*>();
//TODO
//						ttime_next = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(loc, next_loc,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, end);
					}
					else ttime_next = 20.0 * 60.0;

					// get maximum end time of current activity given departure time for next activity
					end_max = next_act->template Start_Time<Time_Seconds>() - ttime_next;
				}

				//=======================================================================
				// find minimum start time of the new activity
				Time_Seconds start_min;
				_Activity_Location_Interface* prev_loc=nullptr;
				if (prev_act == nullptr) start_min = iteration()+1;
				else
				{
					// modify the previous activity if it is an at-home act
					if (prev_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
					{
						Resolve_At_Home_Timing_Conflict<NT,Activity_Plan*>(act, prev_act);
						at_home_activity_modified = true;
					}

					// if previous activity location is known, get the expected travel time from current to next, else assume 20 minutes			
					if (prev_act->template Location_Is_Planned<bool>())
					{
						prev_loc = prev_act->template Location<_Activity_Location_Interface*>();
//TODO
//						ttime_prev = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(prev_loc,loc, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
					}
					else ttime_prev = 20.0 * 60.0;

					// get maximum end time of current activity given departure time for next activity
					start_min = prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>() + ttime_prev;
				}

				// determine conflict regimes and modify appropriately
				Time_Seconds available_time = end_max - start_min;


				// 1. simple conflict, fits without shortening
				if (available_time > end - start)
				{
					if (start < start_min) act->template Start_Time<Time_Seconds>(start_min); // move forward in time
					if (end > end_max) act->template Start_Time<Time_Seconds>(start - (end-end_max)); // move backward in time
					// update the movement plans of surrounding activities
					if (update_movement_plans) 
					{
						// No need to resolve conflicts for return home activities, these are scheduled on-the-fly and should always fit
						if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
						{
							At_Home_Activity_Plan* at_home_act = (At_Home_Activity_Plan*)act;
							act->template Start_Time<Time_Seconds>(at_home_act->template Fixed_Departure<Time_Seconds>()+ttime_prev);
						}
						else 
						{
							move->template departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev); //act->template Expected_Travel_Time<Time_Seconds>());
						}
						// update the following movement plan to account for new travel time
						if (next_act != nullptr) 
						{
							next_move->template origin<_Activity_Location_Interface*>(loc);
							next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);
						}
					}
					POP_FROM_STACK;
					return true;
				}
				// 2. complex conflict, move and shorten
				else if (available_time > act->template Minimum_Duration<Time_Seconds>() )
				{
					//cout << ", resolution type: MOVE AND SHORTEN"<<endl;
					act->template Start_Time<Time_Seconds>(start_min);
					act->template Duration<Time_Seconds>(end_max-start_min);
					if (update_movement_plans)
					{
						// No need to resolve conflicts for return home activities, these are scheduled on-the-fly and should always fit
						if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
						{
							At_Home_Activity_Plan* at_home_act = (At_Home_Activity_Plan*)act;
							act->template Start_Time<Time_Seconds>(at_home_act->template Fixed_Departure<Time_Seconds>()+ttime_prev);
						}
						else 
						{
							move->template departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev); //act->template Expected_Travel_Time<Time_Seconds>());
						}
						// update the following movement plan to account for new travel time
						if (next_act != nullptr) 
						{
							next_move->template origin<_Activity_Location_Interface*>(loc);
							next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);
						}
					}
					POP_FROM_STACK;
					return true;
				}
				// 3. can't fit, delete activity
				else
				{
					if (next_act != nullptr) 
					{
						if (prev_loc == nullptr) prev_loc =  _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						if (next_loc == nullptr) next_loc =  _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//TODO
//						ttime_next = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(prev_loc, next_loc,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, end);
					
						next_move->template origin<_Activity_Location_Interface*>(prev_loc);
						next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);

						if (at_home_activity_modified) prev_act->template End_Time<Time_Seconds>(next_move->template departed_time<Time_Seconds>(),false);

					}
					else
					{
						if (at_home_activity_modified) prev_act->template End_Time<Time_Seconds>(END,false);
					}
					POP_FROM_STACK;
					return false;
				}
				POP_FROM_STACK;
			}
			tag_feature_signature_as_available(Resolve_Timing_Conflict,1);
			template<typename TargetType> void Resolve_At_Home_Timing_Conflict(TargetType current_activity, TargetType previous_activity)
			{
				PUSH_TO_STACK("Resolve_At_Home_Timing_Conflict");

				Activity_Plan* act = (Activity_Plan*)current_activity;
				Activity_Plan* prev_act = (Activity_Plan*)previous_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Movement_Plan* move = act->template movement_plan<Movement_Plan*>();

				// if the start time of the activity has not been planned no way to identify timing conflicts
				if (!act->template Start_Is_Planned<bool>())
				{
					POP_FROM_STACK;
					return;
				}

				// else get the start and end times for the activity
				Time_Seconds start = act->template Start_Time<Time_Seconds>();
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

				// get the activity location for calculating approximate travel times to from surrounding activities
				_Activity_Location_Interface* loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				if (act->template Location_Is_Planned<bool>()) loc = act->template Location<_Activity_Location_Interface*>();


				//=======================================================================
				// find minimum start time of the new activity
				Time_Seconds new_end;

				// if next activity location is known, get the expected travel time from current to next, else assume 20 minutes
				Time_Seconds ttime;
				_Activity_Location_Interface* prev_loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
//TODO
//				ttime = network->template Get_TTime<Target_Type<NT,Time_Seconds,_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(prev_loc,loc, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);


				// get maximum end time of current activity given departure time for next activity
				new_end = act->template Start_Time<Time_Seconds>() - ttime;
				prev_act->template End_Time<Time_Seconds>(new_end,false);
				POP_FROM_STACK;
			}
			tag_feature_signature_as_available(Resolve_At_Home_Timing_Conflict,1);

			// Adding activities and movements to the planning schedules
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan, requires(TargetType,check(strip_modifiers(TargetType),is_pointer) && check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;
				// get the associated activity
				Activity_Plan* act = move->template destination_activity_reference<Activity_Plan*>();


				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < iteration()) return;

				
				// check for timing conflicts again and updated movement plans
				bool added = this->Resolve_Timing_Conflict<Activity_Plan*>(act,true);

				if (!added)
				{
					//cout << endl << "activity not added: " << act->Activity_Plan_ID<int>() << endl;
					act->template Unschedule_Activity_Events<NT>();
					this->template Remove_Activity_Plan<Activity_Plan*>(act);

					// if the failed movement addition represents a return home activity, advance the departure time of the following activity so no gaps left in schedule
					if (act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
					{
						Activity_Plan* next_act = this->next_activity_plan_by_act<Activity_Plan*>(act);
						if (next_act == nullptr) return;

						Movement_Plan* next_move = next_act->movement_plan<Movement_Plan*>();
						if (next_move == nullptr) return;

						next_move->departed_time<Simulation_Timestep_Increment>(move->departed_time<Simulation_Timestep_Increment>());
					}
					return;
				}

				// Insert new movement plan at the appropriate space in plan schedule
				Movement_Plans* movements = this->template Movement_Plans_Container<Movement_Plans*>();
				move_itr = movements->begin();
				while(move_itr != movements->end())
				{
					Movement_Plan* current_move = *move_itr;
					if (current_move->template departed_time<Simulation_Timestep_Increment>() > move->template departed_time<Simulation_Timestep_Increment>()) break;
					++move_itr;

					WHILE_PRINT("add movement while loop");
				}
				movements->insert(move_itr,move);	
				

				//// modify the origin for the following movement plan
				//if (move_itr != movements->end())
				//{
				//	Movement_Plan* next_move = *move_itr;				
				//	Activity_Plan* next_act = next_move->template destination_activity_reference<Activity_Plan*>();
				//	// update the movement plan to next act
//TODO
//				//	_Activity_Location_Interface* orig = this->previous_location<Target_Type<NT,_Activity_Location_Interface*,Activity_Plan*>>(next_act);
				//	_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
				//	Simulation_Timestep_Increment end = act->template Start_Time<Simulation_Timestep_Increment>() + act->template Duration<Simulation_Timestep_Increment>();
				//	next_act->template Update_Movement_Plan<_Activity_Location_Interface*>(orig,dest,end);
				//}

				

				// log the activity throught the global person logger
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				typedef  Person_Components::Prototypes::Person_Data_Logger< typename MasterType::person_data_logger_type> _Logger_Interface;
				if (scenario->template write_activity_output<bool>()) ((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Plan*>(act,false);

				// no logging if the movement is a return home movement
				if (act == nullptr) return;

				// cache the movement plans activity in the activity record container since all attributes have been planned at this point			
				//Activity_Records* act_records = _Parent_Person->template Activity_Record_Container<Activity_Records*>();
				//Activity_Record* act_record = (Activity_Record*)Allocate<typename MasterType::activity_record_type>();
				//act_record->template Initialize<Activity_Plan*>(act);
				//act_records->push_back(act_record);
				
			}
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan, requires(TargetType,!check(strip_modifiers(TargetType),is_pointer) || !check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check(strip_modifiers(TargetType),is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			template<typename TargetType> void Remove_Movement_Plan(TargetType movement_plan, requires(TargetType,check(strip_modifiers(TargetType),is_pointer)))
			{
				//typedef Activity_Components::Prototypes::Activity_Plan<typename remove_pointer<typename type_of(Activity_Plans_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence<type_of(Activity_Plans_Container),Activity_Plan*> Activity_Plans;

				Movement_Plan* move = (Movement_Plan*)movement_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<stringstream&>(s);
				//}

				Movement_Plans* movements = this->Movement_Plans_Container<Movement_Plans*>();		
				
				typename Movement_Plans::iterator itr = movements->begin();

				for (; itr != movements->end(); ++itr)
				{
					if (*itr == move)
					{
						movements->erase(itr);
						break;
					}
				}
			}
		
			
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan, requires(TargetType,check(strip_modifiers(TargetType),is_pointer)/* && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{
				//typedef Activity_Components::Prototypes::Activity_Plan<typename remove_pointer<typename type_of(Activity_Plans_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence<type_of(Activity_Plans_Container),Activity_Plan*> Activity_Plans;

				Activity_Plan* act = (Activity_Plan*)activity_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<stringstream&>(s);
				//}

				Activity_Plans* activities = this->Activity_Container<Activity_Plans*>();		
				activities->push_back(act);
				_Activity_Count++;
			}
			template<typename TargetType> void Remove_Activity_Plan(TargetType activity_plan, requires(TargetType,check(strip_modifiers(TargetType),is_pointer)/* && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{
				//typedef Activity_Components::Prototypes::Activity_Plan<typename remove_pointer<typename type_of(Activity_Plans_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence<type_of(Activity_Plans_Container),Activity_Plan*> Activity_Plans;

				Activity_Plan* act = (Activity_Plan*)activity_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<stringstream&>(s);
				//}

				Activity_Plans* activities = this->Activity_Container<Activity_Plans*>();		
				
				typename Activity_Plans::iterator itr = activities->begin();

				for (; itr != activities->end(); ++itr)
				{
					if (*itr == act)
					{
						activities->erase(itr);
						break;
					}
				}
			}
		
			static bool comparer(typename MasterType::activity_type* act1, typename MasterType::activity_type* act2)
			{
				//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
				//typedef Back_Insertion_Sequence< type_of(Activity_Container),Activity_Plan*> Activity_Plans;

				Activity_Plan* act1_itf = (Activity_Plan*)act1;
				Activity_Plan* act2_itf = (Activity_Plan*)act2;
				return (act1_itf->template Start_Time<Time_Seconds>() < act2_itf->template Start_Time<Time_Seconds>());
			}
			
			template<typename TargetType> TargetType Sort_Activity_Schedule()
			{
				Activity_Plans* activities = this->Activity_Container<Activity_Plans*>();
				boost::container::list<typename MasterType::activity_type*>* acts = (list<typename MasterType::activity_type*>*)activities;

				acts->sort(comparer);
				
			} tag_feature_as_available(Sort_Activity_Schedule);


		};

	}

}
