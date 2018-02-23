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

#define Investigate_Feasibility

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

		
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(IntraHousehold_AV_Assignment_Implementation), Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Parent_Household, NONE, NONE);

				
			//typedef typename get_component_type(_Persons_Container_Interface) _Person_type;

			typedef Household_Components::Prototypes::Household<typename type_of(Parent_Household)> _household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename _household_itf::get_type_of(Static_Properties)> _household_static_properties_itf;


			typedef Random_Access_Sequence< typename _household_itf::get_type_of(Persons_Container)> _persons_container_itf;
			typedef Person_Components::Prototypes::Person<typename get_component_type(_persons_container_itf)> _person_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Properties)> _properties_itf;
			typedef Person_Components::Prototypes::Person_Properties< typename _person_itf::get_type_of(Static_Properties)> _person_static_properties_itf;

			typedef Person_Components::Prototypes::Person_Scheduler<typename _person_itf::get_type_of(Scheduling_Faculty)> _scheduler_itf;
			typedef Random_Access_Sequence< typename _scheduler_itf::get_type_of(Activity_Container)> _activity_container_itf;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(_activity_container_itf)>  _activity_itf;

			typedef Network_Components::Prototypes::Network< typename _household_itf::get_type_of(network_reference)> _network_itf;

			typedef Activity_Location_Components::Prototypes::Activity_Location<typename MasterType::activity_location_type> _activity_location_itf;


			//typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
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
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;


			template<typename T> void Initialize(T household)
			{
				this->Parent_Household(household);
				//_Vehicle_Technology_Chooser = (Vehicle_Technology_Chooser_type)Allocate<type_of(Vehicle_Technology_Chooser)>();
				//_Vehicle_Technology_Chooser->Initialize(household);												
			}

			int Get_Max_Number_of_AVs()
			{
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				//return 1;
				float HH_size = household_properties->Household_size<float>();
				return HH_size;
			}

			string GetstrActID(_activity_itf* activity, map<pair<long, int>, string>& act_ID_map, _person_itf* per)
			{
				//auto per = activity->_Parent_Planner->template Parent_Person<_person_itf*>();
				auto key = make_pair(per->uuid<long>(), activity->Activity_Plan_ID<int>());
				if (act_ID_map.find(key) == act_ID_map.end() || act_ID_map[key] == "")
				{
					cout << "Check this one!";
				}
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

			GRBVar& Add_New_Variable(map<string, GRBVar>& grb_Vars, GRBModel& model, string strVarName, char vType)
			{
				
				GRBVar new_var;
				if (vType == GRB_CONTINUOUS)
				{
					auto result = grb_Vars.insert(std::pair<string, GRBVar>(strVarName, model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, strVarName)));
					return (*(result.first)).second;
				}
				else if (vType == GRB_BINARY)
				{
					auto result = grb_Vars.insert(std::pair<string, GRBVar>(strVarName, model.addVar(0, 1, 0.0, GRB_BINARY, strVarName)));
					return (*(result.first)).second;
				}
				else
				{
					return model.addVar(0, 1, 0.0, GRB_BINARY, strVarName);
				}
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


			//void Get_Travel_Time(_activity_location_itf* origin, _activity_location_itf* destination,)
			Time_Minutes Get_Travel_Time(_activity_location_itf* origin_loc, _activity_location_itf* destination_loc, Time_Minutes start)
			{
				_network_itf* network = _Parent_Household->template network_reference<_network_itf*>();				
				Time_Minutes ttime_origin_to_destination = network->template Get_TTime<_activity_location_itf*, Vehicle_Type_Keys, Time_Minutes, Time_Minutes>(origin_loc, destination_loc, SOV, start);
				return ttime_origin_to_destination;
			}

			//void Get_Travel_Time(_activity_location_itf* origin, _activity_location_itf* destination,)
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

			void Assign_Shared_Vehicles()
			{
				//_Vehicles_Container_Interface* vehicles = _Parent_Household->Vehicles_Container<_Vehicles_Container_Interface*>();

				_network_itf* network = this->_Parent_Household->network_reference<_network_itf*>();
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				//auto HHID = household_properties->ID<float>();
				auto HHID = this->_Parent_Household->uuid<long long>();
				//_Zone_Interface* home_zone = _Parent_Household->Home_Location<_Zone_Interface*>();
				//_Skim_Interface* skimmer = network->skimming_faculty<_Skim_Interface*>();

				//// external knowledge references
				//_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				//_Scenario_Interface* scenario = _Parent_Person->template scenario_reference<_Scenario_Interface*>();

				map<pair<long,int>, string> act_ID_map;
				int act_counter = 0;
				try
				{
					GRBEnv env = GRBEnv();
					GRBModel model = GRBModel(env);
					map<string, GRBVar> grb_Vars;
					//map<string, GRBVar>& grb_Vars = *p_grb_Vars;
					
					_persons_container_itf* persons = this->_Parent_Household->Persons_Container<_persons_container_itf*>();
					_activity_itf *per1_activity, *per1_activity_next, *per1_activity_prev;
					_activity_itf *per2_activity, *per2_activity_next, *per2_activity_prev;
					bool firstAct, lastAct;
					string per1_strActID, per1_strActID_next, per1_strActID_prev;
					string per2_strActID, per2_strActID_next, per2_strActID_prev;
					string strVarName, strVehID;
					
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
							if (per_strActID == "")
							{
								cout << "Check this out" << endl;
							}
							act_ID_map.insert(std::pair<std::pair<long, int>, string>(key, per_strActID));
							act_counter++;
						}
					}
					

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
							#pragma region Setting prev and Next Acts
														per1_activity = *per1_act_itr;														
														per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);
														auto per1_act_itr_next = std::next(per1_act_itr);

														if (per1_act_itr_next != per1_activities->end())
														{
															per1_activity_next = *per1_act_itr_next;															
															per1_strActID_next = GetstrActID(per1_activity_next, act_ID_map, per1);
														}
														else
														{
															per1_activity_next = nullptr;
															per1_strActID_next = "";
														}

														if (per1_activity != *(per1_activities->begin()))
														{
															per1_activity_prev = *(std::prev(per1_act_itr));															
															per1_strActID_prev = GetstrActID(per1_activity_prev, act_ID_map, per1);
														}
														else
														{
															per1_activity_prev = nullptr;
															per1_strActID_prev = "";
														}
							#pragma endregion

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

														auto start = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														model.addConstr(start >= start_lb, "c_start_lb_" + per1_strActID);
														model.addConstr(start <= start_ub, "c_start_ub_" + per1_strActID);

														strVarName = "end_" + per1_strActID;
														auto end = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														model.addConstr(end >= start_lb, "c_end_lb_" + per1_strActID);
														model.addConstr(end <= start_ub + duration_ub, "c_end_ub_" + per1_strActID);

														strVarName = "duration_" + per1_strActID;
														auto duration = Add_New_Variable(grb_Vars, model, strVarName, GRB_CONTINUOUS);
														model.addConstr(end - start == duration);

														model.addConstr(duration >= duration_lb, "c_dur_lb_" + per1_strActID);
														model.addConstr(duration <= duration_ub, "c_dur_ub_" + per1_strActID);


							#pragma endregion 

							//Adding Taxi trip from an activity to next activity
							if (per1_activity_next)
							{
								Add_New_Variable(grb_Vars, model, "t_AV_00_" + per1_strActID + "_E_" + per1_strActID_next + "_S", GRB_BINARY);
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
									Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_Prk_S", GRB_BINARY);
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

#ifndef Investigate_Feasibility
										if (per1->uuid<long>() != per2->uuid<long>()) 
											continue;
#endif

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

											//Binary: Trip from Parking
											if (per2_activity_prev)
											{
												if (per1->uuid<long>() == per2->uuid<long>())
												{
													//trying to limit the size of the problem by eliminating infeasible combinations
													if (per2_activity->Start_Time<Time_Minutes>() < per1_activity->Start_Time<Time_Minutes>() )
														Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per2_strActID + "SPrk_E_" + per1_strActID + "_E", GRB_BINARY);
												}
												else
													Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per2_strActID + "SPrk_E_" + per1_strActID + "_E", GRB_BINARY);
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

										if (per1_activity_prev && per2_activity_prev)
										{
											Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_S", GRB_BINARY);
										}

										if (per1_activity_prev && per2_activity_next)
										{
											Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E", GRB_BINARY);
										}

										if (per1_activity_next && per2_activity_prev)
										{
											Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S", GRB_BINARY);
										}

										if (per1_activity_next && per2_activity_next)
										{
											Add_New_Variable(grb_Vars, model, "t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E", GRB_BINARY);
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

					model.update();

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
#pragma region Setting prev and next activities
							per1_activity = *per1_act_itr;
							auto per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);
							auto per1_act_itr_next = std::next(per1_act_itr);

							if (per1_act_itr_next != per1_activities->end())
							{
								per1_activity_next = *per1_act_itr_next;
								per1_strActID_next = GetstrActID(per1_activity_next, act_ID_map, per1);
							}
							else
							{
								per1_activity_next = nullptr;
								per1_strActID_next = "";
							}

							if (per1_activity != *(per1_activities->begin()))
							{
								per1_activity_prev = *(std::prev(per1_act_itr));
								per1_strActID_prev = GetstrActID(per1_activity_prev, act_ID_map, per1);
							}
							else
							{
								per1_activity_prev = nullptr;
								per1_strActID_prev = "";
							}
