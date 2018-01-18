#pragma once

#include "Person_Properties_Prototype.h"
//#include "Person_Prototype.h"
//#include "Movement_Plan_Prototype.h"
//#include "Network_Skimming_Prototype.h"
//#include "Activity_Prototype.h"
//#include "Population_Unit_Implementations.h"
//#include "Person_Implementations.h"

namespace Person_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ADAPTS_Person_Properties_Implementation : public Polaris_Component<MasterType,INHERIT(ADAPTS_Person_Properties_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ADAPTS_Person_Properties_Implementation),Data_Object>::Component_Type ComponentType;

			// static counters
			static int* Count_Array;
			static int Count;
			m_static_container(concat(std::unordered_map<int, pair<float,float>>), average_activity_frequency_and_duration_container, NONE, NONE);
			static bool _is_initialized;
			static void Static_Initializer();

			// local data members
			m_prototype(Prototypes::Person, typename MasterType::person_type, Parent_Person, NONE, NONE);
			//m_data(int, home_location_id, NONE, NONE);
			m_data(int, work_location_id, NONE, NONE);
			m_data(int, school_location_id, NONE, NONE);
			m_data(Types::TELECOMMUTE_FREQUENCY, Telecommute_Frequency, NONE, NONE);
			
			typedef Prototypes::Person<type_of(Parent_Person)> person_itf;
			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
			typedef Random_Access_Sequence<typename household_itf::get_type_of(Vehicles_Container)> vehicles_container_itf;
			typedef Vehicle_Components::Prototypes::Vehicle<typename person_itf::get_type_of(vehicle)> vehicle_interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

			//=======================================================================================================================================================================
			// INTERFACE DEFINITIONS
			//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//typedef Prototypes::Person<ComponentType> this_itf;
			typedef Prototypes::Person_Properties<ComponentType> this_itf;
			typedef Network_Components::Prototypes::Network<typename Parent_Person_interface::get_type_of(Perception_Faculty)::type_of(Network)> network_reference_interface;
			typedef Scenario_Components::Prototypes::Scenario<typename Parent_Person_interface::get_type_of(Perception_Faculty)::type_of(Scenario)> scenario_reference_interface;
			typedef Prototypes::Person_Properties<typename Parent_Person_interface::get_type_of(Static_Properties)> Static_Properties_interface;
			typedef Prototypes::Person_Mover<typename Parent_Person_interface::get_type_of(Moving_Faculty)> Moving_Faculty_interface;
			typedef Prototypes::Person_Planner<typename Parent_Person_interface::get_type_of(Planning_Faculty)> Planning_Faculty_interface;
			typedef Prototypes::Person_Scheduler<typename Parent_Person_interface::get_type_of(Scheduling_Faculty)> Scheduling_Faculty_interface;
			typedef Prototypes::Person_Perception<typename Parent_Person_interface::get_type_of(Perception_Faculty)> Perception_Faculty_interface;
			typedef Routing_Components::Prototypes::Routing<typename Parent_Person_interface::get_type_of(router)> router_interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename Parent_Person_interface::get_type_of(vehicle)> vehicle_interface;
			typedef Prototypes::Activity_Generator<typename Planning_Faculty_interface::get_type_of(Activity_Generation_Faculty)> generator_itf;
			typedef Prototypes::Destination_Chooser<typename Planning_Faculty_interface::get_type_of(Destination_Choice_Faculty)> destination_choice_itf;
			typedef Prototypes::Mode_Chooser<typename Planning_Faculty_interface::get_type_of(Mode_Choice_Faculty)> mode_choice_itf;
			typedef Prototypes::Activity_Timing_Chooser<typename Planning_Faculty_interface::get_type_of(Timing_Chooser)> timing_choice_itf;
			typedef Prototypes::Telecommute_Chooser<typename Planning_Faculty_interface::get_type_of(Telecommuting_Choice_Faculty)> telecommute_choice_itf;
			typedef Pair_Associative_Container< typename network_reference_interface::get_type_of(zones_container)> zones_container_interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(zones_container_interface)>  zone_interface;
			typedef Random_Access_Sequence< typename network_reference_interface::get_type_of(activity_locations_container)> locations_container_interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_container_interface)>  location_interface;
			typedef Back_Insertion_Sequence< typename Parent_Person_interface::get_type_of(Activity_Record_Container)> Activity_Records;
			typedef Activity_Components::Prototypes::Activity_Planner<get_component_type(Activity_Records)> Activity_Record;
			typedef PopSyn::Prototypes::Synthesis_Zone<typename MasterType::synthesis_zone_type> synthesis_zone_interface;

			//==========================================================
			// Initializer - called from person_implementation
			template<typename TargetType> void Initialize();

			template<typename TargetType> void Set_Locations();

			//==============================================================
			// Location setters
			template<typename TargetType> void Choose_Work_Location();
			template<typename TargetType> void Choose_School_Location();


			//==============================================================
			// Getter / setter for the average activity duration
			template<typename ActivitytypeType, typename ReturnTimeType> ReturnTimeType Average_Activity_Duration(ActivitytypeType act_type);
