#pragma once

#pragma push_macro("check")
#undef check
#include "gurobi_c++.h"
#pragma pop_macro("check")


#include <numeric>
#include "IntraHousehold_AV_Assignment_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Household_Prototype.h"
#include "Demand.h"

//Timer related
#include <iostream>
#include <cstdio>
#include <ctime>

//#define Write_Visualization_Files
#define  Prevent_Write_to_Console
#define  Ignore_Taxi
#define  Ignore_RideSharing
#define  Ignore_None_Auto_Modes
//#define Debug_Intrahousehold_Vehicle_Assignment

#pragma region Setting_prev_and_Next_Acts

#define Setting_prev_and_Next_Acts( a )\
per##a##_activity = *per##a##_act_itr; \
per##a##_strActID = GetstrActID(per##a##_activity, act_ID_map, per##a##); \
auto per##a##_act_itr_next = std::next(per##a##_act_itr); \
if (per##a##_act_itr_next != per##a##_activities->end()) \
{ \
	per##a##_activity_next = *per##a##_act_itr_next; \
	per##a##_strActID_next = GetstrActID(per##a##_activity_next, act_ID_map, per##a##); \
} \
else \
{ \
	per##a##_activity_next = nullptr; \
	per##a##_strActID_next = ""; \
} \
if (per##a##_activity != *(per##a##_activities->begin())) \
{ \
	per##a##_activity_prev = *(std::prev(per##a##_act_itr)); \
	per##a##_strActID_prev = GetstrActID(per##a##_activity_prev, act_ID_map, per##a##); \
} \
else \
{ \
	per##a##_activity_prev = nullptr; \
	per##a##_strActID_prev = ""; \
}
#pragma endregion


namespace Household_Components
{
	namespace Implementations
	{

		implementation struct IntraHousehold_AV_Assignment_Implementation : public Polaris_Component<MasterType, INHERIT(IntraHousehold_AV_Assignment_Implementation), Data_Object>
		{					
			static double min_travel_time;
			static double min_act_dur;
			static double min_start_flex;
			static double min_dur_flex;
			static double cost_AV_fixed;				//fixed cost of each AV in the system
			static double cost_parking;		//$0.5 per hour
			static double cost_AV_energy;		//Cost of AV : $5 per hour(0.0833 per minute)
			static double cost_vot;				//Person time cost : $10 / hr($0.167 / min) When changes to the activity start and duration is made
			static double cost_taxi_fixed;			//#cost of taxi : $2
			static double cost_taxi_by_minute;	//#$24 per hour($0.4 per minute)
			static double cost_ZOV_tax;		//  #$1 per hour
			static double cost_ZOV_fixed_tax;

			static int counter_solved;
			static int counter_timedout;
			static int counter_error;
		
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(IntraHousehold_AV_Assignment_Implementation), Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);

				
			//typedef typename get_component_type(_Persons_Container_Interface) _Person_type;

			typedef Household_Components::Prototypes::Household<typename type_of(Parent_Household)> _household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename _household_itf::get_type_of(Static_Properties)> _household_static_properties_itf;


			typedef Random_Access_Sequence< typename _household_itf::get_type_of(Persons_Container)> _persons_container_itf;
			typedef Person_Components::Prototypes::Person<typename get_component_type(_persons_container_itf)> _person_itf;
			//typedef Person_Components::Prototypes::Person_Planner<Parent_Planner_component_type> _planning_itf;
			
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Static_Properties)> _person_static_properties_itf;

			typedef Person_Components::Prototypes::Person_Scheduler<typename _person_itf::get_type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef Random_Access_Sequence< typename _scheduler_itf::get_type_of(Activity_Container)> _activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(_activity_container_itf)>  _activity_itf;
			
			typedef Person_Components::Prototypes::Person_Planner<typename _activity_itf::get_type_of(Parent_Planner)> _planning_itf;

			typedef Network_Components::Prototypes::Network< typename _household_itf::get_type_of(network_reference)> _network_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> _activity_location_itf;

			//typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _movement_plan_itf;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _movement_plan_itf;

			//typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _scenario_itf;

			//typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			//typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;

			//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			//typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;


			//typedef typename _scheduler_itf::get_type_of(Activity_Container) _Actvity_Container_itf;
			//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
			

			//person_itf* _Parent_Person = base_type::_Parent_Planner->template Parent_Person<person_itf*>();

			//scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();

			//typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
			//typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(_Vehicles_Container_Interface)> _Vehicle_Interface;
			//typedef typename get_component_type(_Vehicles_Container_Interface) _Vehicle_type;

			// Interface definitions	
			

			template<typename T> void Initialize(T household)
			{
				this->Parent_Household(household);
				//_Vehicle_Technology_Chooser = (Vehicle_Technology_Chooser_type)Allocate<type_of(Vehicle_Technology_Chooser)>();
				//_Vehicle_Technology_Chooser->Initialize(household);												
			}

			int Get_Max_Number_of_AVs()
			{
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				float HH_size = household_properties->Household_size<float>();
				return HH_size;

				int maxVehicles = 0;
				_persons_container_itf* persons = _Parent_Household->Persons_Container<_persons_container_itf*>();
				for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
				{
					auto per = (*per_itr);
					_person_static_properties_itf* per_properties = per->Static_Properties<_person_static_properties_itf*>();
					float age = per_properties->Age<float>();
					if (age > 6) 
					{
						maxVehicles++ ;
					}
				}
				return maxVehicles;
				
				//float male = per1_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
			}

			string GetstrActID(_activity_itf* activity, map<pair<long, int>, string>& act_ID_map, _person_itf* per)
			{
				auto key = make_pair(per->uuid<long>(), activity->Activity_Plan_ID<int>());
				return  act_ID_map[key];
			}

			GRBVar* Add_New_Binary_Variable(map<string, GRBVar>& grb_Vars, GRBModel& model, string strVarName)
			{				
				//if (model.get(GRB_IntAttr_NumVars) != grb_Vars.size() )
				//{
				//	cout << "Investigate this: Gorubi: " << model.get(GRB_IntAttr_NumVars)  << "   Dictionary" << grb_Vars.size() << endl;
				//}

				GRBVar new_var = model.addVar(0, 1, 0.0, GRB_BINARY, strVarName);
				grb_Vars.insert(std::pair<string, GRBVar>(strVarName, new_var));				

				//if (model.get(GRB_IntAttr_NumVars) != grb_Vars.size())
				//{
				//	cout << "Investigate this: Gorubi: " << model.get(GRB_IntAttr_NumVars) << "   Dictionary" << grb_Vars.size() << endl;
				//}
				return &new_var;
			}

			void Add_New_Variable(map<string, GRBVar>& grb_Vars, GRBModel& model, string strVarName, char vType)
			{
				
				GRBVar new_var;
				if (vType == GRB_CONTINUOUS)
				{
					auto result = grb_Vars.insert(std::pair<string, GRBVar>(strVarName, model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, strVarName)));
					//return (*(result.first)).second;
				}
				else if (vType == GRB_BINARY)
				{
					auto result = grb_Vars.insert(std::pair<string, GRBVar>(strVarName, model.addVar(0, 1, 0.0, GRB_BINARY, strVarName)));
					//(*(result.first)).second;
				}
				else
				{
					//return model.addVar(0, 1, 0.0, GRB_BINARY, strVarName);
					model.addVar(0, 1, 0.0, GRB_BINARY, strVarName);
				}
			}

			Time_Minutes  Get_Start_lb(_activity_itf* activity)
			{				
				//if (activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY)
				//{
				//	return  max(0.0, activity-> template Start_Time<Time_Minutes>().Value - min_start_flex);
				//}
				//else
				{
					return  max(0.0, activity-> template Start_Time<Time_Minutes>().Value - min_start_flex);
				}
			}

			Time_Minutes  Get_Start_ub(_activity_itf* activity)
			{
				return  activity-> template Start_Time<Time_Minutes>().Value + min_start_flex;
			}

			Time_Minutes  Get_Duration_lb(_activity_itf* activity)
			{
				return max(min_act_dur, activity-> template End_Time<Time_Minutes>().Value - activity-> template Start_Time<Time_Minutes>().Value - min_dur_flex);
			}

			Time_Minutes  Get_Duration_ub(_activity_itf* activity)
			{
				return max(min_act_dur, activity-> template End_Time<Time_Minutes>().Value - activity-> template Start_Time<Time_Minutes>().Value + min_dur_flex);
			}

			Time_Minutes  Get_End_lb(_activity_itf* activity)
			{
				return  Get_Start_lb(activity) + Get_Duration_lb(activity);
			}

			Time_Minutes  Get_End_ub(_activity_itf* activity)
			{
				return  Get_Start_ub(activity) + Get_Duration_ub(activity);
			}

			bool Home_Activity(_activity_itf* activity)
			{
				if (activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY ||
					activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
					return true;
				else 
					return false;
			}

			//This tries to calculate the absolute shift for a variable to be used in the objective function
			void Add_Absolute(map<string, GRBVar>& grb_Vars, GRBModel& model, _activity_itf& activity, string strVarName, double value, GRBVar& var, GRBLinExpr& Obj_LinExpr, string strActID)
			{				
				string varName = strVarName + "_" + strActID + "_abs";				
				GRBVar new_var1 = model.addVar(0, 1440.0, 0.0, GRB_CONTINUOUS, varName);
				grb_Vars.insert(std::pair<string, GRBVar>(varName, new_var1));

				varName = strVarName + "_" + strActID + "_absBi";
				GRBVar new_var2 = model.addVar(0, 1, 0.0, GRB_BINARY, varName);
				grb_Vars.insert(std::pair<string, GRBVar>(varName, new_var2));

				varName = strVarName + "_" + strActID + "_absI1";
				GRBVar new_var3 = model.addVar(0, 1440.0, 0.0, GRB_CONTINUOUS, varName);
				grb_Vars.insert(std::pair<string, GRBVar>(varName, new_var3));

				varName = strVarName + "_" + strActID + "_absI2";
				GRBVar new_var4 = model.addVar(0, 1440.0, 0.0, GRB_CONTINUOUS, varName);
				grb_Vars.insert(std::pair<string, GRBVar>(varName, new_var4));

				//p <= M*y and n <= M * (1 - y) w
				model.addConstr(var - value == new_var3 - new_var4);
				model.addConstr(new_var3 <= 1440.0 * new_var2);
				model.addConstr(new_var4 <= 1440.0 * (1 - new_var2 ));
				
				model.addConstr(new_var1 == new_var3 + new_var4);
				
				Obj_LinExpr += cost_vot * new_var1;
			}

			string Get_VehID(string term)
			{
				
				return "";
			}

			string Get_Origin_ActID(string term)
			{
				
				return "";
			}

			string Get_Destination_ActID(string term)
			{

				return "";
			}
			
			Time_Minutes Get_Travel_Time(_activity_location_itf* origin_loc, _activity_location_itf* destination_loc, Time_Minutes start)
			{
				_network_itf* network = _Parent_Household->template network_reference<_network_itf*>();				
				Time_Minutes ttime_origin_to_destination = network->template Get_TTime<_activity_location_itf*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(origin_loc, destination_loc, SOV, start);
				return ttime_origin_to_destination;
			}
			
			Time_Minutes Get_Travel_Time(_activity_itf* origin_act, _activity_itf* destination_act, Time_Minutes start)
			{
				_network_itf* network = _Parent_Household->template network_reference<_network_itf*>();
				//Time_Minutes start = origin_act->Start_Time<Time_Minutes>();
				_activity_location_itf* origin_loc = origin_act->Location<_activity_location_itf*>();
				_activity_location_itf* destination_loc = destination_act->Location<_activity_location_itf*>();
				Time_Minutes ttime_origin_to_destination = network->template Get_TTime<_activity_location_itf*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(origin_loc, destination_loc, SOV, start);
				return ttime_origin_to_destination;

				//typedef Network_Components::Prototypes::Network<type_of(network_reference)> network_itf;
				//typedef Prototypes::Network_Skimming<type_of(skim_reference)> skimmer_itf;
				//typedef Zone_Components::Prototypes::Zone<typename remove_pointer<typename network_itf::get_type_of(zones_container)::mapped_type>::type> zone_itf;
				//typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> zones_itf;
				//typename MasterType::activity_location_type
				//typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
				//typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations), location_itf*> locations_itf;


				//typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
				//typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;

				//Time_Minutes ttime_prev_to_home = network->template Get_TTime<_Activity_Location_Interface*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(orig, _Parent_Person->template Home_Location<_Activity_Location_Interface*>(), SOV, start);
				//Time_Minutes ttime_home_to_this = network->template Get_TTime<_Activity_Location_Interface*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(), dest, SOV, start);
				//Time_Minutes ttime_prev_to_this = network->template Get_TTime<_Activity_Location_Interface*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(orig, dest, SOV, start);

				//Time_Minutes ttime_this_to_next = network->template Get_TTime<_Activity_Location_Interface*, MODE, Time_Minutes, Time_Minutes>(current->template Location<_Activity_Location_Interface*>(), next_act->template Location<_Activity_Location_Interface*>(), MODE::SOV, end_this);
				//Time_Minutes ttime_home_to_next = network->template Get_TTime<_Activity_Location_Interface*, MODE, Time_Minutes, Time_Minutes>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(), next_act->template Location<_Activity_Location_Interface*>(), MODE::SOV, end_this + ttime_this_to_home);


				//// interface to destination zone
				//zone_itf* dzone = (zone_itf*)(d_zone_itr->second);

				//// do not include the origin zone in calculations
				//if (zone == dzone) continue;

				//// get TTime by automobile and transit through skim interface, in minutes
				//Time_Minutes ttime_auto_ampeak_max = skim->template Get_TTime<zone_itf*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(zone, dzone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 7.5);
			}

			vector<_movement_plan_itf* >*  Assign_Shared_Vehicles()
			{												
				_scenario_itf* scenario = _Parent_Household->template scenario_reference<_scenario_itf*>();
				string folder_name = scenario->template output_dir_name<string>() + "\\ZOV";
#ifdef _WIN32		
				_mkdir(folder_name.c_str());
				//while (_mkdir(temp_dir_name.c_str()) == -1)
#else
				mkdir(folder_name.c_str());
				//while (mkdir(temp_dir_name.c_str(), 0777) == -1)
#endif

				_network_itf* network = this->_Parent_Household->network_reference<_network_itf*>();
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				//auto HHID = household_properties->ID<float>();
				auto HHID = this->_Parent_Household->uuid<long long>();
				float HH_size = household_properties->Household_size<float>();

				std::clock_t start;
				double duration;
				start = std::clock();

				map<string, _activity_itf*> act_String_map;
				map<pair<long,int>, string> act_ID_map;
				int act_counter = 0;
				map<string, GRBVar> grb_Vars;

				_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();
				_activity_itf *per1_activity, *per1_activity_next, *per1_activity_prev;
				_activity_itf *per2_activity, *per2_activity_next, *per2_activity_prev;
				bool firstAct, lastAct;
				string per1_strActID, per1_strActID_next, per1_strActID_prev;
				string per2_strActID, per2_strActID_next, per2_strActID_prev;
				string strVarName, strVehID;

				try
				{
					GRBEnv env = GRBEnv();
					GRBModel model = GRBModel(env);
#pragma region gurobi settings
#ifdef Prevent_Write_to_Console
					model.getEnv().set(GRB_IntParam_OutputFlag, 0); //0- disables writing to the console. 1- enables writing to the console.
#endif
					model.set(GRB_IntParam_Threads, 1); //Set hte number of threads.
					model.set(GRB_DoubleParam_MIPGap, 0.2); //set the time limit to 2 seconds
					model.set(GRB_DoubleParam_TimeLimit, 5.0); //set the time limit to 2 seconds

					//version 7:   -1=automatic, 0=primal simplex, 1=dual simplex, 2=barrier, 3=concurrent, 4=deterministic concurrent.
					//version 7.5: -1 = automatic, 0 = primal simplex, 1 = dual simplex, 2 = barrier, 3 = concurrent, 4 = deterministic concurrent, 5 = deterministic
					model.set(GRB_IntParam_Method, -1); 

					//model.set(GRB_IntParam_MIPFocus, 1); 
					//model.set(GRB_IntParam_Cuts, 1); 
					//model.set(GRB_IntParam_CliqueCuts, 2); 
#pragma endregion
					
#pragma region Generating new string IDs for activities and creating a map
					for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
					{
						auto per = (*per_itr);
						_scheduler_itf* per_scheduler = per->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per_activities = per_scheduler->Activity_Container<_activity_container_itf*>();
						per_scheduler->Sort_Activity_Schedule<void>();
						for (auto per_act_itr = per_activities->begin(); per_act_itr != per_activities->end(); per_act_itr++)
						{
							auto per_activity = *per_act_itr;
							int act_id = per_activity->Activity_Plan_ID<int>();
							string per_strActID = act_counter >= 100 ? to_string(act_counter) : act_counter >= 10 ? "0" + to_string(act_counter) : "00" + to_string(act_counter);
							std::pair<long, int> key = std::make_pair(per->uuid<long>(), act_id );
							act_ID_map.insert(std::pair<std::pair<long, int>, string>(key, per_strActID));
							
							act_String_map.insert(std::pair<string, _activity_itf *>(per_strActID, per_activity));

							act_counter++;
						}
					}
#pragma endregion

					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						//float age = per1_properties->Age<float>();
						//float male = per1_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
						//cout << "age = " << age << endl;

						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();
						//per1->Sort_Activity_Records<void>();
						
						//for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						//{
						//	per1_activity = *per1_act_itr;
						//	int act_id = per1_activity->Activity_Plan_ID<int>();
						//	string per1_strActID = act_counter > 100 ? to_string(act_counter) : act_counter > 10 ? "0" + to_string(act_counter) : "00" + to_string(act_counter);
						//	act_ID_map.insert(std::pair<int, string>(act_id, per1_strActID));
						//	act_counter++;
						//}

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							Setting_prev_and_Next_Acts ( 1 );
							//Movement_Plans* movements = scheduler->template Movement_Plans_Container<Movement_Plans*>();
							//cout << "Start_Time = " << pActivity1->template Start_Time<Time_Minutes>() << endl;

							#pragma region Start and duration of activy  
							//defining start and end of activity and lower and upper bounds of start(lowe bounds are set in activity class construction)
														double start_lb = max(0.0, per1_activity->template Start_Time<Time_Minutes>().Value - min_start_flex);
														double start_ub = per1_activity-> template Start_Time<Time_Minutes>().Value + min_start_flex;

														//constraint duration of activity
														double duration_lb = max(min_act_dur, per1_activity-> template End_Time<Time_Minutes>().Value - per1_activity-> template Start_Time<Time_Minutes>().Value - min_dur_flex);
														double duration_ub = max(min_act_dur, per1_activity-> template End_Time<Time_Minutes>().Value - per1_activity-> template Start_Time<Time_Minutes>().Value + min_dur_flex);

														strVarName = "start_" + per1_strActID;
														//GRBVar start = model.addVar(start_lb, start_ub, 0.0, GRB_CONTINUOUS, strVarName);
														//grb_Vars.insert(std::pair<string, GRBVar*>(strVarName, &start));

														//auto start = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														auto start = grb_Vars[strVarName];
														model.addConstr(start >= start_lb, "c_start_lb_" + per1_strActID);
														model.addConstr(start <= start_ub, "c_start_ub_" + per1_strActID);

														strVarName = "end_" + per1_strActID;
														//auto end = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														auto end = grb_Vars[strVarName];
														model.addConstr(end >= start_lb, "c_end_lb_" + per1_strActID);
														model.addConstr(end <= start_ub + duration_ub, "c_end_ub_" + per1_strActID);

														strVarName = "duration_" + per1_strActID;
														//auto duration = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														auto duration = grb_Vars[strVarName];
														model.addConstr(end - start == duration);

														model.addConstr(duration >= duration_lb, "c_dur_lb_" + per1_strActID);
														model.addConstr(duration <= duration_ub, "c_dur_ub_" + per1_strActID);


							#pragma endregion 

							//Adding Taxi trip from an activity to next activity
							if (per1_activity_next)
							{
								strVarName = "t_AV_00_" + per1_strActID + "_E_" + per1_strActID_next + "_S";
								Add_New_Variable(grb_Vars, model, strVarName, GRB_BINARY);
							}

							//adding a few other movements
							for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
							{
								strVehID = std::to_string(veh_id);
								if(veh_id < 10) strVehID = "0" + strVehID;

								//from parking to first Activity.
								if (per1_activity_next)
								{
									Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_Hom_S_" + per1_strActID + "_E", GRB_BINARY);
								}

								//parking at activity location variable
								if (per1_activity_next && per1_activity_prev)
								{
									Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per1_strActID + "_E", GRB_BINARY);
								}

								//Trip from one activity to another for the same person
								if (per1_activity_next)
								{
									Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per1_strActID_next + "_S", GRB_BINARY);
								}

								//exit the system and Trip from Parking
								if (per1_activity_prev)
								{
									//exit the system
									Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_Hom_E", GRB_BINARY);

									//Binary : Trip to Parking
									//Prevent going to parking for short term trips
									//if (per1_activity->Duration<Time_Minutes>() > 15  )
									{
										Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_Prk_S", GRB_BINARY);
									}
								}

								//Trip from Parking
								if (per1_activity_next)
								{
									for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
									{
										auto per2 = (*per2_itr);
										_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
										_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
										_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

										int per2_act_index = 0;
										int per2_n_acts = per2_activities->size();

										for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
										{
											Setting_prev_and_Next_Acts(2);

											//Binary: Trip from Parking
											if (per2_activity_prev)
											{
												//If trip to the parking was not generated previously, do not create a return from parking trip
												auto it = grb_Vars.find("t_AV_" + strVehID + "_" + per2_strActID + "_S_Prk_S");
												if (it == grb_Vars.end())  continue;


												auto tt = Get_Travel_Time(per2_activity, per1_activity, per2_activity->Start_Time<Time_Minutes>());
												//trying to limit the size of the problem by eliminating infeasible combinations
												if (per1->uuid<long>() == per2->uuid<long>())
												{
													if(per2_activity->Start_Time<Time_Minutes>() < per1_activity->Start_Time<Time_Minutes>() || per2_strActID == per1_strActID)
														Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per2_strActID + "SPrk_E_" + per1_strActID + "_E", GRB_BINARY);
												}
												else
												{
													if (Get_Start_lb(per2_activity) + tt.Value <= Get_End_ub(per1_activity))
													{
														Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per2_strActID + "SPrk_E_" + per1_strActID + "_E", GRB_BINARY);
													}
												}
											}
										}

									}
								}

								//Travel Between two persons' activities
								for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
								{
									auto per2 = (*per2_itr);
									_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
									_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
									_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

									if (per1->uuid<long>() == per2->uuid<long>()){continue;}

									for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
									{
										#pragma region Setting prev and next activities
										per2_activity = *per2_act_itr;
										auto per2_strActID = GetstrActID(per2_activity, act_ID_map, per2);
										auto per2_act_itr_next = std::next(per2_act_itr);

										if (per2_act_itr_next != per2_activities->end())
										{
											per2_activity_next = *per2_act_itr_next;
											per2_strActID_next = GetstrActID(per2_activity_next, act_ID_map, per2);
										}
										else
										{
											per2_activity_next = nullptr;
											per2_strActID_next = "";
										}

										if (per2_activity != *(per2_activities->begin()))
										{
											per2_activity_prev = *(std::prev(per2_act_itr));
											per2_strActID_prev = GetstrActID(per2_activity_prev, act_ID_map, per2);
										}
										else
										{
											per2_activity_prev = nullptr;
											per2_strActID_prev = "";
										}
#pragma endregion
										
										if (per1_activity_prev && per2_activity_prev )
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											if (Get_Start_lb(per1_activity) + tt.Value - 2.0 <= Get_Start_ub(per2_activity))
											{
												strVarName = "t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S";
												Add_New_Variable(grb_Vars, model, strVarName, GRB_BINARY);
#ifdef Ignore_RideSharing
												model.addConstr(grb_Vars[strVarName] == 0);
#endif
											}
										}

										if (per1_activity_prev && per2_activity_next)
										{
											strVarName = "t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E";
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											if (Get_Start_lb(per1_activity) + tt.Value - 2.0 <= Get_End_ub(per2_activity))
												Add_New_Variable(grb_Vars, model, strVarName, GRB_BINARY);
										}

										if (per1_activity_next && per2_activity_prev)
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											if (Get_End_lb(per1_activity) + tt.Value - 2.0 <= Get_Start_ub(per2_activity))
											{
												strVarName = "t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S";
												Add_New_Variable(grb_Vars, model, strVarName , GRB_BINARY);
#ifdef Ignore_RideSharing
												model.addConstr(grb_Vars[strVarName] == 0);
#endif
											}
										}

										if (per1_activity_next && per2_activity_next)
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											if (Get_End_lb(per1_activity) + tt - 2.0 <= Get_End_ub(per2_activity))
											{
												strVarName = "t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E";
												Add_New_Variable(grb_Vars, model, strVarName, GRB_BINARY);
#ifdef Ignore_RideSharing
												model.addConstr(grb_Vars[strVarName] == 0);
#endif
											}

										}
									}
								}
							}
						}
					}					

