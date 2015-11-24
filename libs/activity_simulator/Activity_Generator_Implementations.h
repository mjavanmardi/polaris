#pragma once

//#include "Person_Prototype.h"
#include "Activity_Prototype.h"
#include "Activity_Generator_Prototype.h"

using namespace Activity_Components::Types;

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Planning classes
		//----------------------------------------------------------------------------------
		implementation struct General_Activity_Generator_Implementation : public Polaris_Component<MasterType,INHERIT(General_Activity_Generator_Implementation),Data_Object>
		{
			// Tag as implementation
			typedef typename Polaris_Component<MasterType,INHERIT(General_Activity_Generator_Implementation),Data_Object>::Component_Type ComponentType;

			// Pointer to the Parent class
			m_prototype(Prototypes::Person_Planner,typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);

			// Interface definitions
			typedef Prototypes::Person< typename type_of(Parent_Planner)::type_of(Parent_Person)> _Person_Interface;
			typedef Household_Components::Prototypes::Household< typename _Person_Interface::get_type_of(Household)> _Household_Interface;
			typedef Prototypes::Person_Scheduler< typename _Person_Interface::get_type_of(Scheduling_Faculty)> _Scheduler_Interface;
			
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;

			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;

			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Back_Insertion_Sequence< typename _Scheduler_Interface::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Basic_Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename _Scheduler_Interface::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;

			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::routine_activity_plan_type> Routine_Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type> Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> At_Home_Activity_Plan;

			template<typename TargetType> TargetType Parent_Person()
			{
				return _Parent_Planner->Parent_Person<TargetType>();
			}
			template<typename TargetType> TargetType Scheduling_Faculty()
			{
				_Person_Interface* person = _Parent_Planner->Parent_Person<_Person_Interface*>();
				return person->Scheduling_Faculty<_Scheduler_Interface*>();
			}
			int Scheduled_Activity_Count()
			{
				_Person_Interface* person = _Parent_Planner->Parent_Person<_Person_Interface*>();
				return person->Scheduling_Faculty<_Scheduler_Interface*>()->Activity_Count<int>();
			}

			// ACTIVITY CREATION METHODS
			template<typename TargetType> Routine_Activity_Plan* Create_Routine_Activity(TargetType act_type, int& activity_count, int start_plan_time)
			{
				Routine_Activity_Plan* activity = (Routine_Activity_Plan*)Allocate<typename MasterType::routine_activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(activity_count);
	
				// Activity planning time
				Simulation_Timestep_Increment plan_time = start_plan_time + activity_count;
				activity->template Initialize<ACTIVITY_TYPES, Simulation_Timestep_Increment>(act_type, plan_time);

				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<Routine_Activity_Plan*>(activity);
				activity_count++;
				return activity;
			}
			template<typename TargetType> Activity_Plan* Create_Activity(TargetType act_type, int& activity_count, int start_plan_time)
			{
				Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(activity_count);


				Simulation_Timestep_Increment _plan_time = start_plan_time + activity_count;
				activity->template Initialize<ACTIVITY_TYPES, Simulation_Timestep_Increment>(act_type, _plan_time);

				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<Activity_Plan*>(activity);		
				activity_count++;
				return activity;
			}
			template<typename TargetType, typename LocationType> Activity_Plan* Create_Activity(TargetType act_type, int start_plan_time, LocationType location)
			{
				Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(Scheduled_Activity_Count()+1);

				activity->template Initialize<ACTIVITY_TYPES>(act_type, start_plan_time);

				// set location and remove from planning stream
				activity->Location<LocationType>(location);
				activity->Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<Activity_Plan*>(activity);	
				return activity;
			}
			template<typename TargetType, typename LocationType, typename ModeType> Activity_Plan* Create_Activity(TargetType act_type, int start_plan_time, LocationType location, ModeType mode)
			{
				Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);

				activity->template Activity_Plan_ID<int>(Scheduled_Activity_Count()+1);

				activity->template Initialize<ACTIVITY_TYPES>(act_type, start_plan_time);
				//activity->Set_Meta_Attributes<void>();
				// schedule the activity events based on plan times.
				//activity->Set_Attribute_Planning_Times<TimeType>(start_plan_time);

				// set location and remove from planning stream
				activity->Location<LocationType>(location);
				activity->Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->Mode<ModeType>(mode);
				activity->Mode_Planning_Time<Revision&>()._iteration = END+1;
				activity->Mode_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<Activity_Plan*>(activity);	
				return activity;
			}
			template<typename TargetType, typename LocationType, typename ModeType, typename TimeType> Activity_Plan* Create_Activity(TargetType act_type, TimeType start_plan_time, LocationType location, ModeType mode, TimeType start, TimeType duration)
			{
				Activity_Plan* activity = (Activity_Plan*)Allocate<typename MasterType::activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);

				activity->template Activity_Plan_ID<int>(Scheduled_Activity_Count()+1);

				activity->template Initialize<ACTIVITY_TYPES, TimeType>(act_type, start_plan_time);
				//activity->Set_Meta_Attributes<void>();
				// schedule the activity events based on plan times.
				//activity->Set_Attribute_Planning_Times<TimeType>(start_plan_time);

				// set location and remove from planning stream
				activity->Location<LocationType>(location);
				activity->Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->Mode<ModeType>(mode);
				activity->Mode_Planning_Time<Revision&>()._iteration = END+1;
				activity->Mode_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->Start_Time<TimeType>(start);
				activity->Start_Time_Planning_Time<Revision&>()._iteration = END+1;
				activity->Start_Time_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->Duration<TimeType>(duration);
				activity->Duration_Planning_Time<Revision&>()._iteration = END+1;
				activity->Duration_Planning_Time<Revision&>()._sub_iteration = END+1;		

				activity->Schedule_Activity_Events<NT>();
				
				this->_Parent_Planner->template Add_Activity_Plan<Activity_Plan*>(activity);	
				return activity;
			}
			template<typename TargetType> At_Home_Activity_Plan* Create_Home_Activity(int& activity_count)
			{
				At_Home_Activity_Plan* activity = (At_Home_Activity_Plan*)Allocate<typename Master_Type::at_home_activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(activity_count);

				activity->template Initialize<ACTIVITY_TYPES, Simulation_Timestep_Increment, Vehicle_Components::Types::Vehicle_Type_Keys>(ACTIVITY_TYPES::AT_HOME_ACTIVITY,0,0,END,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<At_Home_Activity_Plan*>(activity);		
				activity_count++;
				return activity;
			}
			template<typename ReturnType, typename TimeType, typename ModeType> ReturnType Create_Home_Activity(TimeType departure_time, TimeType start, TimeType duration, ModeType mode)
			{
				At_Home_Activity_Plan* activity = (At_Home_Activity_Plan*)Allocate<typename Master_Type::at_home_activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(Scheduled_Activity_Count()+100);

				activity->template Initialize<ACTIVITY_TYPES, Simulation_Timestep_Increment, Vehicle_Components::Types::Vehicle_Type_Keys>(ACTIVITY_TYPES::AT_HOME_ACTIVITY,departure_time,start,duration,mode);
				activity->Schedule_Activity_Events<NT>();

				this->_Parent_Planner->template Add_Activity_Plan<At_Home_Activity_Plan*>(activity);		

				return (ReturnType)activity;
			}
			
		};


		//======================================================================
		// Competing hazard-based activity generation for implementing full ADAPTS model
		implementation struct ADAPTS_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType,  INHERIT(ADAPTS_Activity_Generator_Implementation)>
		{
			typedef General_Activity_Generator_Implementation<MasterType,  INHERIT(ADAPTS_Activity_Generator_Implementation)> base_type;
			typedef typename base_type::Component_Type ComponentType;

			typedef Prototypes::Activity_Generator<base_type> base_itf;
			typedef Prototypes::Person<typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;

			template<typename TargetType> void Initialize(requires(TargetType,check(typename ComponentType::Parent_Type,Activity_Simulator::Person_Concepts::Is_Person)))
			{	
			}
			template<typename TargetType> void Initialize(requires(TargetType,check(typename ComponentType::Parent_Type,!Activity_Simulator::Person_Concepts::Is_Person)))
			{	
				assert_sub_check(typename ComponentType::Parent_Type,Activity_Simulator::Person_Concepts::Is_Person,Has_Initialize_Defined, "The specified ParentType is not a valid Person type.");
				assert_sub_check(typename ComponentType::Parent_Type,Activity_Simulator::Person_Concepts::Is_Person,Has_Properties_Defined, "The specified ParentType does not have the required Properties member defined.");
				assert_sub_check(typename ComponentType::Parent_Type,Activity_Simulator::Person_Concepts::Is_Person,Has_Planner_Defined, "The specified ParentType does not have the required Planner member defined.");
			}

			template<typename TargetType> void Activity_Generation()
			{
			}
		};


		//======================================================================
		// Basic activity generation for implementing frequency-based model 
		// (i.e. no hazard-duration equations - random draws from observed distributions)
		implementation struct Simple_Activity_Generator_Implementation : public General_Activity_Generator_Implementation<MasterType,  INHERIT(Simple_Activity_Generator_Implementation)>
		{
			// IMPLEMENTATION TYPEDEFS AND INTERFACES
			typedef General_Activity_Generator_Implementation<MasterType,  INHERIT(Simple_Activity_Generator_Implementation)> base_type;
			typedef typename base_type::Component_Type ComponentType;

			typedef base_type base;
			typedef Prototypes::Activity_Generator<base_type> base_itf;

			// static discretionary activity generation model
			static float work_activity_freq[8];
			static float part_time_work_activity_freq[8];
			static float school_activity_freq[8];
			static float eat_out_activity_freq[8];
			static float errands_activity_freq[8];
			static float healthcare_activity_freq[8];
			static float leisure_activity_freq[8];
			static float major_shopping_activity_freq[8];
			static float other_activity_freq[8];
			static float other_shopping_activity_freq[8];
			static float personal_business_activity_freq[8];
			static float religious_or_civic_activity_freq[8];
			static float service_vehicle_activity_freq[8];
			static float social_activity_freq[8];

			static int Generator_Count_Array[32/*num_sim_threads()*/];
			static int Generator_Count;

			
			// Interface definitions
			typedef Prototypes::Person_Planner<typename base_type::type_of(Parent_Planner)> _planner_itf;
			//typedef  Prototypes::Destination_Chooser< typename _planner_itf::get_type_of(Destination_Chooser)> _Destination_Choice_Itf;
			typedef Prototypes::Person<typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Prototypes::Person_Properties<typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Static_Properties)> _static_properties_itf;
			typedef Prototypes::Person_Properties<typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(Properties)> _properties_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename base_type::type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;

			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef Back_Insertion_Sequence< typename _Scheduler_Interface::get_type_of(Activity_Container)> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Basic_Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename _Scheduler_Interface::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;

			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::routine_activity_plan_type> Routine_Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type> Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> At_Home_Activity_Plan;

			template<typename TargetType> void Initialize()
			{	
			}

			template<typename TargetType> void Activity_Generation()
			{
				// updates for counters
				this->Generator_Count_Array[__thread_id]++;
				if (this->Generator_Count_Array[__thread_id] % 10000 == 0)  
				{
					//LOCK(this->_update_lock);
					this->Generator_Count+=10000;
					cout << '\r' << "Activity Generation: " << this->Generator_Count << "                                 ";
					//UNLOCK(this->_update_lock);
				}
				
				person_itf* _Parent_Person = base_type::_Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				_static_properties_itf* static_properties = _Parent_Person->template Static_Properties<_static_properties_itf*>();
				_properties_itf* properties = _Parent_Person->template Properties<_properties_itf*>();
				
				// person type used in activity generation calculations
				int person_index = this->Person_Type_index<NT>();
				
				// get references to the plan containers
				Activity_Plans* activities = scheduler->template Activity_Container<Activity_Plans*>();
				Movement_Plans* movements = scheduler->template Movement_Plans_Container<Movement_Plans*>();	

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->template scenario_reference<_Scenario_Interface*>();

				int act_count = 1;

				//=========================================================================================================================
				// Get random start plan time inthe first 3 minutes for generation
				int start_plan_time = (int)(GLOBALS::Uniform_RNG.template Next_Rand<float>()*180.0f) + iteration();
				

				//=========================================================================================================================
				// Initialize person with at-home activity
				At_Home_Activity_Plan* home_act = Create_Home_Activity<NT>(act_count);
				scheduler->Update_Current_Activity(home_act);
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate work activity
				EMPLOYMENT_STATUS work_status = static_properties->template Employment_Status<EMPLOYMENT_STATUS>();
				if (work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK || work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					float num_work = work_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_work ) Create_Routine_Activity<ACTIVITY_TYPES>(PRIMARY_WORK_ACTIVITY,act_count, start_plan_time);

					float num_pwork = part_time_work_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_pwork ) Create_Routine_Activity<ACTIVITY_TYPES>(PART_TIME_WORK_ACTIVITY,act_count, start_plan_time);
				}
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate school activity
				Person_Components::Types::SCHOOL_ENROLLMENT sch_status = static_properties->template School_Enrollment<SCHOOL_ENROLLMENT>();
				if (sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					float num_school = school_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_school ) Create_Routine_Activity<ACTIVITY_TYPES>(SCHOOL_ACTIVITY,act_count, start_plan_time);
				}
				//-------------------------------------------------------------------------------------------------------------------------

				// separate routine and regular activity planning i.e. make sure all routine planning is done first
				start_plan_time += 20;

				//=========================================================================================================================
				// Get frequency of each activity type
				float num_eat_out = eat_out_activity_freq[person_index];
				float num_errand = errands_activity_freq[person_index];
				float num_healthcare = healthcare_activity_freq[person_index];
				float num_leisure = leisure_activity_freq[person_index];
				float num_maj_shop = major_shopping_activity_freq[person_index];
				float num_other = other_activity_freq[person_index];
				float num_other_shop = other_shopping_activity_freq[person_index];
				float num_pb = personal_business_activity_freq[person_index];
				float num_civic = religious_or_civic_activity_freq[person_index];
				float num_service = service_vehicle_activity_freq[person_index];
				float num_social = social_activity_freq[person_index];

				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_eat_out ) Create_Activity<ACTIVITY_TYPES>(EAT_OUT_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_errand ) Create_Activity<ACTIVITY_TYPES>(ERRANDS_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_healthcare ) Create_Activity<ACTIVITY_TYPES>(HEALTHCARE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_leisure ) Create_Activity<ACTIVITY_TYPES>(LEISURE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_maj_shop ) Create_Activity<ACTIVITY_TYPES>(MAJOR_SHOPPING_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_other ) Create_Activity<ACTIVITY_TYPES>(OTHER_ACTIVITY,act_count);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_other_shop ) Create_Activity<ACTIVITY_TYPES>(OTHER_SHOPPING_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_pb ) Create_Activity<ACTIVITY_TYPES>(PERSONAL_BUSINESS_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_civic ) Create_Activity<ACTIVITY_TYPES>(RELIGIOUS_OR_CIVIC_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_service ) Create_Activity<ACTIVITY_TYPES>(SERVICE_VEHICLE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_social ) Create_Activity<ACTIVITY_TYPES>(SOCIAL_ACTIVITY,act_count, start_plan_time);

			}

			template<typename TargetType> int Person_Type_index()
			{
				person_itf* _Parent_Person = base_type::_Parent_Planner->template Parent_Person<person_itf*>();
				_static_properties_itf* per = _Parent_Person->template Static_Properties<_static_properties_itf*>();

				int age = per->template Age<int>();
				Time_Hours hours = per->template Work_Hours<Time_Hours>();
				bool student = per->template Is_Student<bool>();
				bool employed = per->template Is_Employed<bool>();

				int index = -1;
				if (employed && hours >= 30) index = 3;		// full time
				else if (employed) index = 4;				// part time
				else if (student && age > 18) index = 7;	// adult student
				else if (age >= 65) index = 6;				// senior
				else if (age < 65 && age > 18) index = 5;	// adult non-worker
				else if (age >= 16 && age <= 18) index = 0;	// driving age school child
				else if (age < 16 && age >= 5) index = 1;	// school child
				else if (age < 5) index = 2;				// pre- school age child
				else index = 5;

				return index;
			}

		};
		//============================================
		#pragma region Static member initialization
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::work_activity_freq[]= {0,0,0,1.0,0,0,0,0};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::part_time_work_activity_freq[]= {0,0,0,0,0.75,0.15,0,0};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::school_activity_freq[]= {0.75,0.75,0,0,0,0,0,0.25};
		
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::eat_out_activity_freq[]= {0.134,0.125,0.158,0.260,0.236,0.234,0.248,0.178};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::errands_activity_freq[]= {0.033,0.052,0.093,0.108,0.156,0.198,0.181,0.070};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::healthcare_activity_freq[]= {0.030,0.034,0.053,0.051,0.070,0.110,0.142,0.046};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::leisure_activity_freq[]= {0.198,0.271,0.234,0.161,0.213,0.236,0.184,0.206};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::major_shopping_activity_freq[]= {0.007,0.014,0.022,0.028,0.039,0.041,0.041,0.015};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::other_activity_freq[]= {0.008,0.016,0.021,0.027,0.023,0.034,0.031,0.028};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::other_shopping_activity_freq[]= {0.174,0.203,0.320,0.359,0.484,0.658,0.598,0.302};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::personal_business_activity_freq[]= {0.117,0.082,0.110,0.097,0.133,0.206,0.167,0.197};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::religious_or_civic_activity_freq[]= {0.059,0.069,0.033,0.049,0.074,0.113,0.103,0.043};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::service_vehicle_activity_freq[]= {0.021,0.023,0.043,0.080,0.076,0.077,0.076,0.061};
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::social_activity_freq[]= {0.227,0.188,0.215,0.129,0.211,0.225,0.189,0.200};
		template<typename MasterType, typename InheritanceList> int Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::Generator_Count_Array[];
		template<typename MasterType, typename InheritanceList> int Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::Generator_Count;
		#pragma endregion
	}
	
}
