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
			typedef typename MasterType Master_Type;

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

			//%%%RLW
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef typename _Scheduler_Interface::get_type_of(Activity_Container) _Actvity_Container;
			typedef Back_Insertion_Sequence<_Actvity_Container> Activity_Plans;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Plans)> Basic_Activity_Plan;
			
			typedef Back_Insertion_Sequence< typename _Scheduler_Interface::get_type_of(Movement_Plans_Container)> Movement_Plans;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<get_component_type(Movement_Plans)> Movement_Plan;

			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::routine_activity_plan_type> Routine_Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_plan_type> Activity_Plan;
			typedef Activity_Components::Prototypes::Activity_Planner<typename MasterType::at_home_activity_plan_type> At_Home_Activity_Plan;

			template<typename TargetType> TargetType Parent_Person()
			{
				return _Parent_Planner->template Parent_Person<TargetType>();
			}
			template<typename TargetType> TargetType Scheduling_Faculty()
			{
				_Person_Interface* person = _Parent_Planner->template Parent_Person<_Person_Interface*>();
				return person->template Scheduling_Faculty<_Scheduler_Interface*>();
			}
			int Scheduled_Activity_Count()
			{
				_Person_Interface* person = _Parent_Planner->template Parent_Person<_Person_Interface*>();
				return person->template Scheduling_Faculty<_Scheduler_Interface*>()->template Activity_Count<int>();
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

				activity->template Schedule_Activity_Events<NT>();

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

				activity->template Schedule_Activity_Events<NT>();

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
				activity->template Location<LocationType>(location);
				activity->template Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->template Schedule_Activity_Events<NT>();

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
				activity->template Location<LocationType>(location);
				activity->template Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->template Mode<ModeType>(mode);
				activity->template Mode_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Mode_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->template Schedule_Activity_Events<NT>();

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
				activity->template Location<LocationType>(location);
				activity->template Location_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Location_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->template Mode<ModeType>(mode);
				activity->template Mode_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Mode_Planning_Time<Revision&>()._sub_iteration = END+1;

				// set location and remove from planning stream
				activity->template Start_Time<TimeType>(start);
				activity->template Start_Time_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Start_Time_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->template Duration<TimeType>(duration);
				activity->template Duration_Planning_Time<Revision&>()._iteration = END+1;
				activity->template Duration_Planning_Time<Revision&>()._sub_iteration = END+1;

				activity->template Schedule_Activity_Events<NT>();
				
				this->_Parent_Planner->template Add_Activity_Plan<Activity_Plan*>(activity);	
				return activity;
			}
			template<typename TargetType> At_Home_Activity_Plan* Create_Home_Activity(int& activity_count)
			{
				At_Home_Activity_Plan* activity = (At_Home_Activity_Plan*)Allocate<typename Master_Type::at_home_activity_plan_type>();
				activity->template Parent_Planner<Parent_Planner_type>(_Parent_Planner);
				activity->template Activity_Plan_ID<int>(activity_count);

				activity->template Initialize<ACTIVITY_TYPES, Simulation_Timestep_Increment, Vehicle_Components::Types::Vehicle_Type_Keys>(ACTIVITY_TYPES::AT_HOME_ACTIVITY,0,0,END,Vehicle_Components::Types::Vehicle_Type_Keys::SOV);
				activity->template Schedule_Activity_Events<NT>();

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
				activity->template Schedule_Activity_Events<NT>();

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
			static float recreation_activity_freq[8];
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

			//%%%RLW
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;

			typedef typename base_type::_Scheduler_Interface _Scheduler_Interface;

			typedef typename _Scheduler_Interface::get_type_of(Activity_Container) _Actvity_Container;
			typedef Back_Insertion_Sequence<_Actvity_Container> Activity_Plans;
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
				At_Home_Activity_Plan* home_act = this->template Create_Home_Activity<NT>(act_count);
				//%%%RLW scheduler->template Update_Current_Activity(home_act);
				scheduler->Update_Current_Activity(home_act);
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate work activity
				EMPLOYMENT_STATUS work_status = static_properties->template Employment_Status<EMPLOYMENT_STATUS>();
				if (work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK || work_status == EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
				{
					float num_work = work_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_work ) this->template Create_Routine_Activity<ACTIVITY_TYPES>(PRIMARY_WORK_ACTIVITY,act_count, start_plan_time);

					float num_pwork = part_time_work_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_pwork ) this->template Create_Routine_Activity<ACTIVITY_TYPES>(PART_TIME_WORK_ACTIVITY,act_count, start_plan_time);
				}
				//-------------------------------------------------------------------------------------------------------------------------


				//=========================================================================================================================
				// Generate school activity
				Person_Components::Types::SCHOOL_ENROLLMENT sch_status = static_properties->template School_Enrollment<SCHOOL_ENROLLMENT>();
				if (sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || sch_status == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					float num_school = school_activity_freq[person_index];
					if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_school ) this->template Create_Routine_Activity<ACTIVITY_TYPES>(SCHOOL_ACTIVITY,act_count, start_plan_time);
				}
				//-------------------------------------------------------------------------------------------------------------------------

				// separate routine and regular activity planning i.e. make sure all routine planning is done first
				start_plan_time += 20;

				float activity_overgeneration_factor = 1.3;

				//=========================================================================================================================
				// Get frequency of each activity type
				float num_eat_out = eat_out_activity_freq[person_index] * activity_overgeneration_factor;
				float num_errand = errands_activity_freq[person_index] * activity_overgeneration_factor;
				float num_healthcare = healthcare_activity_freq[person_index] * activity_overgeneration_factor;
				float num_leisure = leisure_activity_freq[person_index] * activity_overgeneration_factor;
				float num_maj_shop = major_shopping_activity_freq[person_index] * activity_overgeneration_factor;
				float num_other = other_activity_freq[person_index] * activity_overgeneration_factor;
				float num_other_shop = other_shopping_activity_freq[person_index] * activity_overgeneration_factor;
				float num_pb = personal_business_activity_freq[person_index] * activity_overgeneration_factor;
				float num_civic = religious_or_civic_activity_freq[person_index] * activity_overgeneration_factor;
				float num_service = service_vehicle_activity_freq[person_index] * activity_overgeneration_factor;
				float num_social = social_activity_freq[person_index] * activity_overgeneration_factor;

				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_eat_out ) this->template Create_Activity<ACTIVITY_TYPES>(EAT_OUT_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_errand ) this->template Create_Activity<ACTIVITY_TYPES>(ERRANDS_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_healthcare ) this->template Create_Activity<ACTIVITY_TYPES>(HEALTHCARE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_leisure ) this->template Create_Activity<ACTIVITY_TYPES>(LEISURE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_maj_shop ) this->template Create_Activity<ACTIVITY_TYPES>(MAJOR_SHOPPING_ACTIVITY,act_count, start_plan_time);
				//if (GLOBALS::Uniform_RNG.Next_Rand<float>() < num_other ) Create_Activity<ACTIVITY_TYPES>(OTHER_ACTIVITY,act_count);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_other_shop ) this->template Create_Activity<ACTIVITY_TYPES>(OTHER_SHOPPING_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_pb ) this->template Create_Activity<ACTIVITY_TYPES>(PERSONAL_BUSINESS_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_civic ) this->template Create_Activity<ACTIVITY_TYPES>(RELIGIOUS_OR_CIVIC_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_service ) this->template Create_Activity<ACTIVITY_TYPES>(SERVICE_VEHICLE_ACTIVITY,act_count, start_plan_time);
				if (GLOBALS::Uniform_RNG.template Next_Rand<float>() < num_social ) this->template Create_Activity<ACTIVITY_TYPES>(SOCIAL_ACTIVITY,act_count, start_plan_time);

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
		//																																							student-driver, school child, pre-school, full-time, part-time, non-worker, senior, adult-student
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::work_activity_freq[] = { 0,0,0,1.17114433653373,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::part_time_work_activity_freq[] = { 0.00700125285577419,0,0,0,0.525218331986168,0.0198731108044916,0.0197172699406381,0.0429368430786302 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::school_activity_freq[] = { 0.887132434225072,0.905430052081557,0.338047211983374,0.0225387887937393,0.192641133320773,0.00583805682289839,0.00520570487129086,0.664180254600376 };

		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::eat_out_activity_freq[] = { 0.0880039059621195,0.0695757393311594,0.0986902988000941,0.219896894805232,0.215543961153012,0.186377119560776,0.208285141453395,0.187575131769565 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::errands_activity_freq[] = { 0,0,0,0,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::healthcare_activity_freq[] = { 0,0,0,0,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::leisure_activity_freq[] = { 0.0249834180853416,0.0254539938405239,0.0186902988000941,0.0322400143336681,0.046462019046693,0.0232160113075292,0.0248503188027861,0.0282341337114324 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::major_shopping_activity_freq[] = { 0.0407270248360233,0.0142321762659087,0.0149760803074269,0.0459073025404392,0.0693344596853102,0.0450410170366133,0.059941207542872,0.0221927688561477 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::other_activity_freq[] = { 0,0,0,0,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::other_shopping_activity_freq[] = { 0.180088805365171,0.0800720134243183,0.0783781664183201,0.365543014851964,0.543368602736063,0.598986553078561,0.556293679516307,0.361202724778843 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::personal_business_activity_freq[] = { 0.347630628638809,0.340365735062565,0.423279742765273,0.385353545915498,0.548239669873528,0.578866650961765,0.577980516407494,0.408177418857689 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::recreation_activity_freq[] = { 0.159315351168104,0.135248595655911,0.0381115206650459,0.077968711230282,0.110533416796825,0.0850228121709783,0.0811862173514332,0.0899731837376321 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::religious_or_civic_activity_freq[] = { 0.0428826737416169,0.0566876411840587,0.0189883146419889,0.0487016895754781,0.101836601982461,0.0686512534274345,0.064062963304788,0.0184631507567118 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::service_vehicle_activity_freq[] = { 0,0,0,0,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::social_activity_freq[] = { 0.181986513376078,0.105497785152111,0.0835824641204611,0.0811299180844432,0.154728551199383,0.155050648307957,0.120241767779613,0.153160928397497 };


	/*	template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::work_activity_freq[] = { 0,0,0,1.23595951577694,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::part_time_work_activity_freq[] = { 0.00657894736842105,0,0,0,0.73030303030303,0.0318203088441741,0.0254604550379198,0.0634920634920635 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::school_activity_freq[] = { 0.947368421052632,0.903969270166453,0.336854460093897,0.020837467751538,0.212727272727273,0.00655124005615349,0.00541711809317443,0.724867724867725 };
		
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::eat_out_activity_freq[] = { 0.0986842105263158,0.0746905676483141,0.106807511737089,0.237150228219885,0.240606060606061,0.219934487599438,0.243770314192849,0.201058201058201 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::errands_activity_freq[] = { 0.036796205630355,0.0435791051813537,0.0639700851676221,0.117884846027076,0.178840557033328,0.209938050472446,0.191249840000731,0.0978692978692979 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::healthcare_activity_freq[] = { 0.0459952570379437,0.0287266346399536,0.0379715478802463,0.0563359419714632,0.0842544202785167,0.129887322783006,0.151412736398919,0.0588874588874589 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::leisure_activity_freq[] = { 0.220394736842105,0.192915066154503,0.0845070422535211,0.115102202818019,0.165454545454545,0.14225549836219,0.145178764897075,0.121693121693122 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::major_shopping_activity_freq[] = { 0.0361842105263158,0.0145113102859582,0.0152582159624413,0.048620758086922,0.0787878787878788,0.0589611605053814,0.0698808234019502,0.0264550264550265 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::other_activity_freq[] = { 0,0,0,0,0,0,0,0 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::other_shopping_activity_freq[] = { 0.190789473684211,0.0810926163038839,0.0809859154929577,0.380432625520937,0.536969696969697,0.672437997192326,0.631094257854821,0.375661375661376 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::personal_business_activity_freq[] = { 0.128786719706242,0.0695486943914666,0.0711538915233444,0.106524183829895,0.154975747144422,0.199903836695218,0.178870182307738,0.220620620620621 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::religious_or_civic_activity_freq[] = { 0.0460526315789474,0.058898847631242,0.0211267605633803,0.0470331414963286,0.0824242424242424,0.0954609265325222,0.0937161430119177,0.0317460317460317 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::recreation_activity_freq[] = { 0.0460526315789474,0.058898847631242,0.0211267605633803,0.0470331414963286,0.0824242424242424,0.0954609265325222,0.0937161430119177,0.0317460317460317 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::service_vehicle_activity_freq[] = { 0.0318428702570379,0.0194771919075846,0.0311298275414632,0.0913526665918876,0.0892020028164607,0.0845103782570973,0.0803090614442905,0.0829400829400829 };
		template<typename MasterType, typename InheritanceList> float Simple_Activity_Generator_Implementation<MasterType, InheritanceList>::social_activity_freq[] = { 0.200657894736842,0.117370892018779,0.107981220657277,0.102996626314745,0.197575757575758,0.195601310248011,0.169014084507042,0.19047619047619 };
		*/
		template<typename MasterType, typename InheritanceList> int Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::Generator_Count_Array[];
		template<typename MasterType, typename InheritanceList> int Simple_Activity_Generator_Implementation<MasterType,  InheritanceList>::Generator_Count;

		/* Chicago model activity generation values
		{0, 0, 0, 1.0, 0, 0, 0, 0};
		{0, 0, 0, 0, 0.75, 0.15, 0, 0};
		{0.75, 0.75, 0, 0, 0, 0, 0, 0.25};

		{0.134, 0.125, 0.158, 0.260, 0.236, 0.234, 0.248, 0.178};
		{0.033, 0.052, 0.093, 0.108, 0.156, 0.198, 0.181, 0.070};
		{0.030, 0.034, 0.053, 0.051, 0.070, 0.110, 0.142, 0.046};
		{0.198, 0.271, 0.234, 0.161, 0.213, 0.236, 0.184, 0.206};
		{0.007, 0.014, 0.022, 0.028, 0.039, 0.041, 0.041, 0.015};
		{0.008, 0.016, 0.021, 0.027, 0.023, 0.034, 0.031, 0.028};
		{0.174, 0.203, 0.320, 0.359, 0.484, 0.658, 0.598, 0.302};
		{0.117, 0.082, 0.110, 0.097, 0.133, 0.206, 0.167, 0.197};
		{0.059, 0.069, 0.033, 0.049, 0.074, 0.113, 0.103, 0.043};
		{0, 0, 0, 0, 0, 0, 0, 0};
		{0.021, 0.023, 0.043, 0.080, 0.076, 0.077, 0.076, 0.061};
		{0.227, 0.188, 0.215, 0.129, 0.211, 0.225, 0.189, 0.200};*/

		#pragma endregion
	}
	
}
