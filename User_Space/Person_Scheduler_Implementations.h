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
		implementation struct General_Person_Scheduler_Implementation : public Polaris_Component<APPEND_CHILD(General_Person_Scheduler_Implementation),MasterType,Execution_Object,ParentType>
		{
			// Tag as implementation
			typedef typename Polaris_Component<APPEND_CHILD(General_Person_Scheduler_Implementation),MasterType,Execution_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			member_prototype(Person_Components::Prototypes::Person, Parent_Person, typename MasterType::person_type, none, none);

			//Containers for activity planning events and movement planning events
			member_container(list<typename MasterType::activity_type*>,Activity_Container,none,none);
			member_container(list<typename MasterType::movement_plan_type*>,Movement_Plans_Container,none,none);

			// Interface definitions
			define_component_interface(_Scenario_Interface, typename type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_Network_Interface, typename type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_Skim_Interface, typename _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_Activity_Locations_Container_Interface, _Activity_Location_Interface, typename _Network_Interface::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, typename _Activity_Location_Interface::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan, type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(Movement_Plans,Movement_Plan, type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			define_container_and_value_interface(Activity_Records,Activity_Record, typename type_of(Parent_Person)::type_of(Activity_Record_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			
			// member features
			feature_implementation void Initialize()
			{

			} tag_feature_as_available(Initialize);

			// scheduling features - move to Person_Scheduler eventually
			feature_implementation TargetType current_movement_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class			
				Movement_Plans* movement_plans = this->template Movement_Plans_Container<ComponentType,CallerType, Movement_Plans*>();
				typename Movement_Plans::iterator itr;
				if ((itr = movement_plans->find(_iteration)) != movement_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_movement_plan);
			feature_implementation TargetType current_activity_plan(requires(check_as_given(TargetType,is_pointer) && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)))
			{
				// Define interfaces to the container members of the class
				Activity_Plans* activity_plans = this->template Activity_Container<Activity_Plans*>();
				typename Activity_Plans::iterator itr;
				if ((itr = activity_plans->find(_iteration)) != activity_plans->end()) return (TargetType)*itr;
				else return NULL;
			}
			tag_feature_as_available(current_activity_plan);
			feature_implementation typename TargetType::ReturnType previous_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<ComponentType,CallerType,Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
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
			feature_implementation typename TargetType::ReturnType next_activity_plan(typename TargetType::ParamType current_time)
			{
				Activity_Plans* activity_plans = this->template Activity_Container<ComponentType,CallerType,Activity_Plans*>();
				typename Activity_Plans::iterator itr;

				// convert current time to seconds
				Time_Seconds start_time = GLOBALS::Time_Converter.Convert_Value<Target_Type<NT,Time_Seconds,typename TargetType::ParamType>>(current_time);
				Time_Seconds min_next = END;
				Activity_Plan* act;
				Activity_Plan* next = nullptr;
				
				// search for the closest preceeding activity
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

			feature_implementation typename TargetType::ReturnType previous_location(typename TargetType::ParamType current_activity)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Parent_Person_interface* person = (Parent_Person_interface*)_Parent_Person;

				// if the start time of the activity has not been planned set the previous location to home
				if (!act->template Start_Is_Planned<bool>()) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				Time_Seconds start = act->template Start_Time<Time_Seconds>();

				Activity_Plan* prev_act = this->previous_activity_plan<ComponentType,CallerType,Target_Type<NT,Activity_Plan*,Time_Seconds>>(act->template Start_Time<Time_Seconds>());

				// if no previous activity, person is at home
				if (prev_act == nullptr) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();

				// if previous act location is not planned treat person as if at home
				if (!prev_act->template Location_Is_Planned<bool>()) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();

				// otherwise, determine if person is at home or still at a previous activity
				_Activity_Location_Interface* orig = prev_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* dest = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if current location is in the process of being planned, use previous location as only deflected time matters
				if (dest == nullptr) return (typename TargetType::ReturnType)orig;

				// check if a stop at home will fit prior to activity
				Time_Seconds time_before = start - (prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>());
				Time_Seconds ttime_prev_to_home = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_Zone_Interface*>()->template uuid<int>(), _Parent_Person->template Home_Location<_Zone_Interface*>()->template uuid<int>(),SOV,start);
				Time_Seconds ttime_home_to_this = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(_Parent_Person->template Home_Location<_Zone_Interface*>()->template uuid<int>(),dest->template zone<_Zone_Interface*>()->template uuid<int>(), SOV,start);
				Time_Seconds ttime_prev_to_this = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_Zone_Interface*>()->template uuid<int>(), dest->template zone<_Zone_Interface*>()->template uuid<int>(),SOV,start);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = std::min((float)ttime_prev_to_home,(float)ttime_home_to_this);			
				if (ttime_prev_to_home + ttime_home_to_this < time_before - min_home_time) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				return (typename TargetType::ReturnType)orig;
			}
			tag_feature_signature_as_available(previous_location,1);
			feature_implementation typename TargetType::ReturnType next_location(typename TargetType::ParamType current_activity)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();

				// if the start time of the activity has not been planned set the next location to home
				if (!act->template Start_Is_Planned<bool>()) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

				Activity_Plan* next_act = this->next_activity_plan<ComponentType,CallerType,Target_Type<NT,Activity_Plan*,Time_Seconds>>(act->template Start_Time<Time_Seconds>());

				// if no next activity, person goes home
				if (next_act == nullptr) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();

				// if next act location is not planned treat person as if returning home
				if (!next_act->template Location_Is_Planned<bool>()) return _Parent_Person->template Home_Location<typename TargetType::ReturnType>();

				// otherwise, determine if person goes home or to next activity
				_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
				_Activity_Location_Interface* orig = act->template Location<_Activity_Location_Interface*>();

				// if current act location is not planned, treat current location as at home
				if (!act->template Location_Is_Planned<bool>()) orig = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// if act is currently being planned (i.e. is_planned is true, but location pointer is null) then use next location
				if (orig == nullptr) return (typename TargetType::ReturnType)dest;

				// check if a stop at home will fit prior to activity
				Time_Seconds time_after = next_act->template Start_Time<Time_Seconds>() - end;
				Time_Seconds ttime_this_to_home = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_Zone_Interface*>()->template uuid<int>(), _Parent_Person->template Home_Location<_Zone_Interface*>()->template uuid<int>(),SOV,end);
				Time_Seconds ttime_home_to_next = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(_Parent_Person->template Home_Location<_Zone_Interface*>()->template uuid<int>(),dest->template zone<_Zone_Interface*>()->template uuid<int>(), SOV,end);
				Time_Seconds ttime_this_to_next = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Type_Keys, Time_Seconds>>(orig->template zone<_Zone_Interface*>()->template uuid<int>(), dest->template zone<_Zone_Interface*>()->template uuid<int>(),SOV,end);
				// enough time between previous activity and this activity to go home, stay there for a minimimum amount of time (equal to the shortest leg of the return home trip) and get to this activity
				float min_home_time = std::min((float)ttime_this_to_home,(float)ttime_home_to_next);			
				if (ttime_this_to_home + min_home_time + ttime_home_to_next  < time_after) dest = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();

				// return the expected origin
				return (typename TargetType::ReturnType)dest;
			}
			tag_feature_signature_as_available(next_location,1);

			// Simplified timing conflict resolution - only modifying the existing activity
			feature_implementation bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans)
			{
				Activity_Plan* act = (Activity_Plan*)current_activity;
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				Movement_Plan* move = act->movement_plan<Movement_Plan*>();

				// if the start time of the activity has not been planned no way to identify timing conflicts
				if (!act->template Start_Is_Planned<bool>()) return false;

				// else get the start and end times for the activity
				Time_Seconds start = act->template Start_Time<Time_Seconds>();
				Time_Seconds end = act->template Start_Time<Time_Seconds>() + act->template Duration<Time_Seconds>();

				// get the activity location for calculating approximate travel times to from surrounding activities
				_Activity_Location_Interface* loc = _Parent_Person->template Home_Location<_Activity_Location_Interface*>();
				if (act->template Location_Is_Planned<bool>()) loc = act->template Location<_Activity_Location_Interface*>();

				// Get the surrounding activities in the schedule
				Activity_Plan* prev_act = this->previous_activity_plan<ComponentType,CallerType,Target_Type<NT,Activity_Plan*,Time_Seconds>>(start);
				Activity_Plan* next_act = this->next_activity_plan<ComponentType,CallerType,Target_Type<NT,Activity_Plan*,Time_Seconds>>(start);

				//=======================================================================
				// find maximum end time of the new activity
				Time_Seconds end_max;
				if (next_act == nullptr) end_max = END;
				else
				{
					// if next activity location is known, get the expected travel time from current to next, else assume 20 minutes
					Time_Seconds ttime;
					_Activity_Location_Interface* next_loc;
					if (next_act->template Location_Is_Planned<bool>())
					{
						next_loc = next_act->template Location<_Activity_Location_Interface*>();
						ttime = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(loc->template zone<_Zone_Interface*>()->template uuid<int>(), next_loc->template zone<_Zone_Interface*>()->template uuid<int>(),Vehicle_Components::Types::Vehicle_Type_Keys::SOV, end);
					}
					else ttime = 20.0 * 60.0;

					// get maximum end time of current activity given departure time for next activity
					end_max = next_act->template Start_Time<Time_Seconds>() - ttime;
				}

				//=======================================================================
				// find minimum start time of the new activity
				Time_Seconds start_min;
				if (prev_act == nullptr) start_min = _iteration+1;
				else
				{
					// if next activity location is known, get the expected travel time from current to next, else assume 20 minutes
					Time_Seconds ttime;
					_Activity_Location_Interface* prev_loc;
					if (prev_act->template Location_Is_Planned<bool>())
					{
						prev_loc = prev_act->template Location<_Activity_Location_Interface*>();
						ttime = network->template Get_LOS<Target_Type<NT,Time_Seconds,int,Vehicle_Components::Types::Vehicle_Type_Keys,Time_Seconds>>(prev_loc->template zone<_Zone_Interface*>()->template uuid<int>(),loc->template zone<_Zone_Interface*>()->template uuid<int>(), Vehicle_Components::Types::Vehicle_Type_Keys::SOV, start);
					}
					else ttime = 20.0 * 60.0;

					// get maximum end time of current activity given departure time for next activity
					start_min = prev_act->template Start_Time<Time_Seconds>() + prev_act->template Duration<Time_Seconds>() + ttime;
				}

				// determine conflict regimes and modify appropriately
				Time_Seconds available_time = end_max - start_min;

				//cout << endl <<"CONFLICT: min_start="<<start_min<<", max_end="<<end_max<<", act start/end="<<start <<","<<end;

				// 1. simple conflict, fits without shortening
				if (available_time > end - start)
				{
					//cout << ", resolution type: MOVE"<<endl;
					if (start < start_min) act->template Start_Time<Time_Seconds>(start_min); // move forward in time
					if (end > end_max) act->template Start_Time<Time_Seconds>(start - (end-end_max)); // move backward in time
					if (update_movement_plans) move->departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - act->template Expected_Travel_Time<Time_Seconds>());
					return true;
				}
				// 2. complex conflict, move and shorten
				else if (available_time > 0.5*(end-start) )
				{
					//cout << ", resolution type: MOVE AND SHORTEN"<<endl;
					act->template Start_Time<Time_Seconds>(start_min);
					act->template Duration<Time_Seconds>(end_max-start_min);
					if (update_movement_plans) move->departed_time<Time_Seconds>(act->template Start_Time<Time_Seconds>() - act->template Expected_Travel_Time<Time_Seconds>());
					return true;
				}
				// 3. can't fit, delete activity
				else
				{
					//cout << endl <<"CONFLICT: min_start="<<start_min<<", max_end="<<end_max<<", act start/end="<<start <<","<<end << ", resolution type: DELETE CONFLICTING"<<" -- ";
					// output previous act
					/*if (prev_act != nullptr)
					{
						cout << "Prev act start=" << prev_act->Start_Time<Time_Seconds>() <<", prev act end="<<prev_act->End_Time<Time_Seconds>();
					}
					else cout << "Prev act null, start at :"<<_iteration;
					if (next_act != nullptr)
					{
						cout << ",  Next act start=" << next_act->Start_Time<Time_Seconds>() <<", next act end="<<next_act->End_Time<Time_Seconds>()<<endl;
					}
					else cout << ",  Next act null, end at :"<<END;*/
					return false;
				}

				
			}
			tag_feature_signature_as_available(Resolve_Timing_Conflict,1);


			// Adding activities and movements to the planning schedules
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(check_as_given(TargetType,is_pointer) && check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				// define interfaces
				typename Movement_Plans::iterator move_itr;

				// get interface to the movement plan to be added
				Movement_Plan* move = (Movement_Plan*)movement_plan;

				// catch skipped movement plans
				if (move->template departed_time<Simulation_Timestep_Increment>() < _iteration) return;

				// Insert new movement plan at the appropriate space in plan schedule
				Movement_Plans* movements = this->template Movement_Plans_Container<ComponentType,CallerType,Movement_Plans*>();
				move_itr = movements->begin();
				while(move_itr != movements->end())
				{
					Movement_Plan* current_move = *move_itr;
					if (current_move->template departed_time<Simulation_Timestep_Increment>() > move->template departed_time<Simulation_Timestep_Increment>()) break;
					++move_itr;
				}
				movements->insert(move_itr,move);	
				Activity_Plan* act = move->template destination_activity_reference<Activity_Plan*>();

				// modify the origin for the following movement plan
				if (move_itr != movements->end())
				{
					Movement_Plan* next_move = *move_itr;				
					Activity_Plan* next_act = next_move->template destination_activity_reference<Activity_Plan*>();
					// update the movement plan to next act
					_Activity_Location_Interface* orig = this->previous_location<ComponentType,CallerType,Target_Type<NT,_Activity_Location_Interface*,Activity_Plan*>>(next_act);
					_Activity_Location_Interface* dest = next_act->template Location<_Activity_Location_Interface*>();
					Simulation_Timestep_Increment end = act->template Start_Time<Simulation_Timestep_Increment>() + act->template Duration<Simulation_Timestep_Increment>();
					next_act->Update_Movement_Plan<_Activity_Location_Interface*>(orig,dest,end);
				}

				// check for timing conflicts again, after updated movement plans
				this->Resolve_Timing_Conflict<ComponentType,CallerType,Activity_Plan*>(act,true);

				// log the activity throught the global person logger
				//define_component_interface(_Logger_Interface, typename MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
				
				//((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity_Plan*>(act,false);

				// no logging if the movement is a return home movement
				if (act == nullptr) return;

				// cache the movement plans activity in the activity record container since all attributes have been planned at this point			
				Activity_Records* act_records = _Parent_Person->template Activity_Record_Container<Activity_Records*>();
				Activity_Record* act_record = (Activity_Record*)Allocate<typename MasterType::activity_record_type>();
				act_record->template Initialize<Activity_Plan*>(act);
				act_records->push_back(act_record);
				
			}
			feature_implementation void Add_Movement_Plan(TargetType movement_plan, requires(!check_as_given(TargetType,is_pointer) || !check(TargetType,Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype)))
			{
				assert_check_as_given(TargetType,is_pointer, "Error, TargetType must be passed as a pointer");
				assert_check(TargetType, Movement_Plan_Components::Concepts::Is_Movement_Plan_Prototype, "Error, Function requires TargetType to be a Movement_Plan_Prototype.");
			}
			tag_feature_as_available(Add_Movement_Plan);
			
			feature_implementation void Add_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer)/* && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{
				//define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<ComponentType,CallerType,stringstream&>(s);
				//}

				Activity_Plans* activities = this->Activity_Container<ComponentType,CallerType,Activity_Plans*>();		
				activities->push_back(act);
			}
			feature_implementation void Remove_Activity_Plan(TargetType activity_plan, requires(check_as_given(TargetType,is_pointer)/* && check(TargetType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/))
			{
				//define_container_and_value_interface_unqualified_container(Activity_Plans,Activity_Plan,type_of(Activity_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Plan,ComponentType);	
				Activity_Plan* act = (Activity_Plan*)activity_plan;

				// print to log file if requested
				//if (_write_activity_files) 
				//{			
				//	stringstream s;	
				//	s << endl << "ACTIVITY GEN (PERID.ACTID.ACTTYPE)," << _Parent_Person->template uuid<int>() << "," << act->template Activity_Plan_ID<int>() << ", " << act->template Activity_Type<ACTIVITY_TYPES>();
				//	this->Write_To_Log<ComponentType,CallerType,stringstream&>(s);
				//}

				Activity_Plans* activities = this->Activity_Container<ComponentType,CallerType,Activity_Plans*>();		
				
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
		
		};

	}

}