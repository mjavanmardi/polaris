#pragma once
#include "Activity_Prototype.h"


namespace Activity_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		//=======================================================================
		// Activity Classes
		//-----------------------------------------------------------------------
		implementation struct Basic_Activity_Plan_Implementation : public Polaris_Component<APPEND_CHILD(Basic_Activity_Plan_Implementation),MasterType,Execution_Object,ParentType>
		{
			//=================================================================
			// Pointer back to planner
			member_prototype(Person_Components::Prototypes::Person_Planner, Parent_Planner, typename MasterType::person_planner_type, none, none);
			member_prototype(Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, movement_plan, typename MasterType::movement_plan_type,none,none);


			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
			define_component_interface(_properties_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename type_of(Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Chooser),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_activity_plans_container_itf,_activity_plan_itf, typename type_of(Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(_movement_plans_container_itf,_movement_plan_itf, typename type_of(Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================


			////=================================================================
			//// progress counters
			//static int Location_Planning_Counter[_num_threads];
			//static member_data(int, Location_Planning_Count,none,none);
			//static int Route_Planning_Counter[_num_threads];
			//static member_data(int, Route_Planning_Count,none,none);
			//static int Scheduling_Counter[_num_threads];
			//static member_data(int, Scheduling_Count,none,none);
			//member_data(_lock, update_lock, none, none);

		
			// Fundamental activity properties
			member_data(int, Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(Types::ACTIVITY_TYPES, Activity_Type,none,none);

			// Activity attributes
			member_prototype(Activity_Location_Components::Prototypes::Activity_Location_Prototype, Location, typename MasterType::activity_location_type, check(ReturnValueType,Activity_Location_Components::Concepts::Is_Activity_Location), check(SetValueType,Activity_Location_Components::Concepts::Is_Activity_Location));
			member_data(Vehicle_Components::Types::Vehicle_Type_Keys, Mode, none, none);
			member_component_and_feature_accessor(Start_Time, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_and_feature_accessor(Duration, Value, Basic_Units::Prototypes::Time_Prototype,Basic_Units::Implementations::Time_Implementation<NT>)
			member_component_feature(Expected_Travel_Time, Value, Basic_Units::Prototypes::Time_Prototype, Basic_Units::Implementations::Time_Implementation<NT>);
			member_container(vector<typename MasterType::person_type*>, Involved_Persons_Container, none, none);

			// Planning event time members
			member_data(Revision,Location_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Mode_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Start_Time_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Duration_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Involved_Persons_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));
			member_data(Revision,Route_Planning_Time,check_2(strip_modifiers(ReturnValueType),Revision,is_same), check_2(strip_modifiers(SetValueType),Revision,is_same));

			// Basic Activity Events - Plan route and add to schedule
			feature_implementation void Initialize(TargetType activity_type)
			{
				UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->Activity_Type<TargetType>(activity_type);
				pthis->Duration<Time_Seconds>(END);
				pthis->Start_Time<Time_Seconds>(END);
				pthis->Location<_activity_location_itf*>(nullptr);
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}
			tag_feature_as_available(Initialize);

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END;
				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END;
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END;
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END;
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = END;
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = std::min(_iteration+1, (int)planning_time);
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 5;
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Add_Activity_To_Schedule_Event_Handler()
			{
				// updates for counters
				//this->Scheduling_Counter[_thread_id]++;
				//if (this->Scheduling_Counter[_thread_id] % 10000 == 0)  
				//{
				//	LOCK(this->_update_lock);
				//	this->_Scheduling_Count+=10000;
				//	cout << '\r' << "Scheduling: " <<  this->_Scheduling_Count << "                                 ";
				//	UNLOCK(this->_update_lock);
				//}

				_planning_itf* planner = this->Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_movement_plan_itf* move = this->movement_plan<ComponentType,CallerType,_movement_plan_itf*>();
				

				// Assign the movement plan to the persons activity schedule
				if (move->valid_trajectory<bool>())
				{
					planner->template Add_Movement_Plan<_movement_plan_itf*>(move);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT SCHEDULED, no valid route found from origin to destination: "<< person->template uuid<int>();
					s << "," <<move->template origin<_activity_location_itf*>()->uuid<int>() << ", " <<move->template destination<_activity_location_itf*>()->uuid<int>()<<endl;
					planner->Write_To_Log<NT>(s);
				}
			}
			feature_implementation void Route_Planning_Event_Handler()
			{
				//// updates for counters
				//this->Route_Planning_Counter[_thread_id]++;
				//if (this->Route_Planning_Counter[_thread_id] % 10000 == 0)  
				//{
				//	LOCK(this->_update_lock);
				//	this->_Route_Planning_Count+=10000;
				//	cout << '\r' << "Route Planning: " <<  this->_Route_Planning_Count<< "                                 ";
				//	UNLOCK(this->_update_lock);
				//}


				// Create movement plan and give it an ID
				_movement_plan_itf* move = (_movement_plan_itf*)Allocate<typename type_of(Parent_Planner)::type_of(Movement_Plans_Container)::unqualified_value_type>();
				move->template initialize_trajectory<NULLTYPE>();
				move->template activity_reference<ComponentType*>((ComponentType*)this);

				// Get the origin and destination locations
				_planning_itf* planner = this->Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->template Parent_Person<_person_itf*>();
				_dest_choice_itf* dest_chooser = planner->template Destination_Chooser<_dest_choice_itf*>();

				_activity_location_itf* orig = person->template Home_Location<_activity_location_itf*>();
				_activity_location_itf* dest = this->Location<ComponentType,CallerType,_activity_location_itf*>();

				this->movement_plan<ComponentType,CallerType,_movement_plan_itf*>(move);

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					// If the trip is valid, assign to a movement plan and add to the schedule
					if (orig->internal_id<int>() != dest->internal_id<int>() && orig->origin_links<_links_container_itf&>().size() != 0 && dest->origin_links<_links_container_itf&>().size() != 0)
					{
						// add attributes to plan
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);

						move->template origin<_link_itf*>(orig->origin_links<_links_container_itf&>().at(0));
						move->template destination<_link_itf*>(dest->origin_links<_links_container_itf&>().at(0));
						move->template departed_time<Simulation_Timestep_Increment>(this->Start_Time<ComponentType,CallerType,Simulation_Timestep_Increment>());

						// Add to plans schedule
						planner->template Schedule_New_Routing<_movement_plan_itf*>(_iteration+1, move);
					}
					else
					{
						move->template origin<_activity_location_itf*>(orig);
						move->template destination<_activity_location_itf*>(dest);
					}
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT SCHEDULED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					planner->template Write_To_Log<NT>(s);
					//----------------------------------------------------------------
				}
			}
		};



		// Specialization for ADAPTS-style activities with plan horizon info and more detailed set_plan_times function
		implementation struct ADAPTS_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Activity_Plan_Implementation)> base_type;
			typedef Prototypes::Activity_Planner<base_type> base_itf;

			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
			define_component_interface(_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename get_type_of(base_type::Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Chooser),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_activity_plans_container_itf,_activity_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(_movement_plans_container_itf,_movement_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
		
			member_data_component(Basic_Units::Implementations::Time_Implementation<NT>,_Activity_Planning_Time,none,none);
			member_component_feature(Activity_Planning_Time, _Activity_Planning_Time, Value, Basic_Units::Prototypes::Time_Prototype)

			// Activity attribute planning properties		
			member_data(Types::PLAN_HORIZON_VALUES, Activity_Plan_Horizon,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Location_Plan_Horizon, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, none, none);
			member_data(Types::PLAN_HORIZON_VALUES, Mode_Plan_Horizon, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, none, none);
			member_data(Types::PLAN_HORIZON_VALUES, Start_Time_Plan_Horizon,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Duration_Plan_Horizon,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility,none,none);
			member_data(Types::PLAN_HORIZON_VALUES, Involved_Persons_Plan_Horizon,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility,none,none);

			// Activity methods
			feature_implementation void Initialize(TargetType activity_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->Activity_Type<TargetType>(activity_type);
				pthis->Duration<Time_Seconds>(END);
				pthis->Start_Time<Time_Seconds>(END);
				pthis->Location<_activity_location_itf*>(nullptr);
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_itf* base_this = (base_itf*)this;

				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 1;
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1,(int)planning_time);
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 2;
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 3;
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 4;
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 5;
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Location_Planning_Event_Handler()
			{
				// updates for counters
				//this->Location_Planning_Counter[_thread_id]++;
				//if (this->Location_Planning_Counter[_thread_id] % 10000 == 0)  
				//{
				//	//LOCK(this->_update_lock);
				//	this->_Location_Planning_Count+=10000;
				//	cout << '\r' << "Location Planning: " << this->_Location_Planning_Count << "                                 ";
				//	UNLOCK(this->_update_lock);
				//}


				_planning_itf* planner = this->Parent_Planner<ComponentType,CallerType,_planning_itf*>();
				_person_itf* person = planner->Parent_Person<_person_itf*>();
				_dest_choice_itf* dest_chooser = planner->Destination_Chooser<_dest_choice_itf*>();

				_activity_location_itf* orig = person->Home_Location<_activity_location_itf*>();

				//cout << endl << "schedule location origin: "<<orig->uuid<int>();

				_activity_location_itf* dest = dest_chooser->Choose_Destination<_activity_location_itf*>(orig);

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					this->Location<ComponentType,CallerType,_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					planner->Write_To_Log<NT>(s);
					//----------------------------------------------------------------
				}
			}

			feature_implementation void Mode_Planning_Event_Handler()
			{
				
			}

			feature_implementation void Duration_Planning_Event_Handler()
			{
				
			}

			feature_implementation void Start_Time_Planning_Event_Handler()
			{
				// see if there is already an activity in the morning
				_activity_plans_container_itf* plans = _Parent_Planner->Activity_Container<_activity_plans_container_itf*>();

				_activity_plans_container_itf::iterator plan_itr = plans->begin();
				bool has_AM_start = true;
				for (;plan_itr != plans->end(); ++plan_itr)
				{
					_activity_plan_itf* act = *plan_itr;
					if (act->Start_Time<Time_Hours>() < 12.0) has_AM_start = false;
				}

				int i=1;
				if (has_AM_start) i=0;
			
				double t = Normal_RNG.Next_Rand<double>(7.0*(i+1) + 3.0*i, 3.0+i);		

				Time_Seconds time = Simulation_Time.Future_Time<Time_Hours, Time_Seconds>(t);
				Time_Seconds time_min = Simulation_Time.Future_Time<Time_Seconds,Time_Seconds>(_Parent_Planner->Planning_Time_Increment<Time_Seconds>());
				this->Start_Time<ComponentType,CallerType,Time_Seconds>(std::max<int>(time.Value,time_min.Value));

				// start routing on the planning timestep prior to departure 
				int start_minutes = this->Start_Time<ComponentType,CallerType,Time_Minutes>();
				int start_increment = std::max<int>(Simulation_Time.Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes) - _Parent_Planner->Planning_Time_Increment<Simulation_Timestep_Increment>(),_iteration);
				this->Route_Planning_Time<ComponentType,CallerType,Revision&>()._iteration = start_increment;
			}

			feature_implementation void Involved_Persons_Planning_Event_Handler()
			{
				
			}

			//feature_implementation void Set_Attribute_Planning_Times()
			//{
			//	//Create local variables
			//	int ACT1, ACT2, ACT3, ACT4, ACT5, Employed, Student, Male, Senior, TELEWORK, ICT_USE;
			//	ACT1 = 0; ACT2 = 0; ACT3 = 0; ACT4 = 0; ACT5 = 0; Employed = 0; Student = 0; Male = 0; Senior = 0; TELEWORK = 0; ICT_USE = 0;
			//	float AvgDur, AvgFreq;

			//	if (_Activity_Type == WORK_AT_HOME_ACTIVITY || _Activity_Type == PRIMARY_WORK_ACTIVITY || _Activity_Type == SCHOOL_ACTIVITY || _Activity_Type == OTHER_WORK_ACTIVITY) ACT1 = 1;
			//	if (_Activity_Type == HEALTHCARE_ACTIVITY || _Activity_Type == RELIGIOUS_OR_CIVIC_ACTIVITY || _Activity_Type == PERSONAL_BUSINESS_ACTIVITY) ACT2 = 1;
			//	if (_Activity_Type == Types::ERRANDS_ACTIVITY || _Activity_Type == PICK_UP_OR_DROP_OFF_ACTIVITY || _Activity_Type == SERVICE_VEHICLE_ACTIVITY) ACT3 = 1;
			//	if (_Activity_Type == Types::LEISURE_ACTIVITY|| _Activity_Type == Types::EAT_OUT_ACTIVITY || _Activity_Type == Types::RECREATION_ACTIVITY || _Activity_Type == Types::SOCIAL_ACTIVITY) ACT4 = 1;
			//	if (_Activity_Type == Types::OTHER_SHOPPING_ACTIVITY || _Activity_Type == Types::MAJOR_SHOPPING_ACTIVITY) ACT5 = 1;
			//	if (PER.PersonData.Employed) Employed = 1;
			//	if (PER.PersonData.isStudent) Student = 1;
			//	if (PER.PersonData.Gender == GenType.Male) Male = 1;
			//	if (PER.PersonData.Age >= 65) Senior = 1;
			//	if (PER.PersonData.ICT_Use != IctType.NULL || PER.PersonData.ICT_Use != IctType.UseLow) ICT_USE = 1;
			//	if (PER.PersonData.Teleworker == true) TELEWORK = 1;
			//	AvgFreq = PER.PersonData.ActFreqDurGen[(int)Type, 0];
			//	AvgDur = PER.PersonData.ActFreqDurGen[(int)Type, 1];


			//	////////////////////////////////////////////////////////////////////////////////////////////
			//	/////// FLEXIBILITY CALCULATIONS ///////////////////////////////////////////////////////////
			//	double[,] alpha = { { -99999999, -99999999, -99999999, -99999999 }, { 0, 0, 0, 0 }, { 99999999, 99999999, 99999999, 99999999 } };
			//	double[,] sigma = { { 1, 0, 0, 0, 0 }, {0.099646,1,0,0,0}, {-0.053507,-0.03372,1,0,0}, {0.095299,0.305332,-0.113887,1,0}, {0.124233,0.168151,0.025553,-0.099794,1} }; // covariance matrix
			//	double[] P_per = new double[2]; // Holds probability for each level (index by i) of each response (index by j)
			//	double[] P_mod = new double[2];
			//	double[] P_loc = new double[2];
			//	double[] P_tim = new double[2];
			//	double[] P_dur = new double[2];

			//	//Calculate xB values for each response
			//	double xB_mod = -0.383692 + 0.362839 * Employed + 0.4215 * Student - 0.217904 * Male - 0.267344 * Senior + 0.691999 * Male * Senior + 0.231177 * AvgFreq + 0.568976 * ACT1 * ICT_USE - 1.039075 * ACT1 * AvgFreq;
			//	double xB_per = -0.338294 + 0.347752 * Student - 0.536749 * Male + 0.259776 * Senior + 0.478152 * Male * Senior + 0.532325 * ICT_USE + 1.508163 * TELEWORK - 1.655547 * ICT_USE * TELEWORK - 1.390818 * ACT1 * +1.352461 * ACT1 * TELEWORK - 0.598393 * ACT2 + 11.034344 * ACT2 * AvgDur - 0.617948 * ACT3 + 1.727868 * AvgFreq * ACT3 + 0.437761 * AvgFreq * ACT4 + 6.708181 * ACT4 * AvgDur - 0.606538 * ACT5 + 0.325567 * AvgFreq * ACT5;
			//	double xB_loc = -0.368786 - 1.174171 * Male - 0.1914 * Senior + 1.346607 * Male * Senior - 0.3674 * ICT_USE + 0.99392 * TELEWORK - 0.815509 * ICT_USE * TELEWORK + 1.023933 * ACT1 * TELEWORK + 2.325112 * ACT1 * AvgFreq - 8.966545 * ACT2 * AvgDur - 1.02207 * AvgFreq * ACT3 + 0.904036 * ACT4 + 0.637477 * AvgFreq * ACT4 - 8.32642 * ACT4 * AvgDur + 2.382859 * ACT5;
			//	double xB_tim = 1.326259 - 0.38711 * Male - 0.226637 * Senior + 0.484623 * Male * Senior - 0.302255 * ICT_USE + 0.196488 * TELEWORK + 3.3758 * ACT2 * AvgDur - 0.400564 * ACT3 + 1.301273 * AvgFreq * ACT3 - 0.601686 * ACT5 + 0.192867 * AvgFreq * ACT5;
			//	double xB_dur = -1.239259 + 1.294471 * Employed + 1.461113 * Student - 0.246144 * Senior - 0.374409 * Male * Senior - 0.23527 * ICT_USE * TELEWORK + 1.30007 * AvgDur + 0.621008 * ACT1 * +0.994951 * ACT1 * ICT_USE - 1.124459 * ACT1 * TELEWORK - 0.56222 * ACT1 * AvgFreq - 2.070631 * ACT1 * AvgDur - 0.167278 * ICT_USE * ACT4 + 0.769745 * AvgFreq * ACT4 + 0.487925 * AvgFreq * ACT5;			

			//	// Get probabilities of each response level
			//	for (int i = 0; i < 2; i++)
			//	{
			//		P_mod[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha[i + 1, 0] - xB_mod, 0, 1);
			//		P_per[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha[i + 1, 0] - xB_per, 0, 1);
			//		P_loc[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha[i + 1, 1] - xB_loc, 0, 1);
			//		P_tim[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha[i + 1, 2] - xB_tim, 0, 1);
			//		P_dur[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha[i + 1, 3] - xB_dur, 0, 1);
			//	}

			//	// Set correlated random variables
			//	double[] rand = Troschuetz.Random.ContinuousUniformDistribution.MultivariateUniform(sigma, GLOBALS.STDNORM[ThreadNum]);

			//	// Set flexibility values
			//	for (int i = 0; i < 2; i++)
			//	{
			//		if (rand[0] < P_mod[i])
			//		{
			//			FlexMod = (ActFlex)i;
			//			rand[0] = 999;
			//		}
			//		if (rand[1] < P_per[i])
			//		{
			//			FlexPer = (ActFlex)i;
			//			rand[1] = 999;
			//		}
			//		if (rand[2] < P_loc[i])
			//		{
			//			FlexLoc = (ActFlex)i;
			//			rand[2] = 999;
			//		}
			//		if (rand[3] < P_tim[i])
			//		{
			//			FlexStart = (ActFlex)i;
			//			rand[3] = 999;
			//		}
			//		if (rand[4] < P_dur[i])
			//		{
			//			FlexDur = (ActFlex)i;
			//			rand[4] = 999;
			//		}
			//	}
   //         
			//	// Set results of flexiblity model to use in next models
			//	double IFLOC, IFTIM, IFDUR, IFMOD;
			//	IFMOD = P_mod[0];
			//	IFLOC = P_loc[0];
			//	IFTIM = P_tim[0];
			//	IFDUR = P_dur[0];


			//	////////////////////////////////////////////////////////////////////////////////////////////
			//	//////////////// Activity Plan Horizon Calculations ////////////////////////////////////////
			//	double xB = 0.130 + 0.640303 * IFLOC - 0.63621 * IFTIM - 1.498554 * IFDUR + 0.767701 * Employed + 0.485362 * ICT_USE
			//		- 0.560652 * TELEWORK + 1.049907 * ACT1 - 0.446779 * ACT1 * AvgFreq - 0.789474 * ACT1 * IFLOC + 2.241387 * ACT1 * IFDUR
			//		- 0.892963 * ACT1 * Employed + 1.717603 * ACT1 * Student + 15.248565 * ACT2 * AvgDur + 1.497258 * IFDUR * ACT2
			//		- 0.613958 * ICT_USE * ACT2 + 1.573672 * AvgFreq * ACT3 + 0.633234 * IFDUR * ACT3 - 0.745918 * Employed * ACT3
			//		- 1.115487 * Student * ACT3 + 1.049812 * ACT3 * Senior - 0.593425 * ACT3 * Male + 0.519961 * AvgFreq * ACT4 + 2.200494 * AvgDur * ACT4
			//		+ 1.403807 * IFDUR * ACT4 + 0.837724 * Student * ACT4 + 0.681377 * Senior * ACT4 - 0.793118 * Male * ACT4 - 0.432584 * ICT_USE * ACT4
			//		+ 0.289143 * AvgFreq * ACT5 + 1.128966 * IFDUR * ACT5 - 0.610606 * Employed * ACT5 + 0.415577 * Senior * ACT5
			//		- 0.752066 * ICT_USE * ACT5;
			//	double Pimp, Pday, Pweek;
			//	double a1 = 1.646398;
			//	double a2 = 3.505000;
			//	double r = GLOBALS.RAND[PER.GetThreadID].NextDouble();

			//	// calculate probabilities
			//	Pimp = 1.0 / (1.0 + Math.Exp(xB));
			//	Pday = 1.0 / (1.0 + Math.Exp(-a1 + xB));
			//	Pweek = 1.0 / (1.0 + Math.Exp(-a2 + xB));

			//	// Set activity plan horizon
			//	if (r < Pimp) ActPlanHorizon = ActHorizon.IMPULSIVE;
			//	else if (r < Pday) ActPlanHorizon = ActHorizon.SAMEDAY;
			//	else if (r < Pweek) ActPlanHorizon = ActHorizon.SAMEWEEK;
			//	else ActPlanHorizon = ActHorizon.PREPLAN;


			//	//////////////////////////////////////////////////////////////////////////////////////////////////
			//	/////// ATT. PLAN HORIZON CALCULATIONS ///////////////////////////////////////////////////////////
			//	//double PLANIMP = Pimp, PLANSAME = Pday, PLANWEEK = Pweek, PLANONTOUR;
			//	double PLANIMP = Pimp, PLANSAME = Pday-Pimp, PLANWEEK = Pweek-Pday, PLANONTOUR;
			//	PLANONTOUR = PLANIMP * 1;

			//	double[,] alpha2 = { { -99999999, -99999999, -99999999, -99999999, -99999999 }, { 0, 0, 0, 0, 0 }, { 0.835415, 2.029384, 1.305499, 1.151026, 0.434582 }, { 1.486357, 3.614181, 2.316088, 2.026512, 0.879016 }, { 1.794494, 4.401609, 2.796206, 2.643064, 1.095505 }, { 99999999, 99999999, 99999999, 99999999, 99999999 } };
			//	double[,] sigma2 = { { 1.0000, 0.0000, 0.0000, 0.0000, 0.0000 }, { 0.14308, 1, 0, 0, 0 }, { 0.148218, 0.104424, 1, 0, 0 }, { 0.159259, 0.180953, 0.352088, 1, 0 }, { 0.186676, 0.177994, 0.218223, 0.538872, 1 } }; // covariance matrix
			//	//double[,] sigma2 = { { 1.0000, 0.0000, 0.0000, 0.0000, 0.0000 }, { 0, 1, 0, 0, 0 }, { 0,0, 1, 0, 0 }, { 0,0,0, 1, 0 }, { 0,0,0,0, 1 } }; // covariance matrix
			//	double[] P_pmod = new double[5];
			//	double[] P_pper = new double[5]; // Holds probability for each level (index by i) of each response (index by j)
			//	double[] P_ploc = new double[5];
			//	double[] P_ptim = new double[5];
			//	double[] P_pdur = new double[5];

			//	//Calculate xB values for each response
			//	double xB_pmod = 0.618 + 0.262288 * ACT2 + 0.376779 * ACT4 + 0.195715 * ACT5 + 0.575386 * PLANONTOUR - 1.527302 * PLANIMP 
			//					 - 0.492435 * PLANSAME - 0.271618 * PLANWEEK + 0.633625 * Employed + 1.043977 * Student - 0.115081 * Senior
			//					 + 0.106084 * IFDUR + 0.785746 * ACT1 * AvgDur + 0.353502 * PLANWEEK * ACT3 - 3.92232 * ACT4 * AvgDur
			//					 + 0.456054 * ACT1 * TELEWORK + 0.130093 * ICT_USE;
			//	double xB_pper = 4.096 - 5.532536 * PLANIMP - 2.912268 * PLANSAME - 1.231383 * PLANWEEK + 0.825496 * ACT2 * Student
			//					 - 5.690886 * ACT4 * AvgDur + 0.358763 * ACT4 * ICT_USE;
			//	double xB_ploc = 2.079 - 3.2086 * PLANIMP - 1.40061 * PLANSAME - 0.581665 * PLANWEEK + 0.202875 * Employed + 0.350806 * Student
			//					 + 1.846201 * AvgDur + 0.951409 * ACT1 * IFLOC + 0.278635 * ACT4 * IFLOC + 0.839625 * ICT_USE
			//					 - 0.736206 * ACT2 * ICT_USE - 0.692824 * ACT3 * ICT_USE - 1.033742 * ACT4 * ICT_USE - 1.166372 * ACT5 * ICT_USE;
			//	double xB_ptim = 1.612 + 0.206278 * ACT5 - 3.178382 * PLANIMP - 1.570305 * PLANSAME - 0.932677 * PLANWEEK + 0.359391 * Employed
			//					 + 0.163188 * IFDUR + 0.616433 * ACT3 * IFTIM + 0.756115 * ACT1 * TELEWORK - 0.145507 * ICT_USE
			//					 + 0.185875 * ACT4 * ICT_USE - 0.497667 * ACT5 * ICT_USE;
			//	double xB_pdur = -1.264 + 0.323092 * ACT2 - 1.466871 * PLANIMP - 0.518569 * PLANSAME - 0.35598 * PLANWEEK + 0.742175 * Employed
			//					 + 0.157659 * Senior + 0.130234 * Male + 0.297442 * IFTIM + 0.835193 * IFDUR + 0.266187 * AvgFreq
			//					 - 0.395776 * ACT2 * IFDUR - 0.282278 * ACT5 * IFDUR - 0.338128 * ACT5 * IFTIM + 0.879233 * ACT1 * TELEWORK;
			//	// Get probabilities of each response level
			//	for (int i = 0; i < 5; i++)
			//	{
			//		P_pmod[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha2[i + 1, 0] - xB_pmod, 0, 1);
			//		P_pper[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha2[i + 1, 1] - xB_pper, 0, 1);
			//		P_ploc[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha2[i + 1, 2] - xB_ploc, 0, 1);
			//		P_ptim[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha2[i + 1, 3] - xB_ptim, 0, 1);
			//		P_pdur[i] = Troschuetz.Random.NormalDistribution.CumulativeDist(alpha2[i + 1, 4] - xB_pdur, 0, 1);
			//	}

			//	// Set correlated random variables
			//	double[] rand2 = Troschuetz.Random.ContinuousUniformDistribution.MultivariateUniform(sigma2, GLOBALS.STDNORM[ThreadNum]);

			//	// Set attribute plan horizon values
			//	for (int i = 0; i < 5; i++)
			//	{
			//		// convert from i to planhorizon values (skip 'previous day')
			//		int x = i + 1; if (i > 1) x = x + 1;

			//		if (rand2[0] < P_pmod[i])
			//		{
			//			PlanMode = (ActHorizon)x;
			//			rand2[0] = 999;
			//		}
			//		if (rand2[1] < P_pper[i])
			//		{
			//			PlanPer = (ActHorizon)x;
			//			rand2[1] = 999;
			//		}
			//		if (rand2[2] < P_ploc[i])
			//		{
			//			PlanLoc = (ActHorizon)x;
			//			rand2[2] = 999;
			//		}
			//		if (rand2[3] < P_ptim[i])
			//		{
			//			PlanStart = (ActHorizon)x;
			//			rand2[3] = 999;
			//		}
			//		if (rand2[4] < P_pdur[i])
			//		{
			//			PlanDur = (ActHorizon)x;
			//			rand2[4] = 999;
			//		}
			//	}
			//}
		};
		


		// Specialization for ADAPTS-style routine activities
		implementation struct ADAPTS_Routine_Activity_Plan_Implementation : public Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Routine_Activity_Plan_Implementation)>
		{
			typedef Basic_Activity_Plan_Implementation<MasterType,ParentType,APPEND_CHILD(ADAPTS_Routine_Activity_Plan_Implementation)> base_type;
			typedef Prototypes::Activity_Planner<ComponentType,ComponentType> this_itf;
		
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================
			// Interfaces
			define_component_interface(_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_static_properties_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties), Person_Components::Prototypes::Person_Properties, ComponentType);
			define_component_interface(_planning_itf,typename type_of(base_type::Parent_Planner),Person_Components::Prototypes::Person_Planner,ComponentType);
			define_component_interface(_person_itf,typename _planning_itf::get_type_of(Parent_Person),Person_Components::Prototypes::Person,ComponentType);
			define_component_interface(_dest_choice_itf,typename _planning_itf::get_type_of(Destination_Chooser),Person_Components::Prototypes::Destination_Chooser,ComponentType);
			define_component_interface(_scenario_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
			define_component_interface(_network_itf, typename type_of(base_type::Parent_Planner)::type_of(Parent_Person)::type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);	
			define_component_interface(_skim_itf, typename _network_itf::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,ComponentType);
			define_container_and_value_interface(_activity_locations_container_itf, _activity_location_itf, typename _network_itf::get_type_of(activity_locations_container), Random_Access_Sequence_Prototype, Activity_Location_Components::Prototypes::Activity_Location_Prototype, ComponentType);
			define_container_and_value_interface(_links_container_itf, _link_itf, typename _activity_location_itf::get_type_of(origin_links), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
			define_container_and_value_interface(_zones_container_itf, _zone_itf, typename _network_itf::get_type_of(zones_container), Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, ComponentType);
			define_container_and_value_interface_unqualified_container(_activity_plans_container_itf,_activity_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Activity_Container),Containers::Back_Insertion_Sequence_Prototype,Activity_Components::Prototypes::Activity_Planner,ComponentType);
			define_container_and_value_interface_unqualified_container(_movement_plans_container_itf,_movement_plan_itf, typename type_of(base_type::Parent_Planner)::type_of(Movement_Plans_Container),Containers::Back_Insertion_Sequence_Prototype,Movement_Plan_Components::Prototypes::Movement_Plan_Prototype,ComponentType);
			//================================================================================================================================================================================================
			//================================================================================================================================================================================================

			// Fundamental activity properties
			feature_implementation Types::PLAN_HORIZON_VALUES Activity_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Activity_Plan_Horizon);

			// Activity attribute planning properties
			feature_implementation Types::PLAN_HORIZON_VALUES Location_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Location_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Mode_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Mode_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Start_Time_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Start_Time_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Duration_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Duration_Plan_Horizon);
			feature_implementation Types::PLAN_HORIZON_VALUES Involved_Persons_Plan_Horizon()
			{
				return Types::PLAN_HORIZON_VALUES::ROUTINE;
			}
			tag_getter_as_available(Involved_Persons_Plan_Horizon);
			member_data(Types::FLEXIBILITY_VALUES, Location_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Mode_Flexibility, none, none);
			member_data(Types::FLEXIBILITY_VALUES, Start_Time_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Duration_Flexibility,none,none);
			member_data(Types::FLEXIBILITY_VALUES, Involved_Persons_Flexibility,none,none);


			// Activity methods
			feature_implementation void Initialize(TargetType activity_type)
			{
				//UNLOCK(this->_update_lock);
				this_itf* pthis = (this_itf*)this;
				pthis->Activity_Type<TargetType>(activity_type);
				pthis->Duration<Time_Seconds>(END);
				pthis->Start_Time<Time_Seconds>(END);
				pthis->Location<_activity_location_itf*>(nullptr);
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
			}

			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				base_type* base_this = (base_type*)this;

				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Start_Time_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 0;
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Duration_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 1;
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Location_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 2;
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Mode_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 3;
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Involved_Persons_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 4;
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._iteration = min(_iteration+1, (int)planning_time);
				this->Route_Planning_Time<ComponentType, CallerType, Revision&>()._sub_iteration = 5;
			}
			feature_implementation void Set_Attribute_Planning_Times(TargetType planning_time, requires(!check_2(TargetType, Simulation_Timestep_Increment, is_same)))
			{
				assert_check_2(TargetType, Simulation_Timestep_Increment, is_same, "Error: planning_time must be passed as a Simulation_Timestep_Increment type when using this function.");
			}
			tag_feature_as_available(Set_Attribute_Planning_Times);

			feature_implementation void Location_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* base_this = (base_type*)this;
				_person_itf* person = base_this->_Parent_Planner->Parent_Person<_person_itf*>();
				_activity_location_itf* orig = person->Home_Location<_activity_location_itf*>();

				// Select the location based on the activity type
				_activity_location_itf* dest;		
				if (pthis->Activity_Type<Types::ACTIVITY_TYPES>() == Types::AT_HOME_ACTIVITY)
				{
					dest = person->Home_Location<_activity_location_itf*>();
				}
				else if (pthis->Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY)
				{
					dest = person->Work_Location<_activity_location_itf*>();
				}
				else if (pthis->Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					dest = person->School_Location<_activity_location_itf*>();
				}
				else if (pthis->Activity_Type<Types::ACTIVITY_TYPES>() == Types::ACTIVITY_TYPES::WORK_AT_HOME_ACTIVITY)
				{
					dest = person->Home_Location<_activity_location_itf*>();
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, only work, home, school and work at home activities can be routine at this point: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					base_this->_Parent_Planner->Write_To_Log<NT>(s);
					//----------------------------------------------------------------
					return;
				}

				// check that origin and destination are valid
				if (orig != nullptr && dest != nullptr) 
				{
					this->Location<ComponentType,CallerType,_activity_location_itf*>(dest);
				}
				else
				{
					//----------------------------------------------------------------
					// Print to log file
					stringstream s;
					s <<"ACTIVITY NOT GENERATED, null origin or destination: "<< person->template uuid<int>();
					s << "," <<orig << ", " <<dest<<endl;
					base_this->_Parent_Planner->Write_To_Log<NT>(s);
					//----------------------------------------------------------------
				}
			}

			feature_implementation void Mode_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				pthis->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>(Vehicle_Components::Types::SOV);
			}

			feature_implementation void Duration_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->Static_Properties<_static_properties_itf*>();

				Time_Minutes work_duration = static_properties->Work_Hours<Time_Minutes>()/5.0;
				pthis->Duration<Time_Minutes>(work_duration);
			}

			feature_implementation void Start_Time_Planning_Event_Handler()
			{
				this_itf* pthis = (this_itf*)this;
				base_type* bthis = (base_type*)this;

				_person_itf* person = bthis->_Parent_Planner->Parent_Person<_person_itf*>();
				_static_properties_itf* static_properties = person->Static_Properties<_static_properties_itf*>();

				ACTIVITY_TYPES act_type = pthis->Activity_Type<ACTIVITY_TYPES>();

				// School Activity start time (randomly between 7 and 9AM)
				if (act_type == ACTIVITY_TYPES::SCHOOL_ACTIVITY)
				{
					Time_Seconds start_school = (7.0 + Uniform_RNG.Next_Rand<float>() * 2.0) * 60.0 * 60.0;
					Time_Seconds start_min = Simulation_Time.Future_Time<Time_Seconds,Time_Seconds>(_Parent_Planner->Planning_Time_Increment<Time_Seconds>());
					pthis->Start_Time<Time_Seconds>(std::max<int>(start_school.Value,start_min.Value));
				}

				// Work activity start time, based on census data
				else
				{
					Time_Seconds start_work = static_properties->Journey_To_Work_Arrival_Time<Time_Seconds>();
					start_work = start_work + Uniform_RNG.Next_Rand<float>() * 5.0 * 60.0; // add random uniform time between 0 and 5 minutes since ACS start is rounded to 5 min
					Time_Seconds start_min = Simulation_Time.Future_Time<Time_Seconds,Time_Seconds>(_Parent_Planner->Planning_Time_Increment<Time_Seconds>());
					pthis->Start_Time<Time_Seconds>(std::max<int>(start_work.Value,start_min.Value));
				}

				// start routing on the planning timestep prior to departure 
				int start_minutes = this->Start_Time<ComponentType,CallerType,Time_Minutes>();
				int start_increment = std::max<int>(Simulation_Time.Convert_Time_To_Simulation_Timestep<Time_Minutes>(start_minutes) - _Parent_Planner->Planning_Time_Increment<Simulation_Timestep_Increment>(),_iteration);
				this->Route_Planning_Time<ComponentType,CallerType,Revision&>()._iteration = start_increment;
			}

			feature_implementation void Involved_Persons_Planning_Event_Handler()
			{
				
			}

		};



		// Stripped down activity record with minimal memory usage (used for storing and printing completed activities)
		implementation struct Activity_Record : public Polaris_Component<APPEND_CHILD(ADAPTS_Activity_Plan_Implementation), MasterType, Data_Object, ParentType>
		{
			feature_implementation void Initialize(TargetType object,requires(check(TargetType, Concepts::Is_Activity_Plan_Prototype)))
			{
				typedef Activity_Location_Components::Prototypes::Activity_Location_Prototype<MasterType::activity_location_type> location_itf;
				typedef Zone_Components::Prototypes::Zone_Prototype<MasterType::zone_type> zone_itf;
				typedef Prototypes::Activity_Planner<TargetType::Component_Type, TargetType::Caller_Type> object_itf;
				object_itf* obj = (object_itf*)object;

				this->Activity_Plan_ID<char>(obj->Activity_Plan_ID<char>());
				this->Activity_Type<char>((char)obj->Activity_Type<Types::ACTIVITY_TYPES>());
				this->Location<int>(obj->Location<location_itf*>()->zone<zone_itf*>()->uuid<int>());
				this->Duration<Time_Minutes_Short>(obj->Duration<Time_Minutes_Short>());
				this->Start_Time<Time_Minutes_Short>(obj->Start_Time<Time_Minutes_Short>());
				this->Travel_Time<Time_Minutes_Short>(obj->Travel_Time<Time_Minutes_Short>());
			}

			// Fundamental activity properties
			member_data(char, Activity_Plan_ID, check(ReturnValueType,is_arithmetic), check(SetValueType,is_arithmetic));
			member_data(char, Activity_Type,none,none);
			member_data(int, Location, none,none);
			member_data(Time_Minutes_Short, Start_Time, check_2(ReturnValueType,Time_Minutes_Short, is_same),check_2(SetValueType,Time_Minutes_Short, is_same));
			member_data(Time_Minutes_Short, Duration, check_2(ReturnValueType,Time_Minutes_Short, is_same),check_2(SetValueType,Time_Minutes_Short, is_same));
			member_data(Time_Minutes_Short, Travel_Time, check_2(ReturnValueType,Time_Minutes_Short, is_same),check_2(SetValueType,Time_Minutes_Short, is_same));
		};



		//=======================================================================
		// Static activity plan member initialization
		//-----------------------------------------------------------------------
		//static_array_definition(Basic_Activity_Plan_Implementation, Location_Planning_Counter, int);
		//static_array_definition(Basic_Activity_Plan_Implementation, Route_Planning_Counter, int);
		//static_array_definition(Basic_Activity_Plan_Implementation, Scheduling_Counter, int);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Location_Planning_Count, 0);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Route_Planning_Count, 0);
		//static_member_initialization(Basic_Activity_Plan_Implementation, Scheduling_Count, 0);
	}
}
