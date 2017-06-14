#pragma once

//#include "Scenario_Prototype.h"
#include "Person_Prototype.h"
#include "Person_Planner_Implementations.h"
#include "Movement_Plan_Prototype.h"
#include "Network_Skimming_Prototype.h"
#include "Network_Prototype.h"
#include "Activity_Prototype.h"


namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Person Agent classes
		//----------------------------------------------------------------------------------
		implementation struct Person_Implementation : public Polaris_Component<MasterType,INHERIT(Person_Implementation),Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Person_Implementation),Execution_Object>::Component_Type ComponentType;
			typedef ComponentType Component_Type;

			typedef typename MasterType::person_data_logger_type person_data_logger_type;

			// Parent agent
			m_prototype(Household_Components::Prototypes::Household, typename MasterType::household_type, Household, NONE, NONE);

			//=======================================================================================================================================================================
			// DATA MEMBERS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			m_prototype(PopSyn::Prototypes::Synthesis_Zone, typename MasterType::synthesis_zone_type, home_synthesis_zone, NONE, NONE);
			m_prototype(Vehicle_Components::Prototypes::Vehicle,typename MasterType::vehicle_type, vehicle, NONE, NONE);
			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
			
			m_prototype(Person_Components::Prototypes::Person_Mover,typename MasterType::person_mover_type, Moving_Faculty, NONE, NONE);
			m_prototype(Person_Components::Prototypes::Person_Planner, typename MasterType::person_planner_type, Planning_Faculty, NONE, NONE);
			m_prototype(Person_Components::Prototypes::Person_Scheduler,typename MasterType::person_scheduler_type, Scheduling_Faculty, NONE, NONE);
			m_prototype(Person_Components::Prototypes::Person_Perception, typename MasterType::person_perception_type, Perception_Faculty, NONE, NONE);
			m_prototype(Person_Components::Prototypes::Person_Properties, typename MasterType::person_properties_type, Properties, NONE, NONE);
			m_prototype(Person_Components::Prototypes::Person_Properties,typename MasterType::person_static_properties_type, Static_Properties, NONE, NONE);
			
			m_prototype(Activity_Location_Components::Prototypes::Activity_Location,typename MasterType::activity_location_type, current_location, NONE, NONE);


			// quick fix for moving network and scenario handling to the perception class
			typedef typename type_of(Perception_Faculty)::type_of(Network) network_reference_accessible_type;
			typedef typename type_of(Perception_Faculty)::type_of(Scenario) scenario_reference_accessible_type;

			typedef typename remove_pointer<Perception_Faculty_type>::type person_perception_type;

			template<typename TargetType> TargetType network_reference();
			template<typename TargetType> void network_reference(TargetType set_value);
			tag_getter_as_available(network_reference);		
			tag_setter_as_available(network_reference);	
			template<typename TargetType> TargetType scenario_reference();
			template<typename TargetType> void scenario_reference(TargetType set_value);
			tag_getter_as_available(scenario_reference);
			tag_setter_as_available(scenario_reference);

			typedef Network_Components::Prototypes::Network<typename type_of(Perception_Faculty)::type_of(Network)> network_reference_interface;
			typedef Scenario_Components::Prototypes::Scenario<typename type_of(Perception_Faculty)::type_of(Scenario)> scenario_reference_interface;


			// Agent ID
			m_data(long,uuid, NONE, NONE);
			m_data(long,internal_id, NONE, NONE);
			m_data(bool,has_pretrip_information,check_2(TargetType,bool,is_same), check_2(TargetType,bool,is_same));
			m_data(bool,has_done_replanning,check_2(TargetType,bool,is_same), check_2(TargetType,bool,is_same));

			m_data(shared_ptr<polaris::io::Person>, person_record, NONE, NONE);

			// First iteration  - sets the next iteration after all planning is completed
			member_component_and_feature_accessor(First_Iteration, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);

		
			// Record of completed activities (stores a simplified subset of activity data)
			m_container(std::list<Activity_Components::Prototypes::Activity_Planner<typename MasterType::activity_record_type>*>, Activity_Record_Container, NONE, NONE);

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			typedef Prototypes::Person<ComponentType> this_itf;	
			typedef Prototypes::Person_Properties<type_of(Properties)> Properties_interface;
			typedef Prototypes::Person_Properties<type_of(Static_Properties)> Static_Properties_interface;
			typedef Prototypes::Person_Mover<type_of(Moving_Faculty)> Moving_Faculty_interface;
			typedef Prototypes::Person_Planner<type_of(Planning_Faculty)> Planning_Faculty_interface;
			typedef Prototypes::Person_Scheduler<type_of(Scheduling_Faculty)> Scheduling_Faculty_interface;
			typedef Prototypes::Person_Perception<type_of(Perception_Faculty)> Perception_Faculty_interface;
			typedef Routing_Components::Prototypes::Routing<type_of(router)> router_interface;
			typedef Vehicle_Components::Prototypes::Vehicle<type_of(vehicle)> vehicle_interface;

			typedef Prototypes::Activity_Generator<typename type_of(Planning_Faculty)::type_of(Activity_Generation_Faculty)> generator_itf;
			typedef Prototypes::Destination_Chooser<typename type_of(Planning_Faculty)::type_of(Destination_Choice_Faculty)> destination_choice_itf;
			typedef Prototypes::Mode_Chooser<typename type_of(Planning_Faculty)::type_of(Mode_Choice_Faculty)> mode_choice_itf;
			typedef Prototypes::Activity_Timing_Chooser<typename type_of(Planning_Faculty)::type_of(Timing_Chooser)> timing_choice_itf;
			typedef Prototypes::Telecommute_Chooser<typename type_of(Planning_Faculty)::type_of(Telecommuting_Choice_Faculty)> telecommute_choice_itf;
			
			
			typedef Pair_Associative_Container< typename network_reference_interface::get_type_of(zones_container)> zones_container_interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_container_interface)>  zone_interface;
			
			typedef Random_Access_Sequence< typename network_reference_interface::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;
			
			typedef Back_Insertion_Sequence< type_of(Activity_Record_Container)> Activity_Records;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Records)> Activity_Record;
			
			
			//=======================================================================================================================================================================
			// FEATURES
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> void Initialize(TargetType id);
			template<typename T> void Print_Preplanned_Activities_Event();

			template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType> void Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref);

			template<typename TargetType> void Set_Locations();

			template<typename TargetType> void arrive_at_destination();

			//=======================================================================================================================================================================
			// PASS THROUGH FEATURES -> dispatched to member sub-objects
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			template<typename TargetType> TargetType current_movement_plan();

			//=======================================================================================================================================================================
			// Record sorting
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			static bool record_comparer(Activity_Record* act1, Activity_Record* act2);
			template<typename TargetType> TargetType Sort_Activity_Records();

			template<typename TimeType> TimeType Get_Estimated_Return_Home_Time();

			void Leave_Vehicle();
		};

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Person_Implementation<MasterType, InheritanceList>::network_reference()
		{
			return this->_Perception_Faculty->person_perception_type::template Network<TargetType>();
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Implementation<MasterType, InheritanceList>::network_reference(TargetType set_value)
		{
			this->_Perception_Faculty->person_perception_type::template Network<TargetType>(set_value);
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Person_Implementation<MasterType, InheritanceList>::scenario_reference()
		{
			return this->_Perception_Faculty->person_perception_type::template Scenario<TargetType>();
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Implementation<MasterType, InheritanceList>::scenario_reference(TargetType set_value)
		{
			this->_Perception_Faculty->person_perception_type::template Scenario<TargetType>(set_value);
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Implementation<MasterType, InheritanceList>::Initialize(TargetType id)
		{
			// Create and Initialize the Properties faculty
			_Properties = (Properties_interface*)Allocate<type_of(Properties)>();
			_Properties->template Initialize<void>();
			_Properties->template Parent_Person<ComponentType*>(this);

			// Create and Initialize the Planner faculty and its subcomponents
			_Planning_Faculty = (Planning_Faculty_interface*)Allocate<type_of(Planning_Faculty)>();
			_Planning_Faculty->template Parent_Person<ComponentType*>(this);
			_Planning_Faculty->template Initialize<NULLTYPE>();

			generator_itf* generator = (generator_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Activity_Generation_Faculty)>();
			generator->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
			_Planning_Faculty->template Activity_Generation_Faculty<generator_itf*>(generator);

			destination_choice_itf* destination_chooser = (destination_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Destination_Choice_Faculty)>();
			destination_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
			_Planning_Faculty->template Destination_Choice_Faculty<destination_choice_itf*>(destination_chooser);

			mode_choice_itf* mode_chooser = (mode_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Mode_Choice_Faculty)>();
			mode_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
			_Planning_Faculty->template Mode_Choice_Faculty<mode_choice_itf*>(mode_chooser);

			timing_choice_itf* timing_chooser = (timing_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Timing_Chooser)>();
			timing_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
			_Planning_Faculty->template Timing_Chooser<timing_choice_itf*>(timing_chooser);

			telecommute_choice_itf* telecommute_chooser = (telecommute_choice_itf*)Allocate<typename type_of(Planning_Faculty)::type_of(Telecommuting_Choice_Faculty)>();
			telecommute_chooser->template Parent_Planner<Planning_Faculty_interface*>(_Planning_Faculty);
			_Planning_Faculty->template Telecommuting_Choice_Faculty<telecommute_choice_itf*>(telecommute_chooser);

			// Create and Initialize the Scheduling faculty and its subcomponents
			_Scheduling_Faculty = (Scheduling_Faculty_interface*)Allocate<type_of(Scheduling_Faculty)>();
			_Scheduling_Faculty->template Parent_Person<ComponentType*>(this);
			_Scheduling_Faculty->template Initialize<void>();

			// Create and Initialize the Perception faculty and its subcomponents
			_Perception_Faculty = (Perception_Faculty_interface*)Allocate<type_of(Perception_Faculty)>();
			_Perception_Faculty->template Parent_Person<ComponentType*>(this);
			_Perception_Faculty->template Initialize<void>();

			// Create and Initialize the routing faculty
			_router = (router_interface*)Allocate<type_of(router)>();
			//_router->template traveler<ComponentType*>(this);


			// Moving faculty
			_Moving_Faculty = (Moving_Faculty_interface*)Allocate<type_of(Moving_Faculty)>();
			_Moving_Faculty->template Parent_Person<ComponentType*>(this);

			_vehicle = nullptr;

			// Create and Initialize the vehicle		
			//_vehicle = (vehicle_interface*)Allocate<type_of(vehicle)>(); 	
			//_vehicle->template uuid<int>(this->_Household->uuid<int>()*100 + id);
			//_vehicle->template internal_id<int>(id);
			//_vehicle->template traveler<ComponentType*>(this);
			//_vehicle->template router<router_interface*>(_router);
			//_vehicle->template initialize<NT>();
			//_vehicle->template is_integrated<bool>(true);


			this->_current_location = nullptr;

			// Add basic traveler properties							
			this->template uuid<int>(id);
			this->template internal_id<int>(id);
			_has_done_replanning = false;

		}

		template<typename MasterType, typename InheritanceList>
		template<typename T>
		void Person_Implementation<MasterType, InheritanceList>::Print_Preplanned_Activities_Event()
		{
			typedef Person<ComponentType> _Person_Interface;
			_Person_Interface* pthis = (_Person_Interface*)this;

			typedef Person_Scheduler<typename get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario<typename get_type_of(scenario_reference)> scenario_itf;

			typedef Back_Insertion_Sequence< typename scheduler_itf::get_type_of(Activity_Container)> Activities;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activities)>  Activity;

			typedef Back_Insertion_Sequence< typename get_type_of(Activity_Record_Container)> Activity_Records;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Records)>  Activity_Record;


			typedef  Person_Components::Prototypes::Person_Data_Logger< typename ComponentType::person_data_logger_type> _Logger_Interface;


			scheduler_itf* scheduler = pthis->template Scheduling_Faculty<scheduler_itf*>();
			Activities* activities = scheduler->template Activity_Container<Activities*>();
			Activity_Records* activity_records = pthis->template Activity_Record_Container<Activity_Records*>();

#ifdef ANTARES
			for (typename Activities::iterator itr = activities->begin(); itr != activities->end(); ++itr)
			{
				//cout << endl <<"Person ID: " << (*itr)->Parent_ID<int>() << "Activity Type: " << (*itr)->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				//((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity*>(*itr,false);

				// store activity records in the person activity record container.
				Activity_Record* new_record = (Activity_Record*)Allocate<get_component_type(Activity_Records)>();
				new_record->Initialize<Activity*>(*itr);
				activity_records->push_back(new_record);
			}

			pthis->Sort_Activity_Records<void>();
#endif


			//// exit if no activity output is specified
			//scenario_itf* scenario = (scenario_itf*)_global_scenario;
			//if (!scenario->template write_activity_output<bool>()) return;


			// push the start-of-day at home activity to the output database
			typename Activities::iterator itr = activities->begin();
			Activity* act = (Activity*)(*itr);
			if (act->template Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>() == Activity_Components::Types::AT_HOME_ACTIVITY)
			{
				((_Logger_Interface*)_global_person_logger)->template Add_Record<Activity*>(act, true);
			}
		}

		template<typename MasterType, typename InheritanceList>
		template<typename IdType, typename SynthesisZoneType, typename NetworkRefType, typename ScenarioRefType>
		void Person_Implementation<MasterType, InheritanceList>::Initialize(IdType id, SynthesisZoneType home_zone, NetworkRefType network_ref, ScenarioRefType scenario_ref)
		{
			typedef typename remove_pointer<Perception_Faculty_type>::type perception_faculty_type;

			this->Initialize<IdType>(id);
			this->home_synthesis_zone<SynthesisZoneType>(home_zone);
			this->_Perception_Faculty->perception_faculty_type::template Network<NetworkRefType>(network_ref);
			this->_Perception_Faculty->perception_faculty_type::template Scenario<ScenarioRefType>(scenario_ref);
			this->_router->template network<NetworkRefType>(network_ref);

			// Randomly determine if person uses pretrip-information sources (Radio, internet, news, etc.)
			scenario_reference_interface* scenario = this->scenario_reference<scenario_reference_interface*>();
			this->_has_pretrip_information = (GLOBALS::Uniform_RNG.template Next_Rand<float>() < scenario->template pretrip_informed_market_share<float>());
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Implementation<MasterType, InheritanceList>::Set_Locations()
		{
			// This call sets the work/school locations from the properties sub class
			_Properties->template Set_Locations<NT>();
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Implementation<MasterType, InheritanceList>::arrive_at_destination()
		{

		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Person_Implementation<MasterType, InheritanceList>::current_movement_plan()
		{
		}

		template<typename MasterType, typename InheritanceList>
		bool Person_Implementation<MasterType, InheritanceList>::record_comparer(Activity_Record* act1, Activity_Record* act2)
		{
			//typedef Activity_Components::Prototypes::Activity_Planner<typename remove_pointer<typename  type_of(Activity_Container)::value_type>::type> Activity_Plan;
			//typedef Back_Insertion_Sequence< type_of(Activity_Container),Activity_Plan*> Activity_Plans;

			//Activity_Record* act1_itf = (Activity_Record*)act1;
			//Activity_Record* act2_itf = (Activity_Record*)act2;
			return (act1->template Start_Time<Time_Minutes>() < act2->template Start_Time<Time_Minutes>());
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType Person_Implementation<MasterType, InheritanceList>::Sort_Activity_Records()
		{
			Activity_Records* records = this->Activity_Record_Container<Activity_Records*>();
			std::list<Activity_Record*>* recs = (std::list<Activity_Record*>*)records;
			recs->sort(record_comparer);
		}

		template<typename MasterType, typename InheritanceList>
		template<typename TimeType>
		TimeType Person_Implementation<MasterType, InheritanceList>::Get_Estimated_Return_Home_Time()
		{
			return this->_Scheduling_Faculty->Get_Estimated_Return_Home_Time<TimeType>();
		}

		template<typename MasterType, typename InheritanceList>
		void Person_Implementation<MasterType, InheritanceList>::Leave_Vehicle()
		{
			this->_vehicle->Unassign_From_Person();
			this->_vehicle = nullptr;
		}
	}
}