#pragma region Create variable for each vehicle moving in the system
					for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
					{
						strVehID = std::to_string(veh_id);
						if (veh_id < 10) strVehID = "0" + strVehID;
						Add_New_Variable(grb_Vars, model, "t_VE_" + strVehID, GRB_BINARY);
					}
#pragma endregion
					
#pragma region Create variable for each vehicle moving in the system
					for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
					{
						strVehID = std::to_string(veh_id);
						if (veh_id < 10) strVehID = "0" + strVehID;

						GRBLinExpr expr1 = 0;
						for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
						{
							string key = it->first;
							if (key.substr(0, 8) == "t_AV_" + strVehID + "_" && key.substr(7, 7) != "_Hom_S_" && key.substr(key.size() - 6) != "_Hom_E")
							{
								expr1 += (it->second);
							}
						}
						model.addConstr(1000.0 * (grb_Vars["t_VE_" + strVehID]) - expr1 >= 0);
					}
#pragma endregion


					//TODO:
					#pragma region create variables related to the number of passengers in each vehicle
					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						//float age = per1_properties->Age<float>();
						//float male = per1_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 1.0 : 0.0;
						//cout << "age = " << age << endl;

						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
							{
								auto per2 = (*per2_itr);
								firstAct = true;
								lastAct = false;

								_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
								//float age = per2_properties->Age<float>();
								//float male = per2_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::GENDER::MALE ? 2.0 : 0.0;
								//cout << "age = " << age << endl;

								_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
								_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

								for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
								{


									//create variables related to the number of passengers in each vehicle
									//	xyz = {}
									//	for perID_i, Per_i in HH.persons.items() :
									//		for Act_a in Per_i.activities :
									//			for perID_j, Per_j in HH.persons.items() :
									//				for Act_b in Per_j.activities :
									//					for perID_k, Per_k in HH.persons.items() :
									//						for Act_c in Per_k.activities :
									//							for veh_ID in range(1, num_AVs) :
									//								strVehID = "{0:0>2}".format(veh_ID)
									//								### from Start_A to Start_B , passenger activity C 
									//								movement = strVehID + "_" + Act_a.id + "_S_" + Act_b.id + "_S"
									//								passenger = "t_PA_" + movement + "_" + Act_c.id
									//								if "t_AV_" + movement  in AllVariables :
									//									AllVariables["t_PA_" + movement + "_" + Act_c.id] = GrVar_Act(gModel.addVar(vtype = GRB.BINARY, name = "t_PA_" + movement + "_" + Act_c.id), Act_a)
									//									xyz[passenger] = GrVar_Act(gModel.addVar(vtype = GRB.BINARY, name = passenger), Act_a)
									//
									//									xyz[passenger + "_1"] = GrVar_Act(gModel.addVar(vtype = GRB.BINARY, name = passenger + "_1"), Act_a)
									//									gModel.addConstr(AllVariables["end_" + Act_c.id].Var - AllVariables["start_" + Act_a.id].Var <= 10000 - 1000 * xyz[passenger + "_1"] - 0.0001)
									//
									//									#Start of next Activity should be greather than
									//									xyz[passenger + "_2"] = GrVar_Act(gModel.addVar(vtype = GRB.BINARY, name = passenger + "_2"), Act_a)
									//									gModel.addConstr(AllVariables["start_" + Act_b.id].Var - AllVariables["start_" + Act_c.nextAct.id].Var <= 10000 - 1000 * xyz[passenger + "_2"] - 0.0001)
									//
									//									#if the mode of this movement is the same as mode of vehicle leaving act_a, set the variable to 1
									//									xyz[passenger + "_3"] = GrVar_Act(gModel.addVar(vtype = GRB.BINARY, name = passenger + "_3"), Act_a)
									//									relatedTravelVariables = { k: v.Var for k, v in AllVariables.items() if (("t_AV_" + strVehID + "_" + Act_a.id + "_S_"  in k)) }
									//									gModel.addConstr(quicksum(value for key, value in relatedTravelVariables3.items()) + AllVariables["t_AV_" + movement].Var >= 2 * xyz[passenger + "_3"])
									//									gModel.addConstr(quicksum(value for key, value in relatedTravelVariables3.items()) + AllVariables["t_AV_" + movement].Var <= 1 + xyz[passenger + "_3"])
									//
									//									#all above 3 conditions should be met(sum == 3)
									//									gModel.addConstr(xyz[passenger + "_1"] + xyz[passenger + "_2"] + xyz[passenger + "_3"] <= 2 + AllVariables["t_PA_" + movement + "_" + Act_c.id].Var)
									//									gModel.addConstr(xyz[passenger + "_1"] + xyz[passenger + "_2"] + xyz[passenger + "_3"] >= 3 * AllVariables["t_PA_" + movement + "_" + Act_c.id].Var)
								}
							}
						}
					}
					#pragma endregion


					#pragma region setting a few constraints (entering and exiting the system )
					for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
					{
						strVehID = std::to_string(veh_id);	
						if (veh_id < 10) strVehID = "0" + strVehID;
						GRBLinExpr expr2 = 0, expr3 = 0;
						for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
						{
							string key = it->first;

							//entering the system -  Only one entrance
							if (key.find("t_AV_" + strVehID + "_Hom_S_") != std::string::npos)
							{
								expr2 += (it->second);
							}

							//exit --> only on exit from the system
							if (key.substr(0, 8) == "t_AV_" + strVehID + "_"  && key.substr(key.size() - 8) == "_S_Hom_E")
							{
								expr3 += (it->second);
							}
						}						
						model.addConstr(expr2 <= 1, "Just one entry to the system"); //SOS entring to the system
						model.addConstr(expr3 <= 1, "Just one exit from the system"); //SOS Exit from the system
						model.addConstr(expr2 == expr3, "if a vehicle enters, it must exit"); // enter==exit	
						model.addConstr((grb_Vars["t_VE_" + strVehID]) == expr2, "Moving Vehilce == Entered Vehicles");
					}
					#pragma endregion

					#pragma region before leaving the parking, the vehicle should have gone to the parking
					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							Setting_prev_and_Next_Acts (1)

							if (!per1_activity_prev) continue;