// TODO: does not compile ActType not visible
			template<typename ActivitytypeType, typename TimeType> void Average_Activity_Duration(ActivitytypeType act_type, TimeType value);
			// Getter / Setter for the activity frequency
			template<typename ActivitytypeType, typename ReturnType> ReturnType Average_Activity_Frequency(ActivitytypeType act_type);
// TODO: does not compile, ValueType is not visible
			template<typename ActivitytypeType, typename TimeType> void Average_Activity_Frequency(ActivitytypeType act_type, TimeType value);

			//====================================================================
			// VOTT adjustment
			template<typename TargetType> TargetType Value_of_Travel_Time_Adjustment();
		};

		template<typename MasterType, typename InheritanceList>
		/*static*/ void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Static_Initializer()
		{
			// make sure only called once
			if (_is_initialized) return;

			Count_Array = new int[num_sim_threads()];

			// Initialize activity frequency and duration container
			typedef pair<int, pair<float, float>> avg_activity_record;
			avg_activity_record a;
			a.first = Activity_Components::Types::AT_HOME_ACTIVITY;					a.second.first = 2.237f; a.second.second = 442.f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::SCHOOL_ACTIVITY;					a.second.first = 0.122f; a.second.second = 387.3f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::CHANGE_TRANSPORTATION_ACTIVITY;	a.second.first = 0.051f; a.second.second = 18.f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY;		a.second.first = 0.065f; a.second.second = 109.f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY;		a.second.first = 0.263f; a.second.second = 13.4f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::EAT_OUT_ACTIVITY;					a.second.first = 0.229f; a.second.second = 53.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::HEALTHCARE_ACTIVITY;				a.second.first = 0.075f; a.second.second = 97.6f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::ERRANDS_ACTIVITY;					a.second.first = 0.124f; a.second.second = 16.6f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::OTHER_ACTIVITY;					a.second.first = 0.002f; a.second.second = 14.f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY;		a.second.first = 0.129f; a.second.second = 91.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::RECREATION_ACTIVITY;				a.second.first = 0.206f; a.second.second = 140.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::OTHER_SHOPPING_ACTIVITY;			a.second.first = 0.423f; a.second.second = 35.4f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY;			a.second.first = 0.066f; a.second.second = 13.7f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY;			a.second.first = 0.031f; a.second.second = 45.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::SOCIAL_ACTIVITY;					a.second.first = 0.168f; a.second.second = 198.1f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::OTHER_WORK_ACTIVITY;				a.second.first = 0.129f; a.second.second = 170.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::PRIMARY_WORK_ACTIVITY;			a.second.first = 0.381f; a.second.second = 390.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::PART_TIME_WORK_ACTIVITY;			a.second.first = 0.381f; a.second.second = 390.2f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::WORK_AT_HOME_ACTIVITY;			a.second.first = 0.026f; a.second.second = 560.7f;	_average_activity_frequency_and_duration_container.insert(a);
			a.first = Activity_Components::Types::LEISURE_ACTIVITY;					a.second.first = 0.213f; a.second.second = 142.0f;	_average_activity_frequency_and_duration_container.insert(a);
		}
		//==========================================================
		// Initializer - called from person_implementation
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Initialize()
		{}

		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Set_Locations()
		{
			Static_Properties_interface* properties = _Parent_Person->template Static_Properties<Static_Properties_interface*>();

			// updates for counters
			this->Count_Array[__thread_id]++;
			if (this->Count_Array[__thread_id] % 10000 == 0)
			{
				this->Count += 10000;
				cout << '\r' << "Agent Home-Work-School Location Choice: " << this->Count << "                                                                           ";
			}

			// initialize location indices
			_work_location_id = -1;
			_school_location_id = -1;

			// Assign workers to a work location
			if (properties->template Is_Employed<bool>()) Choose_Work_Location<NT>();

			// Assign school location
			if (properties->template School_Enrollment<SCHOOL_ENROLLMENT>() == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || properties->template School_Enrollment<SCHOOL_ENROLLMENT>() == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE) Choose_School_Location<NT>();

			// Determine the telecommute behavior - if person always telecommutes, reset work location to home
			this->_Telecommute_Frequency = Types::TC_NEVER;
			if (properties->template Is_Employed<bool>()) this->_Telecommute_Frequency = _Parent_Person->Planning_Faculty<Planning_Faculty_interface*>()->Telecommuting_Choice_Faculty<telecommute_choice_itf*>()->Telecommute_Choice<Types::TELECOMMUTE_FREQUENCY>();

			// update work location if always telecommuting
			if (this->_Telecommute_Frequency == Types::TC_DAILY) this->work_location_id(_Parent_Person->Home_Location<int>());
		}

		//==============================================================
		// Location setters
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Choose_Work_Location()
		{
			// interface to this
			this_itf* pthis = (this_itf*)this;
			destination_choice_itf* dest_chooser = _Parent_Person->Planning_Faculty<Planning_Faculty_interface*>()->template Destination_Choice_Faculty<destination_choice_itf*>();


			// first, make sure person is worker, if not exit
			EMPLOYMENT_STATUS status = _Parent_Person->Static_Properties<Static_Properties_interface*>()->Employment_Status<EMPLOYMENT_STATUS>();
			if (status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK && status != EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_ARMED_FORCES_AT_WORK)
			{
				this->work_location_id(-1);
				return;
			}

			location_interface* dest = nullptr;

			// do workplace destination choice		
			dest = dest_chooser->template Choose_Routine_Destination<location_interface*>(Activity_Components::Types::PRIMARY_WORK_ACTIVITY);

			// work location choice failed, make this person a telecommuter
			if (dest == nullptr)
			{
				this->work_location_id<int>(_Parent_Person->Home_Location<int>());
				//pthis->template Work_Location<location_interface*>(pthis->template Home_Location<location_interface*>());
			}

			else this->work_location_id<int>(dest->internal_id<int>());
			//else pthis->template Work_Location<location_interface*>(dest);

			scenario_reference_interface* scenario = _Parent_Person->template scenario_reference<scenario_reference_interface*>();
			zone_interface* zone = _Parent_Person->template Work_Location<zone_interface*>();

			// update the simulated employment in the work zone
			zone->template employment_simulated<int&>() += 1.0f / scenario->template percent_to_synthesize<float>();
		}
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Choose_School_Location()
		{
			// interface to this
			this_itf* pthis = (this_itf*)this;
			zone_interface* selected_zone = nullptr;
			Static_Properties_interface* properties = _Parent_Person->Static_Properties<Static_Properties_interface*>();


			//=========================================================
			// first, make sure person is student, if not exit
			//---------------------------------------------------------
			SCHOOL_ENROLLMENT status = properties->template School_Enrollment<SCHOOL_ENROLLMENT>();
			if (status != SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC && status != SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
			{
				this->school_location_id<int>(-1);
				//pthis->template School_Location<int>(-1);
				return;
			}

			//=========================================================
			// Find available zones within the specified target range of the given work travel time
			//---------------------------------------------------------
			zones_container_interface* zones = _Parent_Person->network_reference<network_reference_interface*>()->template zones_container<zones_container_interface*>();
			//typename zones_container_interface::iterator z_itr;
			std::vector<zone_interface*> temp_zones;
			std::vector<float> temp_zone_probabilities;
			zone_interface* orig = _Parent_Person->template Home_Location<zone_interface*>();

			//=========================================================
			// if origin zone has school locations, select, else search
			//---------------------------------------------------------
			//if (orig->template school_locations<locations_container_interface*>()->size() > 0)
			//{
			//	selected_zone = orig;
			//}
			//else
			//{
			// loop through all zones, store those within +- 2 min of estimated work travel time that have available work locations
			float time_range_to_search = 15.0;
			if (properties->Age<int>() > 10) time_range_to_search += 10;
			if (properties->Age<int>() > 18) time_range_to_search += 15;

			std::vector<zone_interface*> zones_near;
			_Parent_Person->network_reference<network_reference_interface*>()->template Get_Locations_Within_Range<zone_interface*, Time_Minutes, Vehicle_Components::Types::Vehicle_Type_Keys, zone_interface*>(zones_near, orig, 480.0, 0, time_range_to_search, SOV, true);

			float school_locations = 0;
			//while (temp_zones.size() == 0)
			//{
			//for (z_itr = zones->begin(); z_itr != zones->end(); ++z_itr)
			for (std::vector<zone_interface*>::iterator z_itr = zones_near.begin(); z_itr != zones_near.end(); ++z_itr)
			{
				//zone_interface* zone = (zone_interface*)(z_itr->second);
				zone_interface* zone = *z_itr;
				//Time_Minutes t = _Parent_Person->network_reference<network_reference_interface*>()->template Get_TTime<zone_interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(orig, zone, Vehicle_Components::Types::Vehicle_Type_Keys::SOV, 9);
				if (/*t < time_range_to_search &&*/ zone->template school_locations<locations_container_interface*>()->size() > 0)
				{
					school_locations += (int)zone->template school_locations<locations_container_interface*>()->size();
					temp_zones.push_back(zone);
				}
			}
			// expand the search time radius if no available zones found
			//if (time_range_to_search >= 60) break;
			//time_range_to_search += time_range_to_search;

			//}

			if (temp_zones.size() == 0)
			{
				this->school_location_id<int>(_Parent_Person->Home_Location<int>());
				//pthis->template School_Location<int>(pthis->template Home_Location<int>());
				return;
			}

			// calculate probabilities
			float cum_prob = 0;
			for (typename std::vector<zone_interface*>::iterator t_itr = temp_zones.begin(); t_itr != temp_zones.end(); ++t_itr)
			{
				cum_prob += (*t_itr)->template school_locations<locations_container_interface*>()->size() / school_locations;
				temp_zone_probabilities.push_back(cum_prob);
			}

			//=========================================================
			// select zone
			//---------------------------------------------------------
			float r = Uniform_RNG.template Next_Rand<float>();
			typename std::vector<zone_interface*>::iterator t_itr;
			std::vector<float>::iterator p_itr;
			for (t_itr = temp_zones.begin(), p_itr = temp_zone_probabilities.begin(); t_itr != temp_zones.end(); ++t_itr, ++p_itr)
			{
				if (r<*p_itr)
				{
					selected_zone = *t_itr;
					break;
				}
			}

			//}

			//=========================================================
			// Select location from within the zone
			//---------------------------------------------------------
			// set school location to home if no valid locations found
			if (selected_zone == nullptr)
			{
				this->school_location_id<int>(_Parent_Person->Home_Location<int>());
				//pthis->template School_Location<int>(pthis->template Home_Location<int>());
			}

			// select school location from within available school locations in the zone
			else
			{
				locations_container_interface* school_locations = selected_zone->template school_locations<locations_container_interface*>();
				float size = school_locations->size();
				int index = (int)(Uniform_RNG.template Next_Rand<float>()*size);
				location_interface* loc = (location_interface*)((*school_locations)[index]);

				if (loc != nullptr) this->school_location_id<int>(loc->template internal_id<int>()); //pthis->template School_Location<int>(loc->template internal_id<int>());
				else this->school_location_id<int>(_Parent_Person->Home_Location<int>());
			}
		}


		//==============================================================
		// Getter / setter for the average activity duration
		template<typename MasterType, typename InheritanceList>
		template<typename ActivitytypeType, typename ReturnTimeType>
		ReturnTimeType ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Average_Activity_Duration(ActivitytypeType act_type)
		{
			Time_Minutes value;

			typename average_activity_frequency_and_duration_container_type::iterator itr;

			itr = this->_average_activity_frequency_and_duration_container.find(act_type);
			if (itr != this->_average_activity_frequency_and_duration_container.end())
			{
				value = itr->second.second;
			}
			else
			{
				//THROW_WARNING("WARNING: Activity type '" << act_type << "' was not found in the Person_Properties average_activity_duration container. Activity duration assumed to be 0.");
				value = 0.0;
			}
			// duration stored in minutes
			ReturnTimeType duration;

			duration = GLOBALS::Time_Converter.Convert_Value<Time_Minutes, ReturnTimeType>(value);
			return duration;
		}
		
		// TODO: does not compile ActType not visible
		template<typename MasterType, typename InheritanceList>
		template<typename ActivitytypeType, typename TimeType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Average_Activity_Duration(ActivitytypeType act_type, TimeType value)
		{

			// duration stored in minutes
			Time_Minutes duration = GLOBALS::Time_Converter.Convert_Value<TimeType, Time_Minutes>(value);

			typename average_activity_frequency_and_duration_container_type::iterator itr;
			itr = this->_average_activity_frequency_and_duration_container.find(act_type);
			if (itr != this->_average_activity_frequency_and_duration_container.end())
			{
				itr->second.second = duration;
			}
			else
			{
				this->template _average_activity_frequency_and_duration_container.insert(pair<ActivitytypeType, pair<TimeType, TimeType> >(act_type, pair<TimeType, TimeType>(0, duration)));
			}
		}
		
		// Getter / Setter for the activity frequency
		template<typename MasterType, typename InheritanceList>
		template<typename ActivitytypeType, typename ReturnType>
		ReturnType ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Average_Activity_Frequency(ActivitytypeType act_type)
		{
			typename average_activity_frequency_and_duration_container_type::iterator itr;
			itr = this->_average_activity_frequency_and_duration_container.find(act_type);
			if (itr != this->_average_activity_frequency_and_duration_container.end())
			{
				return (ReturnType)itr->second.first;
			}
			else
			{
				//THROW_WARNING("WARNING: Activity type '" << act_type << "' was not found in the Person_Properties average_activity_duration container. Activity duration assumed to be 0.");
				return (ReturnType)0.0;
			}
		}
		
		// TODO: does not compile, ValueType is not visible
		template<typename MasterType, typename InheritanceList>
		template<typename ActivitytypeType, typename TimeType>
		void ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Average_Activity_Frequency(ActivitytypeType act_type, TimeType value)
		{
			typename average_activity_frequency_and_duration_container_type::iterator itr;
			itr = this->_average_activity_frequency_and_duration_container.find(act_type);
			if (itr != this->_average_activity_frequency_and_duration_container.end())
			{
				itr->second.first = value;
			}
			else
			{
				this->template _average_activity_frequency_and_duration_container.insert(pair<ActivitytypeType, pair<TimeType, TimeType> >(act_type, pair<TimeType, TimeType>(value, 0)));
			}
		}

		//====================================================================
		// VOTT adjustment
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		TargetType ADAPTS_Person_Properties_Implementation<MasterType, InheritanceList>::Value_of_Travel_Time_Adjustment()
		{
			vehicles_container_itf* vehicles = _Parent_Person->Household<household_itf*>()->Vehicles_Container<vehicles_container_itf*>();

			for (vehicles_container_itf::iterator v_itr = vehicles->begin(); v_itr != vehicles->end(); ++v_itr)
			{
				if ((*v_itr)->is_autonomous<bool>()) return ((_Scenario_Interface*)_global_scenario)->template cav_vott_adjustment<TargetType>();
			}
			return 1.0;

			//vehicle_interface* veh = _Parent_Person->template vehicle<vehicle_interface*>();
			//if (veh->template is_autonomous<bool>())
			//{
			//	TargetType adj = ((_Scenario_Interface*)_global_scenario)->template cav_vott_adjustment<TargetType>();
			//	return adj;
			//}
			//else return 1.0;
		}

		template<typename MasterType, typename InheritanceList> int* ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::Count_Array;
		template<typename MasterType, typename InheritanceList> int ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::Count;
		template<typename MasterType, typename InheritanceList> bool ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::_is_initialized = false;
		template<typename MasterType, typename InheritanceList> std::unordered_map<int, pair<float,float>> ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::_average_activity_frequency_and_duration_container;
	}
}

