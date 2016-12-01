#pragma once

#include "Person_Scheduler_Prototype.h"
#include "Person_Prototype.h"
#include "Movement_Plan_Prototype.h"
#include "Activity_Prototype.h"
#include "Vehicle_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Class to temporarily define activity conflicts
		//----------------------------------------------------------------------------------
		implementation struct Activity_Conflict : public Polaris_Component<MasterType,INHERIT(Activity_Conflict),NT>
		{
			// The newly added activity
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, original_activity, NONE, NONE);

			// The originally scheduled activities
			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, conflicting_activity, NONE, NONE);

			Types::CONFLICT_TYPES conflict_type;

			Types::RESOLUTION_TYPES resolution_type;

			m_data(float, percent_overlap,NONE,NONE);

			m_data(float, time_available,NONE,NONE);

			m_data(bool, out_of_home,NONE,NONE);
			m_data(bool, original_out_of_home,NONE,NONE);
			m_data(bool, conflicting_out_of_home,NONE,NONE);
		};


		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Person_Scheduler_Implementation : public Polaris_Component<MasterType,INHERIT(General_Person_Scheduler_Implementation),Execution_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(General_Person_Scheduler_Implementation),Execution_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person, typename MasterType::person_type, Parent_Person, NONE, NONE);
			m_data(int, Activity_Count, NONE, NONE);

			//Containers for activity planning events and movement planning events
			m_container(std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*>,Activity_Container, NONE, NONE);
			//m_prototype_container(std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_type>*>,typename MasterType::activity_type,Activity_Container, NONE, NONE);
			m_container(std::list<Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type>*>,Movement_Plans_Container, NONE, NONE);

			m_prototype(Activity_Components::Prototypes::Activity_Planner, typename MasterType::activity_type, Current_Activity, NONE, NONE);

			// Interface definitions
			typedef Household_Components::Prototypes::Household< typename type_of(Parent_Person)::type_of(Household)> _Household_Interface;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Person)::type_of(Perception_Faculty)::type_of(Scenario)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Person)::type_of(Perception_Faculty)::type_of(Network)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
			
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Back_Insertion_Sequence<type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Activity_Plan;
			//typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type> Activity_Plan;

			typedef Back_Insertion_Sequence<type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;
			
			typedef Back_Insertion_Sequence<typename type_of(Parent_Person)::type_of(Activity_Record_Container)> Activity_Records;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Records)> Activity_Record;
			
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> At_Home_Activity_Plan;


			// member features
			template<typename TargetType> void Initialize()
			{
				this->_Current_Activity = (Current_Activity_type)Allocate<type_of(Current_Activity)>();
			}
			template<typename ActivityType> void Update_Current_Activity(ActivityType current_act)
			{
				this->_Current_Activity->template Copy<ActivityType>(current_act);
			}

			// scheduling features - move to Person_Scheduler eventually
			//template<typename TargetType> TargetType current_movement_plan(requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			//{
			//	// Define interfaces to the container members of the class			
			//	Movement_Plans* movement_plans = this->template Movement_Plans_Container< Movement_Plans*>();
			//	typename Movement_Plans::iterator itr;
			//	if ((itr = movement_plans->find(iteration())) != movement_plans->end()) return (TargetType)*itr;
			//	else return NULL;
			//}
			//template<typename TargetType> TargetType current_activity_plan(requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			//{
			//	// Define interfaces to the container members of the class
			//	Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
			//	typename Activity_Plans::iterator itr;
			//	if ((itr = activity_plans->find(iteration())) != activity_plans->end()) return (TargetType)*itr;
			//	else return NULL;
			//}

			template<typename TimeType,  typename ReturnType> ReturnType previous_activity_plan(TimeType current_time, requires(TimeType, check(strip_modifiers(TimeType),Is_Time_Value)))
			{
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Seconds>(current_time);
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

				// return either previous activity in the schedule (or the current activity if previous activity has already been removed
				if (previous == nullptr)
				{
					return (ReturnType)_Current_Activity;
				}
				else if (this->_Current_Activity->template Start_Time<Time_Seconds>() > previous->template Start_Time<Time_Seconds>())
				{
					return (ReturnType)_Current_Activity;
				}
				else
				{
					return (ReturnType)previous;
				}
			}
			template<typename ParamType, typename ReturnType> ReturnType previous_activity_plan(ParamType current_act,requires(ParamType, check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// convert start time of current act to seconds
				Activity_Plan* current = (Activity_Plan*)current_act;
				Time_Seconds start_time = current->template Start_Time<Time_Seconds>();
				
				// call the time-based version of next_activity_plan using current_act start
				return (ReturnType)previous_activity_plan<Time_Seconds,ReturnType>(start_time);
			}
			template<typename ParamType, typename ReturnType> ReturnType previous_activity_plan(ParamType value, requires(ParamType, !check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan_Prototype) && !check(strip_modifiers(ParamType),Is_Time_Value)))
			{
				assert_check(strip_modifiers(ParamType), Is_Time_Value,"ParamType must be either a Time_Value type, or ");
				assert_check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan_Prototype,"ParamType must be an Activity_Plan_Prototype.");
			}

			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_time, requires(ParamType, check(strip_modifiers(ParamType),Is_Time_Value)))
			{
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<ParamType, Time_Seconds>(current_time);
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
				return (ReturnType)next;
			}
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType current_act, requires(ParamType, check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// convert start time of current act to seconds
				Activity_Plan* current = (Activity_Plan*)current_act;
				Time_Seconds start_time = current->template Start_Time<Time_Seconds>();
				
				// call the time-based version of next_activity_plan using current_act start
				return (ReturnType)next_activity_plan<Time_Seconds,ReturnType>(start_time);
			}
			template<typename ParamType, typename ReturnType> ReturnType next_activity_plan(ParamType value, requires(ParamType, !check(strip_modifiers(ParamType),Activity_Components::Concepts::Is_Activity_Plan_Prototype) && !check(strip_modifiers(ParamType),Is_Time_Value)))
			{
				assert_check(strip_modifiers(ParamType), Is_Time_Value,"ParamType must be either a Time_Value type, or ");
				assert_check(strip_modifiers(ParamType), Activity_Components::Concepts::Is_Activity_Plan_Prototype,"ParamType must be an Activity_Plan_Prototype.");
			}

			template<typename ActivityItfType, typename ReturnType> ReturnType previous_location(ActivityItfType current_activity)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Parent_Person_type* person = (Parent_Person_type*)_Parent_Person;

				// if the start time of the activity has not been planned set the previous location to home
				if (!act->template Start_Is_Planned<bool>()) return _Parent_Person->template Home_Location<ReturnType>();
				Time_Seconds start = act->template Start_Time<Time_Seconds>();

				Activity_Plan* prev_act = this->previous_activity_plan<Time_Seconds,Activity_Plan*>(act->template Start_Time<Time_Seconds>());

				// if no previous activity, person is at home
				if (prev_act == nullptr)
				{
					return _Parent_Person->template Home_Location<ReturnType>();
				}

				// if previous act location is not planned treat person as if at home
				if (!prev_act->template Location_Is_Planned<bool>())
				{
					return _Parent_Person->template Home_Location<ReturnType>();
				}

				// otherwise, determine if person is at home or still at a previous activity
				_Activity_Location_Interface* orig = prev_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* dest = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if current location is in the process of being planned, use previous location as only deflected time matters
				if (dest == nullptr) 
				{
					return (ReturnType)orig;
				}

				// check if a stop at home will fit prior to activity
				Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
				Time_Seconds ttime_prev_to_home = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(orig, _Parent_Person->template Home_Location<_Activity_Location_Interface*>(),SOV,start);
				Time_Seconds ttime_home_to_this = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(),dest, SOV,start);
				Time_Seconds ttime_prev_to_this = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds,Time_Seconds>(orig, dest,SOV,start);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = min((float)ttime_prev_to_home,(float)ttime_home_to_this);			
				if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				return (ReturnType)orig;
			}
			template<typename ActivityItfType, typename ReturnType> ReturnType next_location(ActivityItfType current_activity)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();

				// if the start time of the activity has not been planned set the next location to home
				if (!act->template Start_Is_Planned<bool>())
				{
					return _Parent_Person->template Home_Location<ReturnType>();
				}
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();
				
				//Activity_Plan* next_act = this->next_activity_plan<Time_Seconds,Activity_Plan*>(act->template Start_Time<Time_Seconds>());
				Activity_Plan* next_act = this->next_activity_plan<Activity_Plan*,Activity_Plan*>(act);

				// if no next activity, person goes home
				if (next_act == nullptr)
				{
					return _Parent_Person->template Home_Location<ReturnType>();
				}

				// if next act location is not planned treat person as if returning home
				if (!next_act->template Location_Is_Planned<bool>()) 
				{
					return _Parent_Person->template Home_Location<ReturnType>();
				}

				// otherwise, determine if person goes home or to next activity
				_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* orig = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if act is currently being planned (i.e. is_planned is true, but location pointer is null) then use next location
				if (orig == nullptr)
				{
					return (ReturnType)dest;
				}

				// check if a stop at home will fit prior to activity
				Time_Seconds time_after = next_act->template Start_Time<Time_Seconds>() - end;
				Time_Seconds ttime_this_to_home = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(orig, _Parent_Person->template Home_Location<_Activity_Location_Interface*>(),SOV,end);
				Time_Seconds ttime_home_to_next = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(),dest, SOV,end);
				Time_Seconds ttime_this_to_next = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(orig, dest,SOV,end);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = min((float)ttime_this_to_home,(float)ttime_home_to_next);			
				if (ttime_this_to_home + min_home_time + ttime_home_to_next  < time_after) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				return (ReturnType)dest;
			}

			// Simplified timing conflict resolution - only modifying the new activity (except for split activities)
			template<typename TargetType> bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans)
			{
				bool at_home_activity_modified = false;

				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Movement_Plan* move = act->template movement_plan<Movement_Plan*>();


		
				// if the start time of the activity has not been planned no way to identify timing conflicts
				if (!act->template Start_Is_Planned<bool>())
				{
					return false;
				}


				/*std::vector<Activity_Conflict<MT>> conflict_list;
				Identify_Conflicts<TargetType>(current_activity,conflict_list);*/


				// else get the start and end times for the activity
				Time_Seconds start = act->template Start_Time<Time_Seconds>();
				Time_Seconds dur = act->template Duration<Time_Seconds>();
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

				// get the activity location for calculating approximate travel times to from surrounding activities
				_Activity_Location_Interface* loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				if (act->template Location_Is_Planned<bool>()) loc = act->template Location<_Activity_Location_Interface*>();

				// Get the surrounding activities in the schedule
				Activity_Plan* prev_act = this->previous_activity_plan<Time_Seconds,Activity_Plan*>(start);
				Activity_Plan* next_act = this->next_activity_plan<Activity_Plan*,Activity_Plan*>(act);
				Movement_Plan* next_move = nullptr;
				if (next_act != nullptr)
				{
					if (next_act->template Route_Is_Planned<bool>())	next_move = next_act->template movement_plan<Movement_Plan*>();
				}

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
						ttime_next = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds,Time_Seconds>(loc, next_loc,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, end);
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
						Resolve_At_Home_Timing_Conflict<Activity_Plan*>(act, prev_act);
						at_home_activity_modified = true;
					}

					// if previous activity location is known, get the expected travel time from current to next, else assume 20 minutes			
					if (prev_act->template Location_Is_Planned<bool>())
					{
						prev_loc = prev_act->template Location<_Activity_Location_Interface*>();
						ttime_prev = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds,Time_Seconds>(prev_loc,loc, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
					}
					else ttime_prev = 20.0 * 60.0;

					// get maximum end time of current activity given departure time for next activity
					start_min = prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>() + ttime_prev;
				}

				// determine conflict regimes and modify appropriately
				Time_Seconds available_time = end_max - start_min;

				Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type>* new_act = nullptr;

				// 0. insertion conflicts
				// check for insertion conflicts - does not work with the current start_min/end_max setup
				// Currently valid for original acts longer than 2 hours, where the conflicting act starts at least ttime + min_duration (300s) into the original act, 
				// and ends ttime+min_duration before the end of the original act
				if (prev_act != nullptr)
				{
					if (prev_act->template Duration<Time_Hours>() > 2.0 && prev_act->template Start_Time<Time_Seconds>() + ttime_prev + 300.0 < act->template Start_Time<Time_Seconds>() && prev_act->template End_Time<Time_Seconds>() >= act->template End_Time<Time_Seconds>() + ttime_prev + 300.0 && prev_act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() != Activity_Components::Types::AT_HOME_ACTIVITY)
					{
						//Allocate new activity for the split half and copy from the other half
						new_act = (Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type>*)Allocate<MasterType::activity_plan_type>();
						new_act->template Copy<Activity_Plan*>(prev_act);
						new_act->template Activity_Plan_ID<int>(prev_act->template Activity_Plan_ID<int>() + 1000);
						new_act->template Start_Time<Time_Seconds>(act->template End_Time<Time_Seconds>()+ttime_prev);
						new_act->template End_Time<Time_Seconds>(prev_act->template End_Time<Time_Seconds>(), false);
						Revision &route = new_act->template Route_Planning_Time<Revision&>();
						route._iteration = iteration() + 1;
						route._sub_iteration = 0;
						new_act->template Schedule_Activity_Events<NT>();
					
						//modify the preceding portion of the split activity
						prev_act->template End_Time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev,false);

						//update movement plans
						if (update_movement_plans) move->template departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev);

						Time_Seconds ttime_new = network->template Get_TTime<_Activity_Location_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Seconds, Time_Seconds>(loc, prev_loc, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, new_act->template Start_Time<Time_Seconds>());

						//update the start and end constraints to account for the new split time
						start_min = prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>() + ttime_prev;
						end_max = new_act->template Start_Time<Time_Seconds>() - ttime_new;
					}
				}


				// 1. simple conflict, fits without shortening
				if (available_time > end - start)
				{
					if (start < start_min) act->template Start_Time<Time_Seconds>(start_min); // move forward in time
					if (end > end_max) act->template Start_Time<Time_Seconds>(start - (end-end_max)); // move backward in time
					// update the movement plans of surrounding activities
					if (update_movement_plans) 
					{
						// No need to resolve conflicts for return home activities, these are scheduled on-the-fly and should always fit
						if (act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
						{
							At_Home_Activity_Plan* at_home_act = (At_Home_Activity_Plan*)act;
							act->template Start_Time<Time_Seconds>(at_home_act->template Fixed_Departure<Time_Seconds>()+ttime_prev);
						}
						else 
						{
							move->template departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev); //act->template Expected_Travel_Time<Time_Seconds>());
						}
						// update the following movement plan to account for new travel time
						if (next_act != nullptr && next_move != nullptr) 
						{
							next_move->template Update_Locations<_Activity_Location_Interface*>(loc, next_move->template destination<_Activity_Location_Interface*>());
							next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);
						}
					}
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
						if (act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
						{
							At_Home_Activity_Plan* at_home_act = (At_Home_Activity_Plan*)act;
							act->template Start_Time<Time_Seconds>(at_home_act->template Fixed_Departure<Time_Seconds>()+ttime_prev);
						}
						else 
						{
							move->template departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - ttime_prev); //act->template Expected_Travel_Time<Time_Seconds>());
						}
						// update the following movement plan to account for new travel time
						if (next_act != nullptr && next_move != nullptr) 
						{
							//TODO: verify this code-> changed because origin links were not updating properly
							//next_move->template origin<_Activity_Location_Interface*>(loc);
							next_move->template Update_Locations<_Activity_Location_Interface*>(loc, next_move->template destination<_Activity_Location_Interface*>());
							next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);
						}
					}
					return true;
				}
				// 3. can't fit, delete activity
				else
				{
					// handle the split of the previous activity - rejoin and delete new act
					if (new_act != nullptr)
					{
						prev_act->End_Time(new_act->End_Time<Time_Minutes>(),false);
						this->Remove_Activity_Plan(new_act);
					}

					// otherwise, just update the modified travel between the existing activities
					else if (next_act != nullptr) 
					{
						if (prev_loc == nullptr) prev_loc =  _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						if (next_loc == nullptr) next_loc =  _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
						ttime_next = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds,Time_Seconds>(prev_loc, next_loc,Vehicle_Components::Types::Vehicle_Type_Keys::SOV, end);
					
						if (next_move != nullptr)
						{
							next_move->template Update_Locations<_Activity_Location_Interface*>(prev_loc, next_move->template destination<_Activity_Location_Interface*>());
							//next_move->template origin<_Activity_Location_Interface*>(prev_loc);
							next_move->template departed_time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>() - ttime_next);
						}

						if (at_home_activity_modified)
						{
							if (next_move != nullptr) prev_act->template End_Time<Time_Seconds>(next_move->template departed_time<Time_Seconds>(),false);
							else prev_act->template End_Time<Time_Seconds>(next_act->template Start_Time<Time_Seconds>(),false);
						}

					}
					else
					{
						if (at_home_activity_modified) prev_act->template End_Time<Time_Seconds>(END,false);
					}
					return false;
				}
			}
			template<typename TargetType> void Resolve_At_Home_Timing_Conflict(TargetType current_activity, TargetType previous_activity)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				Activity_Plan* prev_act = (Activity_Plan*)previous_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Movement_Plan* move = act->template movement_plan<Movement_Plan*>();

				// if the start time of the activity has not been planned no way to identify timing conflicts
				if (!act->template Start_Is_Planned<bool>())
				{
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
				ttime = network->template Get_TTime<_Activity_Location_Interface*,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds,Time_Seconds>(prev_loc,loc, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);


				// get maximum end time of current activity given departure time for next activity
				new_end = act->template Start_Time<Time_Seconds>() - ttime;

				//TODO: remove when done testing
				if (new_end > (END)*2.0)
				{
					THROW_WARNING("Error, invalid value when setting end_time: "<<new_end<<", act.start="<<act->Start_Time<Time_Seconds>()<<", start="<<start<<", ttime="<<ttime<<", prev="<<prev_loc->zone<_Zone_Interface*>()->uuid<int>()<<", next="<<loc->zone<_Zone_Interface*>()->uuid<int>());
				}
				prev_act->template End_Time<Time_Seconds>(new_end,false);
			}

			// Adding activities and movements to the planning schedules
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan, requires(TargetType,check(TargetType,is_pointer) && check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;
				// get the associated activity
				Activity_Plan* act = move->template destination_activity_reference<Activity_Plan*>();

				//TODO: remove when done testing scheduler code
				if (_Parent_Person->Household<_Household_Interface*>()->uuid<int>() == 11)
				{
					int test = 1;
				}

				//======================================================
				// log the activity throught the global person logger
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				typedef  Person_Components::Prototypes::Person_Data_Logger< typename MasterType::person_data_logger_type> _Logger_Interface;
				if (scenario->template write_activity_output<bool>()) ((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Plan*>(act, false);


				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < iteration()) return;

				// check for timing conflicts again and updated movement plans
				bool added = this->Resolve_Timing_Conflict<Activity_Plan*>(act,true);

				if (!added)
				{
					//cout << endl << "activity not added: " << act->Activity_Plan_ID<int>() << endl;
					//act->template Unschedule_Activity_Events<NT>();
					this->template Remove_Activity_Plan<Activity_Plan*>(act);

					// if the failed movement addition represents a return home activity, advance the departure time of the following activity so no gaps left in schedule
					if (act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
					{
						Activity_Plan* next_act = this->template next_activity_plan<Activity_Plan*,Activity_Plan*>(act);
						if (next_act == nullptr) return;

						Movement_Plan* next_move = next_act->template movement_plan<Movement_Plan*>();
						if (next_move == nullptr || !next_act->template Route_Is_Planned<bool>()) return;

						next_move->template departed_time<Simulation_Timestep_Increment>(move->template departed_time<Simulation_Timestep_Increment>());
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
				//	_Activity_Location_Interface* orig = this->previous_location<Activity_Plan*,_Activity_Location_Interface*>(next_act);
				//	_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
				//	Simulation_Timestep_Increment end = act->template Start_Time<Simulation_Timestep_Increment>() + act->template Duration<Simulation_Timestep_Increment>();
				//	next_act->template Update_Movement_Plan<_Activity_Location_Interface*>(orig,dest,end);
				//}


				// no logging if the movement is a return home movement
				if (act == nullptr) return;

				// cache the movement plans activity in the activity record container since all attributes have been planned at this point			
				//Activity_Records* act_records = _Parent_Person->template Activity_Record_Container<Activity_Records*>();
				//Activity_Record* act_record = (Activity_Record*)Allocate<typename MasterType::activity_record_type>();
				//act_record->template Initialize<Activity_Plan*>(act);
				//act_records->push_back(act_record);
				
			}
			template<typename TargetType> void Add_Movement_Plan(TargetType movement_plan, requires(TargetType,!check(TargetType,is_pointer) || !check(strip_modifiers(TargetType),Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(strip_modifiers(TargetType), Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			template<typename TargetType> void Remove_Movement_Plan(TargetType movement_plan, requires(TargetType,check(TargetType,is_pointer)))
			{
				Movement_Plan* move = (Movement_Plan*)movement_plan;

				Movement_Plans* movements = this->Movement_Plans_Container<Movement_Plans*>();		
				
				typename Movement_Plans::iterator itr = movements->begin();
				for (; itr != movements->end(); ++itr)
				{
					if (*itr == move)
					{
						movements->erase(itr);
						return;
					}
				}

				//THROW_EXCEPTION("Error: this movement plan not found in the movemen_plans_container.")

			}
		
			
			template<typename TargetType> void Add_Activity_Plan(TargetType activity_plan, requires(TargetType,check(TargetType,is_pointer)/* && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
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
			template<typename TargetType> void Remove_Activity_Plan(TargetType activity_plan, requires(TargetType,check(TargetType,is_pointer)/* && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{

				Activity_Plan* act = (Activity_Plan*)activity_plan;
				Movement_Plan* move = act->template movement_plan<Movement_Plan*>();
				
				// if movement plan is not null check for refreence mismatch, then remove from schedule
				if (move != nullptr)
				{
					if (act != move->template destination_activity_reference<Activity_Plan*>() ) THROW_EXCEPTION("ERROR: mismatch between movement and activity references when deleting activity plan.");
					Remove_Movement_Plan(move);
				}

				// Then remove activity - calls activity free function, which deletes the memory for movement plan
				Activity_Plans* activities = this->Activity_Container<Activity_Plans*>();		
				
				for (auto itr = activities->begin(); itr != activities->end(); ++itr)
				{
					if (*itr == act)
					{
						activities->erase(itr);
						break;
					}
				}
				// Free the activity using interface
				act->template Free_Activity<NT>();
			}
			//template<typename TargetType> void Remove_Activity_Plan(TargetType activity_plan, requires(TargetType,check(TargetType,is_pointer)/* && check(strip_modifiers(TargetType),Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			//{
			//	Activity_Plan* act = (Activity_Plan*)activity_plan;

			//	Activity_Plans* activities = this->Activity_Container<Activity_Plans*>();		
			//	
			//	typename Activity_Plans::iterator itr = activities->begin();

			//	for (; itr != activities->end(); ++itr)
			//	{
			//		if (*itr == act)
			//		{
			//			activities->erase(itr);
			//			break;
			//		}
			//	}

			//	act->Free_Activity<NT>();
			//	
			//	//THROW_EXCEPTION("Error: this activity plan not found in the Activity_Container.")
			//}	

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
				std::list<typename MasterType::activity_type*>* acts = (std::list<typename MasterType::activity_type*>*)activities;

				acts->sort(comparer);
				
			}


			//=======================================================================================================
			//
			// CONFLICT RESOLUTION ROUTINES
			//
			// - Assume ActivityItfType has been verified as a valid Activity_Prototype pointer
			//
			//-------------------------------------------------------------------------------------------------------
			template<typename ActivityItfType> bool Identify_Conflicts(ActivityItfType current_activity, std::vector<Activity_Conflict<MT>>& conflict_list)
			{
				Activity_Plan* current = (Activity_Plan*)current_activity;
				Activity_Plan* previous = previous_activity_plan<Activity_Plan*,Activity_Plan*>(current);
				Activity_Plan* next = next_activity_plan<Activity_Plan*,Activity_Plan*>(current);
				Activity_Plan* next_next = nullptr;

				// if the following activity is still within the current activity, get the next following activity
				if (next->template End_Time<Time_Seconds>() < current->template End_Time<Time_Seconds>())
				{
					next_next = next_activity_plan<Activity_Plan*,Activity_Plan*>(next);

					// if two activities are wholly overlapped by current activity, return false - can not resolve this conflict
					if (next_next->template End_Time<Time_Seconds>() < current->template End_Time<Time_Seconds>()) return false;
				}

				// Determine if a conflict exists with any of the three surrounding activities
				bool has_conflict = Define_Conflict<Activity_Plan*>(current, previous, conflict_list) || Define_Conflict<Activity_Plan*>(current, next, conflict_list) || Define_Conflict<Activity_Plan*>(current, next_next, conflict_list);
			}

			template<typename ActivityItfType> bool Define_Conflict(ActivityItfType conflicting_activity, ActivityItfType original_activity, std::vector<Activity_Conflict<MT>>& conflict_list)
			{
				// no conflict if original_activity doesn't exist
				if (original_activity == nullptr) return false;

				Activity_Plan* conflicting = (Activity_Plan*)conflicting_activity;
				Activity_Plan* original = (Activity_Plan*)original_activity;

				// start defining the conflict
				Implementations::Activity_Conflict<MT> conflict;				
				conflict.template conflicting_activity<Activity_Plan*>(conflicting);
				conflict.template original_activity<Activity_Plan*>(original);
				
				// determine the conflict type and overlap percentage
				if (original->template Start_Time<Time_Seconds>() <= conflicting->template Start_Time<Time_Seconds>() && original->template End_Time<Time_Seconds>() >= conflicting->template End_Time<Time_Seconds>())
				{
					conflict.conflict_type = Types::CONFLICT_TYPES::INSERT;
					conflict.percent_overlap = conflicting->template Duration<Time_Seconds>() / original->template Duration<Time_Seconds>();
				}
				else if (original->template Start_Time<Time_Seconds>() >= conflicting->template Start_Time<Time_Seconds>() && original->template End_Time<Time_Seconds>() <= conflicting->template End_Time<Time_Seconds>())
				{
					conflict.conflict_type = Types::CONFLICT_TYPES::OVERLAP_ALL;
					conflict.percent_overlap = 1.0;
				}
				else if (original->template Start_Time<Time_Seconds>() >= conflicting->template Start_Time<Time_Seconds>() && original->template End_Time<Time_Seconds>() >= conflicting->template End_Time<Time_Seconds>())
				{
					conflict.conflict_type = Types::CONFLICT_TYPES::OVERLAP_START;
					conflict.percent_overlap = (conflicting->template End_Time<Time_Seconds>() - original->template Start_Time<Time_Seconds>()) / original->template Duration<Time_Seconds>();
				}
				else if (original->template Start_Time<Time_Seconds>() <= conflicting->template Start_Time<Time_Seconds>() && original->template End_Time<Time_Seconds>() <= conflicting->template End_Time<Time_Seconds>())
				{
					conflict.conflict_type = Types::CONFLICT_TYPES::OVERLAP_END;
					conflict.percent_overlap = (original->template End_Time<Time_Seconds>() - conflicting->template Start_Time<Time_Seconds>()) / original->template Duration<Time_Seconds>();
				}
				else
				{
					THROW_EXCEPTION("ERROR: indeterminate conflict type found, should not reach this point.");
				}

				// push to conflict list
				conflict_list.push_back(conflict);
			}
		};

	}

}