#pragma endregion

							if (!per1_activity_prev) continue;
#pragma region 
							for (int veh_id = 1; veh_id <= Get_Max_Number_of_AVs(); ++veh_id)
							{
								strVehID = std::to_string(veh_id);
								if (veh_id < 10) strVehID = "0" + strVehID;
								GRBLinExpr expr1 = 0;
								for (auto it = grb_Vars.begin(); it != grb_Vars.end(); it++)
								{
									string key = it->first;
									if (key.substr(0, 18) == "t_AV_" + strVehID + "_" + per1_strActID + "SPrk_E_")
									{
										expr1 += (it->second);
									}
								}
								model.addConstr(((grb_Vars["t_AV_" + strVehID + "_" + per1_strActID + "_S_Prk_S"])) == expr1);
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
#pragma region Setting prev and next activities
							per1_activity = *per1_act_itr;
							auto per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);
							auto per1_act_itr_next = std::next(per1_act_itr);

							if (per1_act_itr_next != per1_activities->end())
							{
								per1_activity_next = *per1_act_itr_next;
								per1_strActID_next = GetstrActID(per1_activity_next, act_ID_map, per1);
							}
							else
							{
								per1_activity_next = nullptr;
								per1_strActID_next = "";
							}

							if (per1_activity != *(per1_activities->begin()))
							{
								per1_activity_prev = *(std::prev(per1_act_itr));
								per1_strActID_prev = GetstrActID(per1_activity_prev, act_ID_map, per1);
							}
							else
							{
								per1_activity_prev = nullptr;
								per1_strActID_prev = "";
							}