#pragma region 
							for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
							{
								strVehID = std::to_string(veh_id);
								if (veh_id < 10) strVehID = "0" + strVehID;
								strVarName = "t_AV_" + strVehID + "_" + per1_strActID + "_S_Prk_S";
								if (grb_Vars.find(strVarName) == grb_Vars.end()) continue;

								GRBLinExpr expr1 = 0;
								for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
								{
									string key = it->first;
									if (key.substr(0, 18) == "t_AV_" + strVehID + "_" + per1_strActID + "SPrk_E_")
									{
										expr1 += (it->second);
									}
								}
								model.addConstr(grb_Vars[strVarName] == expr1, "X1_" + strVarName);
							}
#pragma endregion
						}
					}
#pragma endregion

					#pragma region 	constraints: Making sure each person is 1-picked up and 2-dropped off 3- by the same mode 4-a vehicle has arrived for pick up 5- Vehicle leaves after drop off
					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							Setting_prev_and_Next_Acts(1);
								
							if (!per1_activity_next) continue;

#pragma region Pickup and drop off
							GRBLinExpr expr_pickup = 0, expr_dropoff = 0;
							for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
							{
								string key = it->first;

								//pickup
								if (key.substr(0, 5) == "t_AV_" &&  key.substr(7, 7) == "_" + per1_strActID + "_E_")
								{
									expr_pickup += (it->second);
								}

								//drop off	 //"_" + Act_a.nextAct.id + "_S")
								if (key.substr(0, 5) == "t_AV_" &&  key.substr(key.size() - 6) == "_" + per1_strActID_next + "_S")
								{
									expr_dropoff += (it->second);
								}
							}
							model.addConstr(expr_pickup == 1, "expr_pickup == 1");
							model.addConstr(expr_dropoff == 1, "expr_dropoff == 1");
#pragma endregion

							for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
							{
								strVehID = std::to_string(veh_id);
								if (veh_id < 10) strVehID = "0" + strVehID;
								GRBLinExpr expr_pickup = 0, expr_dropoff = 0, expr3 = 0, expr_arrive_at_pickup = 0, expr_leave_dropoff = 0;
								for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
								{
									string key = it->first;

									//constranits: Making sure the vehicle that picks up, is the one that drops off
									if (key.substr(0, 14) == "t_AV_" + strVehID + "_" + per1_strActID + "_E_")
									{
										expr_pickup += (it->second);
									}

									if (key.substr(0, 8) == "t_AV_" + strVehID + "_" && key.substr(key.size() - 6) == "_" + per1_strActID_next + "_S")
									{
										expr_dropoff += (it->second);
									}


									//constraints: Make sure before pick up, a vehicle has arrived at the location
									if (key.substr(0, 8) == "t_AV_" + strVehID + "_" && key.substr(key.size() - 6) == "_" + per1_strActID + "_E")
									{
										expr_arrive_at_pickup += (it->second);
									}

									//constraints: Make sure when a vehicle drops off, it leaves the location
									if (key.substr(0, 14) == "t_AV_" + strVehID + "_" + per1_strActID_next + "_S_")
									{
										expr_leave_dropoff += (it->second);
									}
								}
								model.addConstr(expr_pickup == expr_dropoff, "C_Same_Vehicle_Picks_Drops" + per1_strActID);
								model.addConstr(expr_pickup == expr_arrive_at_pickup, "C_Pickup_Available1_" + per1_strActID);
								model.addConstr(expr_dropoff == expr_leave_dropoff, "C_Leave_after_dropoff" + per1_strActID);
							}
						}
					}
					#pragma endregion


					#pragma region Constraint: feasibility of trips in terms of Travel times : if a person has been dropped off throught another location
					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							Setting_prev_and_Next_Acts(1);

							for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
							{
								auto per2 = (*per2_itr);
								_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
								_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
								_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

								for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
								{
									Setting_prev_and_Next_Acts(2);

									if (per1->uuid<long>() == per2->uuid<long>() && per1_strActID == per2_strActID)
									{
										continue;
									}
									for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
									{
										strVehID = std::to_string(veh_id);
										if (veh_id < 10) strVehID = "0" + strVehID;

										auto it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S");
										if (it != grb_Vars.end())
										{

											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											model.addConstr(((grb_Vars["start_" + per1_strActID])) + tt.Value - ((grb_Vars["start_" + per2_strActID])) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S");
											model.addConstr(((grb_Vars["start_" + per1_strActID])) + tt.Value - ((grb_Vars["start_" + per2_strActID])) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S");

											//Making sure a loop trip does not happen!
											auto it1 = grb_Vars.find("t_AV_" + strVehID + "_" + per2_strActID + "_S_" + per1_strActID + "_S");
											if (it1 != grb_Vars.end())
												model.addConstr(it->second + it1->second <= 1, "X2" + it->first + "_" + it1->first);
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											model.addConstr((grb_Vars["start_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");
											model.addConstr((grb_Vars["start_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");

											//Making sure a loop trip does not happen!
											auto it1 = grb_Vars.find("t_AV_" + strVehID + "_" + per2_strActID + "_E_" + per1_strActID + "_S");
											if (it1 != grb_Vars.end())
												model.addConstr(it->second + it1->second <= 1, "X3" + it->first + "_" + it1->first);
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");

											//Making sure a loop trip does not happen!
											auto it1 = grb_Vars.find("t_AV_" + strVehID + "_" + per2_strActID + "_S_" + per1_strActID + "_E");
											if (it1 != grb_Vars.end())
												model.addConstr(it->second + it1->second <= 1, "X4" + it->first + "_" + it1->first);
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");

											//Making sure a loop trip does not happen!
											auto it1 = grb_Vars.find("t_AV_" + strVehID + "_" + per2_strActID + "_E_" + per1_strActID + "_E");
											if (it1 != grb_Vars.end())
												model.addConstr(it->second + it1->second <= 1, "X5" + it->first + "_" + it1->first);

										}

										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "SPrk_E_" + per2_strActID + "_E");
										if (it != grb_Vars.end())
										{
											//Time_Minutes ttime_home_to_next = network->template Get_TTime<_Activity_Location_Interface*, MODE, Time_Minutes, Time_Minutes>(_Parent_Person->template Home_Location<_Activity_Location_Interface*>(), next_act->template Location<_Activity_Location_Interface*>(), MODE::SOV, end_this + ttime_this_to_home);

											Time_Minutes tt1 = Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1->template Home_Location<_activity_location_itf*>(), per1_activity->Start_Time<Time_Minutes>());
											Time_Minutes tt2 = Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per2_activity->Location<_activity_location_itf*>(), per2_activity->End_Time<Time_Minutes>());
											model.addConstr((grb_Vars["start_" + per1_strActID]) - (grb_Vars["end_" + per2_strActID]) + tt1.Value + tt2.Value <= 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "SPrk_E_" + per2_strActID + "_E");

										}
									}
								}
							}
						}

					}
					#pragma endregion


					Simplify_Problem(model, grb_Vars, act_ID_map);

					//region Constructing Objective function
					//Note: utility for accomplishing the trip(taking the person from one activity to the next activity is applied as a constraint).It never enters the cost function
					//Only disutilities are included in the cost function
					//Initializing the objective function
					
					auto Obj_LinExpr = GRBLinExpr(0.0);

					//One person movements
					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						firstAct = true;
						lastAct = false;

						_person_static_properties_itf* per1_properties = per1->Static_Properties<_person_static_properties_itf*>();
						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();

						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							Setting_prev_and_Next_Acts(1);

							Add_Absolute(grb_Vars, model, *per1_activity, "start", per1_activity->Start_Time<Time_Minutes>(), grb_Vars["start_" + per1_strActID], Obj_LinExpr, per1_strActID);
							Add_Absolute(grb_Vars, model, *per1_activity, "duration", per1_activity->Duration<Time_Minutes>(), grb_Vars["duration_" + per1_strActID], Obj_LinExpr, per1_strActID);

							//#for Taxi and Walk, These variables are 1 when person takes taxi or walks to the next activity location
							//#disutility / cost  of taking taxi to next activity
							//#cost of taxi : $2 + $24 per hour($0.4 per minute)
							//Obj_LinExpr.addTerms(cost_taxi_fixed + cost_taxi_by_minute * Get_Travel_Time(Act.id, Act.nextAct.id), AllVariables["t_AV_00_" + Act.id + "_E_" + Act.nextAct.id + "_S"].Var)										
							if (per1_activity_next)
							{
								Obj_LinExpr += (grb_Vars["t_AV_00_" + per1_strActID + "_E_" + per1_strActID_next + "_S"]) * (cost_taxi_fixed + cost_taxi_by_minute * Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1_activity_next->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>()));
							}