#pragma endregion

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

#ifdef  Investigate_Feasibility
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
#pragma region Setting prev and next activities
							per1_activity = *per1_act_itr;
							auto per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);
							auto per1_act_itr_next = std::next(per1_act_itr);

							if (per1_act_itr_next != per1_activities->end())
							{
								per1_activity_next = *per1_act_itr_next;
								per1_strActID_next = GetstrActID(per1_activity_next, act_ID_map, per1);
							}
							else
							{
								per1_activity_next = nullptr;
								per1_strActID_next = "";
							}

							if (per1_activity != *(per1_activities->begin()))
							{
								per1_activity_prev = *(std::prev(per1_act_itr));
								per1_strActID_prev = GetstrActID(per1_activity_prev, act_ID_map, per1);
							}
							else
							{
								per1_activity_prev = nullptr;
								per1_strActID_prev = "";
							}
#pragma endregion

							for (auto per2_itr = persons->begin(); per2_itr != persons->end(); per2_itr++)
							{
								auto per2 = (*per2_itr);
								_person_static_properties_itf* per2_properties = per2->Static_Properties<_person_static_properties_itf*>();
								_scheduler_itf* per2_scheduler = per2->Scheduling_Faculty<_scheduler_itf*>();
								_activity_container_itf* per2_activities = per2_scheduler->Activity_Container<_activity_container_itf*>();

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
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->Start_Time<Time_Minutes>());
											model.addConstr((grb_Vars["start_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");
											model.addConstr((grb_Vars["start_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_S_" + per2_strActID + "_E");
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["start_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_S");
										}


										it = grb_Vars.find("t_AV_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");
										if (it != grb_Vars.end())
										{
											auto tt = Get_Travel_Time(per1_activity, per2_activity, per1_activity->End_Time<Time_Minutes>());
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) <= 1 + 10000 - ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV1_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");
											model.addConstr((grb_Vars["end_" + per1_strActID]) + tt.Value - (grb_Vars["end_" + per2_strActID]) >= -1 - 10000 + ((it->second)) * (0.0001 + 10000), "c2_Feasib_AV2_" + strVehID + "_" + per1_strActID + "_E_" + per2_strActID + "_E");
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
#endif //  Investigate_Feasibility

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
#pragma region Setting prev and next activities
							per1_activity = *per1_act_itr;
							auto per1_strActID = GetstrActID(per1_activity, act_ID_map, per1);
							auto per1_act_itr_next = std::next(per1_act_itr);

							if (per1_act_itr_next != per1_activities->end())
							{
								per1_activity_next = *per1_act_itr_next;
								per1_strActID_next = GetstrActID(per1_activity_next, act_ID_map, per1);
							}
							else
							{
								per1_activity_next = nullptr;
								per1_strActID_next = "";
							}

							if (per1_activity != *(per1_activities->begin()))
							{
								per1_activity_prev = *(std::prev(per1_act_itr));
								per1_strActID_prev = GetstrActID(per1_activity_prev, act_ID_map, per1);
							}
							else
							{
								per1_activity_prev = nullptr;
								per1_strActID_prev = "";
							}
#pragma endregion

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
										Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1->template Home_Location<_activity_location_itf*>(), per1_activity->Start_Time<Time_Minutes>());

										//	#TODO: make sure we do not double charge this.This should be removed when I know how many people are in the car, and charge empty vehicles instead of looking at whether their last stop was the last activity!!!!
										//	#just charge ZOV tax if the vehilce is sent home from an activity other than the last activity
										if (per1_activity_next)
											Obj_LinExpr += var * cost_ZOV_fixed_tax;
									}

									//traveling to parking(to save parking cost)
									if (key.substr(key.size() - 12) == "_" + per1_strActID + "_S_Prk_S")
									{
										Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1_activity->Location<_activity_location_itf*>(), per1->template Home_Location<_activity_location_itf*>(), per1_activity->Start_Time<Time_Minutes>());
										Obj_LinExpr += var * cost_ZOV_fixed_tax;
									}
								}
								if (per1_activity_next)
								{
									//entering the system
									if (key.substr(key.size() - 12) == "_Hom_S_" + per1_strActID + "_E")
									{
										//fixed Cost of new AV in the system
										Obj_LinExpr += var * cost_AV_fixed;
										Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per1_activity->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>());
										//TODO : charge the ZOV tax if it enters an activity location other than the first one.make sure it is not double counted when empty vehicles are being taxed.
										if (per1_activity_prev)
											Obj_LinExpr += var * cost_ZOV_fixed_tax;
									}

									//returning from parking to an act location
									if (key.substr(key.size() - 11) == "Prk_E_" + per1_strActID + "_E")
									{
										//fixed Cost of new AV in the system
										Obj_LinExpr += var * cost_AV_energy * Get_Travel_Time(per1->template Home_Location<_activity_location_itf*>(), per1_activity->Location<_activity_location_itf*>(), per1_activity->End_Time<Time_Minutes>());
										if (per1_activity_prev)
											Obj_LinExpr += var * cost_ZOV_fixed_tax;
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
									Obj_LinExpr += var * cost_parking *  per1_activity->Duration<Time_Minutes>();
								}
							}