#pragma region One-Person related
							for (auto const& kvp : grb_Vars)
							{
								const string& key = kvp.first;
								const GRBVar& var = (kvp.second);

								if (key.substr(0, 5) != "t_AV_" || key.substr(0, 8) == "t_AV_00_") continue;

								if (per1_activity_prev)
								{
									//exiting the system
									if (key.substr(key.size() - 12) == "_" + per1_strActID + "_S_Hom_E")
									{
										auto tt = Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1->template Home_Location<_activity_location_itf*>(), per1_activity->Start_Time<Time_Minutes>());
										Obj_LinExpr += var * cost_AV_energy * tt.Value;

										//	#TODO: make sure we do not double charge this.This should be removed when I know how many people are in the car, and charge empty vehicles instead of looking at whether their last stop was the last activity!!!!
										//	#just charge ZOV tax if the vehilce is sent home from an activity other than the last activity
										if (per1_activity_next)
											Obj_LinExpr += var * (cost_ZOV_fixed_tax + cost_ZOV_tax * tt.Value);
									}

									//traveling to parking(to save parking cost)
									if (key.substr(key.size() - 12) == "_" + per1_strActID + "_S_Prk_S")
									{
										auto tt = Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1->template Home_Location<_activity_location_itf*>(), per1_activity->Start_Time<Time_Minutes>());
										Obj_LinExpr += var * cost_AV_energy * tt.Value;
										Obj_LinExpr += var * (cost_ZOV_fixed_tax + cost_ZOV_tax * tt.Value);
									}
								}
								if (per1_activity_next)
								{
									//entering the system
									if (key.substr(key.size() - 12) == "_Hom_S_" + per1_strActID + "_E")
									{
										//fixed Cost of new AV in the system
										auto tt = Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per1_activity->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>());
										Obj_LinExpr += var * cost_AV_fixed;
										Obj_LinExpr += var * cost_AV_energy * tt.Value;
										//TODO : charge the ZOV tax if it enters an activity location other than the first one.make sure it is not double counted when empty vehicles are being taxed.
										if (per1_activity_prev)
											Obj_LinExpr += var * (cost_ZOV_fixed_tax + cost_ZOV_tax * tt.Value);
									}

									//returning from parking to an act location
									if (key.substr(key.size() - 11) == "Prk_E_" + per1_strActID + "_E")
									{
										//fixed Cost of new AV in the system
										auto tt = Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per1_activity->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>());
										Obj_LinExpr += var * cost_AV_energy * tt.Value;
										if (per1_activity_prev)
											Obj_LinExpr += var * (cost_ZOV_fixed_tax + cost_ZOV_tax * tt.Value);
									}

									//This is the variable that shows movement from an activity to the next activity for one person
									//cost / disutility(fuel cost) of traveling to next act
									//disutility / cost for travelling. (taking the person from one activity to the next activity is applied as a constraint).
									//Cost of AV : $5 per hour(0.0833 per minute)
									if (key.substr(key.size() - 12) == "_" + per1_strActID + "_E_" + per1_strActID_next + "_S")
									{
										Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per1_activity->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>());
									}
								}

								//disutility for parking at the location
								//Cost of Parking : $0.5 per hour(0.00833 per minute)
								if (per1_activity_prev && per1_activity_next &&  key.substr(key.size() - 12) == "_" + per1_strActID + "_S_" + per1_strActID + "_E")
								{
									if(!Home_Activity(per1_activity))
										Obj_LinExpr += var * cost_parking *  per1_activity->Duration<Time_Minutes>();
								}
							}
#pragma endregion

#pragma region Intrer-Personal movements related
							if (!per1_activity_prev) continue;

							for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
							{
								auto per2 = (*per2_itr);
								_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
								_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
								_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

								if (per1->uuid<long>() == per2->uuid<long>()) continue;

								for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
								{
									Setting_prev_and_Next_Acts(2);

									for (auto const& kvp : grb_Vars)
									{
										const string& key = kvp.first;
										const GRBVar& var = (kvp.second);
										if (key.substr(0, 5) != "t_AV_" || key.substr(0, 8) == "t_AV_00_") continue;

										//#Non ZOV
										//#E - S: has picked up person i and is going to drop off person j
										if (per1_activity_next && per2_activity_prev)
										{
											if (key.substr(key.size() - 12) == "_" + per1_strActID + "_E" + per2_strActID + "_S")
											{
												Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											}
										}

										//#s - s  has droped off person i, and is going to drop off person j
										else if (per1_activity_prev && per2_activity_prev)
										{
											if (key.substr(key.size() - 12) == "_" + per1_strActID + "_S" + per2_strActID + "_S")
											{
												Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());

											}
										}

										//#s - E: has droped off person i, and is going to pick up person j
										else if (per1_activity_prev && per2_activity_next)
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											if (key.substr(key.size() - 12) == "_" + per1_strActID + "_S" + per2_strActID + "_E")
											{
												Obj_LinExpr += var * cost_AV_energy * tt;

												//	#if a vehicle arrives early, it has to wait for the person to be picked up
												//	#TODO : lets fix the parking time.other variables need to be added in order to use variable parking time
												//TODO: Keep in mind this implies if the first activity is somehow a home location, the vehicle will be charge for parking, even when the vehicle could have waited at home before going to act location												
												double parking_time = max(0.0, per2_activity->End_Time<Time_Minutes>() - per1_activity->Start_Time<Time_Minutes>() - tt.Value);
												Obj_LinExpr += var * cost_parking * parking_time;

												Obj_LinExpr += var * (cost_ZOV_fixed_tax + cost_ZOV_tax * tt.Value);
											}
										}

										//	#E - E: has picked up person i, and is going to pick up person j
										else if (per1_activity_next && per2_activity_next)
										{
											if (key.substr(key.size() - 12) == "_" + per1_strActID + "_E" + per2_strActID + "_E")
											{
												Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											}
										}
									}
								}
							}
#pragma endregion

						}
					}

					model.setObjective(Obj_LinExpr, GRB_MAXIMIZE);					
#ifdef Write_Visualization_Files
					model.update();
					auto HHID = this->_Parent_Household->uuid<long long>();
					ofstream myfile;
					model.write(scenario->template output_dir_name<string>() + "\\ZOV\\model_" + to_string(HHID) + ".lp");