#pragma endregion

#ifdef Investigate_Feasibility

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
												double parking_time = max(0.0, per2_activity->End_Time<Time_Minutes>() - per1_activity->Start_Time<Time_Minutes>() - tt);
												Obj_LinExpr += var * cost_parking * parking_time;
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
#endif
						}
					}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Set objective: maximize x + y + 2 z
					int coutner = 0;
					//for (auto x = Obj_LinExpr.getVar(coutner); coutner< Obj_LinExpr.size(); coutner++;)
					//{
					//	//cout << (*x) << endl;
					//	coutner++;
					//}
					
					model.setObjective(Obj_LinExpr, GRB_MAXIMIZE);
					model.update();
					//model.write("C:\\Mahmoud\\" + std::to_string(floor(HHID)) + "_test.txt");
					auto HHID = this->_Parent_Household->uuid<long long>();
					ofstream myfile;
					model.write("C:\\Mahmoud\\model_" + to_string(HHID) + ".lp");
					model.optimize();

					//cout << x.get(GRB_StringAttr_VarName) << " "  << x.get(GRB_DoubleAttr_X) << endl;
					auto status_code = model.get(GRB_IntAttr_Status);
					if (status_code == GRB_OPTIMAL)
					{
						//void Generate_Activities_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
						Generate_Activities_File(grb_Vars, act_ID_map);
						Generate_Trips_File(grb_Vars, act_ID_map);
						//Generate_Results_File(grb_Vars, act_ID_map);
						Generate_Results2_File(model);
					}
					else if (status_code == GRB_INFEASIBLE)
					{
						cout << "Infeasible or unbounded! " << endl;
					}
					else 
					{
						cout << "Optimization was stopped with status = " << status_code << endl;
					}
				}
				catch (GRBException e)
				{
					cout << "Error code = " << e.getErrorCode() << endl;
					cout << e.getMessage() << endl;
				}
				catch (...)
				{
					cout << "Exception during optimization" << endl;
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

				//Vehicle_ID: parseInt(d["Vehicle_ID"]), From_Act: d["From_Act_Loc"], From_Act_OSE: d["OSE"], To_Act: d["To_Act_Loc"], To_Act_DSE: d["DSE"]};
				ofstream myfile;
				myfile.open("c:\\Mahmoud\\Trips_" + to_string(HHID) + ".txt");
				std::stringstream file_content;				
				file_content << "Term,Vehicle_ID,From_Act_Loc,OSE,To_Act_Loc,DSE\n";
				_household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<_household_static_properties_itf*>();
				
				//auto HHID = household_properties->ID<float>();
				
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
				//auto HHID = household_properties->ID<float>();
				auto HHID = this->_Parent_Household->uuid<long long>();
				ofstream myfile;
				myfile.open("c:\\Mahmoud\\Activities_" + to_string(HHID) + ".txt");
				std::stringstream file_content;
				//file_content << "Houehols_ID,Person_ID,Activity_ID,Activity_Order,Activity_Location,Start,End,Min_Start,Max_Start,Min_Duration,Max_Duration,New_Start,New_End,New_Duration\n";
				file_content << "Houehols_ID,Person_ID,Activity_ID,Start,End,New_Start,New_End\n";
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
						file_content << HHID << "," << perID << "," << act_ID_map[key] << "," << start	 << "," << end << "," << new_start << "," << new_end << endl;
					}
				}
				myfile << file_content.rdbuf();
				myfile.close();
			}

			void Generate_Results_File(map<string, GRBVar>& grb_Vars, map<pair<long, int>, string>& act_ID_map)
			{
				auto HHID = this->_Parent_Household->uuid<long long>();

				ofstream myfile;
				myfile.open("c:\\Mahmoud\\results_" + to_string(HHID) + ".txt");
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

				ofstream myfile;
				myfile.open("c:\\Mahmoud\\results2_" + to_string(HHID) + ".txt");
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


	};
		//template<typename MasterType, typename InheritanceList> typename IntraHousehold_AV_Assignment_Implementation <MasterType, InheritanceList>::type_of(is_initialized) IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::_is_initialized = false;
		
		//template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_travel_time = 1;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_act_dur = 5;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_start_flex = 15;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::min_dur_flex = 15;
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_AV_fixed = -0.0;				//fixed cost of each AV in the system
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_parking = -0.0 / 60.0;		//$0.5 per hour
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_AV_energy = -0.0 / 60.0;		//Cost of AV : $5 per hour(0.0833 per minute)
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_vot = -10 / 60.0;				//Person time cost : $10 / hr($0.167 / min) When changes to the activity start and duration is made
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_taxi_fixed = -20.00;			//#cost of taxi : $2
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_taxi_by_minute = -240.0 / 60;	//#$24 per hour($0.4 per minute)
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_ZOV_tax = -0.0 / 60.0;		//  #$1 per hour
		template<typename MasterType, typename InheritanceList> double IntraHousehold_AV_Assignment_Implementation<MasterType, InheritanceList>::cost_ZOV_fixed_tax = -0 / 100;

	}
}