#endif
					model.optimize();

					//cout << x.get(GRB_StringAttr_VarName) << " "  << x.get(GRB_DoubleAttr_X) << endl;
					auto status_code = model.get(GRB_IntAttr_Status);
					if (status_code == GRB_OPTIMAL)
					{
						//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
						//cout << "End " << HHID << "\tHH_SIZ: " << HH_size << "\ttime: " << duration << "\tcounter: " << counter_solved <<  endl;
						counter_solved++;
						auto dv = std::div(counter_solved, 10);
						if (dv.rem ==  0)
						{
							cout << "\r" << "solved: " << counter_solved <<  "\ttimed out: " << counter_timedout << "\terror: " << counter_error << std::flush;
						}

#ifdef Write_Visualization_Files
						//void Generate_Activities_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
						Generate_Activities_File(grb_Vars, act_ID_map);
						Generate_Trips_File(grb_Vars, act_ID_map);
						//Generate_Results_File(grb_Vars, act_ID_map);
						Generate_Results2_File(model);
#endif
						return Create_Return_Object(model, grb_Vars, act_String_map);
					}
					else if (status_code == GRB_INFEASIBLE)
					{
						counter_error++;
#ifdef Debug_Intrahousehold_Vehicle_Assignment
						cout << "Infeasible or unbounded! " << HHID << "\t" << endl;
						model.write(scenario->template output_dir_name<string>() + "\\ZOV\\model_" + to_string(HHID) + ".lp");
						Generate_Activities_File2(grb_Vars, act_ID_map);
#endif
						return nullptr;
					}
					else if (status_code == GRB_TIME_LIMIT)
					{
						counter_timedout++;
#ifdef Debug_Intrahousehold_Vehicle_Assignment
						
						cout << "Time Limit Reached! " << HHID << "\t" << counter_timedout << endl;
#endif
						return nullptr;
					}
					else 
					{
						counter_error++;
#ifdef Debug_Intrahousehold_Vehicle_Assignment
						cout << "Optimization was stopped with status = " << status_code <<"\tHHID = " <<  HHID << endl;
#endif
						return nullptr;
					}
				}
				catch (GRBException e)
				{
					counter_error++;
					cout << "gurobi Error! Error code = " << e.getErrorCode() << endl;
					cout << e.getMessage() << endl;
					return nullptr;
				}
				catch (...)
				{
					counter_error++;
					cout << "gurobi Error! Exception during optimization" << endl;
					return nullptr;
				}
			}

			void Run_Gurobi_Model()
			{
				Assign_Shared_Vehicles();

				//_Demand_Interface* demand = (_Demand_Interface*)_global_demand;
				//_Vehicle_Types_Interface* veh_types = demand->vehicle_types_container<_Vehicle_Types_Interface*>();

				//_Network_Interface* network = this->_Parent_Household->network_reference<_Network_Interface*>();
				//_Vehicles_Container_Interface* vehicles = _Parent_Household->Vehicles_Container<_Vehicles_Container_Interface*>();

				//this->_Parent_Household->network_reference<_Network_Interface*>();
				//household_static_properties_itf* household_properties = _Parent_Household->Static_Properties<household_static_properties_itf*>();

				//long long lookup_id = census_zone->ID<long long>();

				//// create a vehicle for num vehicles in the household
				//for (int i = 0; i < household_properties->Number_of_vehicles<int>(); ++i)
				//{
				//	float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();
				//	vehicle_distribution_container_type::iterator itr = _vehicle_distribution_container.find(lookup_id);

				//	// home census tract could not be found in vehicle type distribution file!
				//	if (itr == _vehicle_distribution_container.end())
				//	{
				//		THROW_WARNING("WARNING: home census tract '" << lookup_id << "' could not be found in vehicle type distribution file!" << endl);
				//		itr = _vehicle_distribution_container.begin();
				//	}

				//	//_Vehicle_Types_Interface veh_types = (*itr)->second;  //.push_back(vehicle_probability_pair_type(veh_char, prob));
				//	double sumProb = 0;
				//	for (auto x = (*itr).second.begin(); x != (*itr).second.end(); x++)
				//	{
				//		sumProb += x->second;
				//		if (sumProb >= rand)
				//		{
				//			// Allocate a new vehicle
				//			_Vehicle_Interface* veh = (_Vehicle_Interface*)Allocate<_Vehicle_type>();

				//			veh->initialize(x->first, _Parent_Household->uuid<int>());
				//			veh->is_integrated(true);

				//			// determine additional technology for vehicle
				//			_Vehicle_Technology_Chooser->Select_Vehicle_Technology(veh, _Parent_Household->Get_Primary_Driver<_Person_Interface*>());

				//			// Push to household vehicle container
				//			vehicles->push_back(veh);
				//			break;
				//		}
				//	}
				//}
			}

			void Generate_HH_File() 
			{
			
			}


			void Generate_Trips_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				auto HHID = this->_Parent_Household->uuid<long long>();
				_scenario_itf* scenario = _Parent_Household->template scenario_reference<_scenario_itf*>();
				//Vehicle_ID: parseInt(d["Vehicle_ID"]), From_Act: d["From_Act_Loc"], From_Act_OSE: d["OSE"], To_Act: d["To_Act_Loc"], To_Act_DSE: d["DSE"]};
				ofstream myfile;
				myfile.open(scenario->template output_dir_name<string>() + "\\ZOV\\Trips_" + to_string(HHID) + ".txt");
				std::stringstream file_content;				
				file_content << "Term,Vehicle_ID,From_Act_Loc,OSE,To_Act_Loc,DSE\n";
				
				size_t pos1 = 0, pos2 = 0;
				for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
				{
					auto key = it->first;
					pos1 = 0; pos2 = 0;
					if (key.substr(0, 5) == "t_AV_")
					{
						if (it->second.get(GRB_DoubleAttr_X) > 0.00001)
						{							
							string Vehicle_ID = key.substr(5, 2);
							pos1 = key.find('_', 8);
							string From_Act = key.substr(8, pos1 - 8);
							string From_Act_OSE = key.substr(pos1 + 1, 1);
							pos2 = key.find('_', pos1 + 1);
							string To_Act = key.substr(pos2 + 1, pos2 - pos1 + 1);
							string To_Act_DSE = key.substr(key.size() - 1, 1);
							
							file_content << key << "," << Vehicle_ID << "," << From_Act << "," << From_Act_OSE<< "," << To_Act << "," << To_Act_DSE << endl;
						}
					}
				}
				myfile << file_content.rdbuf();
				myfile.close();
			}

			void Generate_Activities_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{				
				auto HHID = this->_Parent_Household->uuid<long long>();
				_scenario_itf* scenario = _Parent_Household->template scenario_reference<_scenario_itf*>();
				ofstream myfile;
				myfile.open(scenario->template output_dir_name<string>() + "\\ZOV\\Activities_" + to_string(HHID) + ".txt");
				std::stringstream file_content;
				//file_content << "Houehols_ID,Person_ID,Activity_ID,Activity_Order,Activity_Location,Start,End,Min_Start,Max_Start,Min_Duration,Max_Duration,New_Start,New_End,New_Duration\n";
				file_content << "Household_ID,Person_ID,Activity_ID,Start,End,New_Start,New_End,At_Home\n";
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();

				for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
				{
					auto per = (*per_itr);
					_scheduler_itf* per_scheduler = per->Scheduling_Faculty<_scheduler_itf*>();
					_activity_container_itf* per_activities = per_scheduler->Activity_Container<_activity_container_itf*>();					
					for (auto per_act_itr = per_activities->begin(); per_act_itr != per_activities->end(); per_act_itr++)
					{
						auto per_activity = *per_act_itr;
						auto perID = per->uuid<long>();
						int act_id = per_activity->Activity_Plan_ID<int>();

						//string per_strActID = act_counter >= 100 ? to_string(act_counter) : act_counter >= 10 ? "0" + to_string(act_counter) : "00" + to_string(act_counter);
						std::pair<long, int> key = std::make_pair(per->uuid<long>(), act_id);	
						
						int start = (int) per_activity->template Start_Time<Time_Minutes>().Value;
						int end = (int)per_activity->template End_Time<Time_Minutes>().Value;
						int new_start = (int)grb_Vars["start_" + act_ID_map[key]].get(GRB_DoubleAttr_X);
						int new_end = (int)grb_Vars["end_" + act_ID_map[key]].get(GRB_DoubleAttr_X);
						int  at_home = 0;
						if (per_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY || 
							per_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
							at_home = 1;
						file_content << HHID << "," << perID << "," << act_ID_map[key] << "," << start	 << "," << end << "," << new_start << "," << new_end << "," << at_home << endl;
					}
				}
				myfile << file_content.rdbuf();
				myfile.close();
			}

			void Generate_Activities_File2(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				//auto HHID = household_properties->ID<float>();
				auto HHID = this->_Parent_Household->uuid<long long>();
				_scenario_itf* scenario = _Parent_Household->template scenario_reference<_scenario_itf*>();
				ofstream myfile;
				myfile.open(scenario->template output_dir_name<string>() + "\\ZOV\\Activities_" + to_string(HHID) + ".txt");
				std::stringstream file_content;
				//file_content << "Houehols_ID,Person_ID,Activity_ID,Activity_Order,Activity_Location,Start,End,Min_Start,Max_Start,Min_Duration,Max_Duration,New_Start,New_End,New_Duration\n";
				file_content << "Household_ID,Person_ID,Activity_ID,Start,End,New_Start,New_End,At_Home,Mode\n";
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();

				for (auto per_itr = persons->begin(); per_itr != persons->end(); per_itr++)
				{
					auto per = (*per_itr);
					_scheduler_itf* per_scheduler = per->Scheduling_Faculty<_scheduler_itf*>();
					_activity_container_itf* per_activities = per_scheduler->Activity_Container<_activity_container_itf*>();
					for (auto per_act_itr = per_activities->begin(); per_act_itr != per_activities->end(); per_act_itr++)
					{
						auto per_activity = *per_act_itr;
						auto perID = per->uuid<long>();
						int act_id = per_activity->Activity_Plan_ID<int>();
						auto mode = per_activity->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();

						//string per_strActID = act_counter >= 100 ? to_string(act_counter) : act_counter >= 10 ? "0" + to_string(act_counter) : "00" + to_string(act_counter);
						std::pair<long, int> key = std::make_pair(per->uuid<long>(), act_id);

						int start = (int)per_activity->template Start_Time<Time_Minutes>().Value;
						int end = (int)per_activity->template End_Time<Time_Minutes>().Value;
						int new_start = start;
						int new_end = end;
						int  at_home = 0;
						if (per_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::AT_HOME_ACTIVITY ||
							per_activity->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
							at_home = 1;
						file_content << HHID << "," << perID << "," << act_ID_map[key] << "," << start << "," << end << "," << new_start << "," << new_end << "," << at_home <<"," << mode << endl;
					}
				}
				myfile << file_content.rdbuf();
				myfile.close();
			}

			void Generate_Results_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				auto HHID = this->_Parent_Household->uuid<long long>();

				ofstream myfile;
				myfile.open(scenario->template output_dir_name<string>() + "\\ZOV\\results_" + to_string(HHID) + ".txt");
				//x.get(GRB_StringAttr_VarName) << " "
				//x.get(GRB_DoubleAttr_X) << endl;
				for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
				{
					//if (it->first.substr(0, 5) == "t_AV_")
					//{
					//	if (it->second.get(GRB_DoubleAttr_X) > 0.00001)
					//		myfile << it->first << "\t" << it->second.get(GRB_DoubleAttr_X) << endl;
					//}
					//else 
					if ((it->first.find("start_") != string::npos || it->first.find("end_") != string::npos || it->first.find("duration_") != string::npos) && it->first.find("abs") == string::npos)
					{
						myfile << it->first << "\t" << it->second.get(GRB_DoubleAttr_X) << endl;
					}
				}
				myfile.close();
			}

			void Generate_Results2_File(GRBModel& model)
			{
				auto HHID = this->_Parent_Household->uuid<long long>();
				_scenario_itf* scenario = _Parent_Household->template scenario_reference<_scenario_itf*>();
				ofstream myfile;
				myfile.open(scenario->template output_dir_name<string>() + "\\ZOV\\results2_" + to_string(HHID) + ".txt");
				//x.get(GRB_StringAttr_VarName) << " "
				//x.get(GRB_DoubleAttr_X) << endl;
				int numvars = model.get(GRB_IntAttr_NumVars);
				GRBVar *vars = 0;
				vars = model.getVars();
				for (int j = 0; j < numvars; j++) 
				{
					GRBVar v = vars[j];
					double x = v.get(GRB_DoubleAttr_X);
					//if (x != 0.0) 
					{
						myfile  << v.get(GRB_StringAttr_VarName) << "\t" << x << endl;
					}
				}
				//for (auto it = model. .begin(); it != grb_Vars.end(); it++)
				//{
				//	if ((it->first.find("start_") != string::npos || it->first.find("end_") != string::npos || it->first.find("duration_") != string::npos) && it->first.find("abs") == string::npos)
				//	{
				//		myfile << it->first << "\t" << it->second.get(GRB_DoubleAttr_X) << endl;
				//	}
				//}

				myfile.close();
			}

			
			void Eliminate_Infeasible_Trips(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				size_t pos1 = 0, pos2 = 0;
				for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
				{
					string key = it->first;
					if (key.substr(0, 5) != "t_AV_" || key.find("_Hom_") != std::string::npos) continue;


					_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();
					_activity_itf *per1_activity, *per1_activity_next, *per1_activity_prev;
					_activity_itf *per2_activity, *per2_activity_next, *per2_activity_prev;
					bool firstAct, lastAct;
					string per1_strActID, per1_strActID_next, per1_strActID_prev;
					string per2_strActID, per2_strActID_next, per2_strActID_prev;
					string strVarName, strVehID;

					for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
					{
						auto per1 = (*per1_itr);
						_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
						_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();
						for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
						{
							auto per1_activity = *per1_act_itr;
							string per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);

							for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
							{
								auto per2 = (*per2_itr);
								_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
								_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
								_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

								for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
								{
									auto per2_activity = *per2_act_itr;
									string per2_strActID = GetstrActID(per2_activity, act_ID_map, per2);

									//auto it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S");
									//if (it != grb_Vars.end())
									//{
									//	auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
									//	model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
									//	model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
									//}

								}
							}
						}

					}

				}
			}


			void Simplify_Problem(GRBModel& model, map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();
				_activity_itf *per1_activity, *per1_activity_next, *per1_activity_prev;
				_activity_itf *per2_activity, *per2_activity_next, *per2_activity_prev;
				bool firstAct, lastAct;
				string per1_strActID, per1_strActID_next, per1_strActID_prev;
				string per2_strActID, per2_strActID_next, per2_strActID_prev;
				string strVarName, strVehID;

				for (auto per1_itr = persons->begin(); per1_itr != persons->end(); per1_itr++)
				{
					auto per1 = (*per1_itr);
					_scheduler_itf* per1_scheduler = per1->Scheduling_Faculty<_scheduler_itf*>();
					_activity_container_itf* per1_activities = per1_scheduler->Activity_Container<_activity_container_itf*>();
					for (auto per1_act_itr = per1_activities->begin(); per1_act_itr != per1_activities->end(); per1_act_itr++)
					{
						Setting_prev_and_Next_Acts(1);

						auto mode = per1_activity->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>();
						if (per1_activity_prev)
						{
							strVarName = "t_AV_00_" + per1_strActID_prev + "_E_" + per1_strActID + "_S";
#if defined Ignore_None_Auto_Modes
							if (mode != Vehicle_Components::Types::Vehicle_Type_Keys::SOV && mode != Vehicle_Components::Types::Vehicle_Type_Keys::HOV)
							{
								model.addConstr(grb_Vars[strVarName] == 1, "X8_" + strVarName);

								for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
								{
									string key = it->first;
									if (key.substr(0, 5) == "t_AV_" && key.substr(0, 8) != "t_AV_00_")
									{
										if (key.substr(key.size() - 6) == "_" + per1_strActID_prev + "_E")
										{
											model.addConstr(it->second == 0, "X6_" + it->first);
										}

										else if (key.substr(7, 7) == "_" + per1_strActID + "_S_" || key.find("_" + per1_strActID + "SPrk_E_") != std::string::npos)
										{
											model.addConstr(it->second == 0, "X7_" + it->first );
										}
										else if (key.find(per1_strActID_prev + "_E_" + per1_strActID + "_S") != std::string::npos )
										{
											model.addConstr(it->second == 0, "X9_" + it->first);
										}

									}

								}
							}
#endif

#if defined Ignore_Taxi 
	#if defined Ignore_None_Auto_Modes
							if (!(mode != Vehicle_Components::Types::Vehicle_Type_Keys::SOV && mode != Vehicle_Components::Types::Vehicle_Type_Keys::HOV))
								model.addConstr(grb_Vars[strVarName] == 0);							
	#else 
							model.addConstr(grb_Vars[strVarName] == 0);
	#endif
#endif
						}

						


						//for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
						//{
						//	auto per2 = (*per2_itr);
						//	_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
						//	_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
						//	_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

						//	for (auto per2_act_itr = per2_activities->begin(); per2_act_itr != per2_activities->end(); per2_act_itr++)
						//	{
						//		auto per2_activity = *per2_act_itr;
						//		string per2_strActID = GetstrActID(per2_activity, act_ID_map, per2);

						//		//auto it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S");
						//		//if (it != grb_Vars.end())
						//		//{
						//		//	auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
						//		//	model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
						//		//	model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
						//		//}

						//	}
						//}
					}

				}
			}
		
			vector<_movement_plan_itf* >* Create_Return_Object(GRBModel& model, map<string, GRBVar>& grb_Vars, map<string, _activity_itf *>& act_String_map)
			{
				vector<_movement_plan_itf* >* movements = new std::vector< _movement_plan_itf*>();
				size_t pos1 = 0, pos2 = 0;
				for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
				{
					auto key = it->first;
					pos1 = 0; pos2 = 0;
					if (key.substr(0, 5) == "t_AV_")
					{
						if (it->second.get(GRB_DoubleAttr_X) > 0.00001)
						{
							string Vehicle_ID = key.substr(5, 2);
							pos1 = key.find('_', 8);
							string From_Act = key.substr(8, pos1 - 8);
							string From_Act_OSE = key.substr(pos1 + 1, 1);
							pos2 = key.find('_', pos1 + 1);
							string To_Act = key.substr(pos2 + 1, pos2 - pos1 + 1);
							string To_Act_DSE = key.substr(key.size() - 1, 1);

							_activity_itf *origin_act = nullptr, *destination_act = nullptr;
							_activity_location_itf *origin_location, *destination_location;
							_persons_container_itf* persons = _Parent_Household->Persons_Container<_persons_container_itf*>();
							auto home_location = (*persons->begin())->template Home_Location<_activity_location_itf*>();
							bool Inter_Person_Trip = false;
							long per1_ID = -1, per2_ID = -1;
							if (From_Act == "Hom" || From_Act == "Prk")
							{
								origin_location = home_location;
								Inter_Person_Trip = true;
							}
							else
							{
								origin_act = act_String_map[From_Act];
								auto per1_ID = origin_act->template Parent_Planner<_planning_itf*>()->template Parent_Person<_person_itf*>()->template uuid<long>();
								origin_location = origin_act->Location<_activity_location_itf*>();
							}

							if (To_Act == "Hom" || To_Act == "Prk")
							{
								destination_location = home_location;
								Inter_Person_Trip = true;
							}
							else
							{
								_activity_itf* destination_act = act_String_map[To_Act];
								auto per2_ID = destination_act->template Parent_Planner<_planning_itf*>()->template Parent_Person<_person_itf*>()->template uuid<long>();
								destination_location = destination_act->Location<_activity_location_itf*>();
							}
							
							if (per1_ID == per2_ID || origin_location == destination_location) continue;
							_movement_plan_itf* movement_plan = (_movement_plan_itf*)Allocate<typename _movement_plan_itf::Component_Type>();
							movement_plan->template origin<_activity_location_itf*>(origin_location);
							movement_plan->template destination<_activity_location_itf*>(destination_location);
							Time_Minutes tt;

							if (origin_act) 
							{
								if (From_Act_OSE == "S")
									movement_plan->template departed_time<Time_Minutes>(origin_act->Start_Time<Time_Minutes>() );
								else
									movement_plan->template departed_time<Time_Minutes>(origin_act->End_Time<Time_Minutes>() );
							}
							else if (destination_act)
							{
								if (To_Act_DSE == "S")
								{
									tt = Get_Travel_Time(origin_location, destination_location, destination_act->Start_Time<Time_Minutes>());
									movement_plan->template departed_time<Time_Minutes>(destination_act->Start_Time<Time_Minutes>() - tt);
								}
								else
								{
									tt = Get_Travel_Time(origin_location, destination_location, destination_act->End_Time<Time_Minutes>());
									movement_plan->template departed_time<Time_Minutes>(destination_act->End_Time<Time_Minutes>() - tt);
								}
							}
							else
							{
								cout <<  "Error in gurobi! " << endl;
							}
							movements->push_back(movement_plan);
						}
					}
				}

				if (!movements->empty()) 
					return movements;
				else 
					delete movements;

			}
		};
			
		//template<typename MasterType, typename InheritanceList> typename IntraHousehold_AV_Assignment_Implementation <MasterType, InheritanceList>::type_of(is_initialized) IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::_is_initialized = false;
		//template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_travel_time = 1;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_act_dur = 5;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_start_flex = 15;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_dur_flex = 15;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_AV_fixed = -5.0;				//fixed cost of each AV in the system
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_parking = -3.0 / 60.0;		//$0.5 per hour
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_AV_energy = -5.0 / 60.0;		//Cost of AV : $5 per hour(0.0833 per minute)
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_vot = -10.0 / 60.0;				//Person time cost : $10 / hr($0.167 / min) When changes to the activity start and duration is made
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_taxi_fixed = -3.0;			//#cost of taxi : $2
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_taxi_by_minute = -20.0 / 60.0;	//#$24 per hour($0.4 per minute)
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_ZOV_tax = -60.0 / 60.0;		//  #$1 per hour
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_ZOV_fixed_tax = -100 / 100;
		
		template<typename MasterType, typename InheritanceList> int IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::counter_solved = 0;
		template<typename MasterType, typename InheritanceList> int IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::counter_timedout = 0;
		template<typename MasterType, typename InheritanceList> int IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::counter_error = 0;

	}
		
}