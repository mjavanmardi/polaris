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
			static m_container(concat(boost::unordered::unordered_map<int, pair<float,float>>), average_activity_frequency_and_duration_container, NONE, NONE);
			static bool _is_initialized;
			static void Static_Initializer()
			{
				// make sure only called once
				if(_is_initialized) return;

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

			// local data members
			m_prototype(Prototypes::Person, typename MasterType::person_type, Parent_Person, NONE, NONE);
			//m_data(int, home_location_id, NONE, NONE);
			m_data(int, work_location_id, NONE, NONE);
			m_data(int, school_location_id, NONE, NONE);
			
			typedef Prototypes::Person<typename type_of(Parent_Person)> person_itf;
			typedef Vehicle_Components::Prototypes::Vehicle<typename person_itf::get_type_of(vehicle)> vehicle_interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;

			// Methods
			template<typename TargetType> void Initialize()
			{
				
			}
			template<typename SynthesisZoneType> void Initialize(SynthesisZoneType home_synthesis_zone/*, requires(TargetType,check(SynthesisZoneType, PopSyn::Concepts::Is_Synthesis_Zone) && check(SynthesisZoneType, is_pointer))*/)
			{	
				// updates for counters
				this->Count_Array[__thread_id]++;
				if (this->Count_Array[__thread_id] % 10000 == 0)  
				{
					this->Count+=10000;
					cout << '\r' << "Agent Home-Work-School Location Choice: " << this->Count << "                                                                           ";
				}
				

				//===============================================================================================================
				// INITIALIZE HOME / WORK / SCHOOL LOCATIONS
				//---------------------------------------------------------------------------------------------------------------
				// get an interface to the given home zone;
				typedef PopSyn::Prototypes::Synthesis_Zone<typename MasterType::synthesis_zone_type> zone_itf;
				typedef Prototypes::Person_Properties<typename type_of(Parent_Person)::get_type_of(Static_Properties)> pop_unit_itf;
				
				// useful interfaces
				typedef Random_Access_Sequence<typename zone_itf::get_type_of(Activity_Locations_Container),int> activity_location_ids_itf;
				typedef Network_Components::Prototypes::Network<typename type_of(Parent_Person)::get_type_of(network_reference)> network_itf;
				typedef Person_Planner<typename type_of(Parent_Person)::get_type_of(Planning_Faculty)> planner_itf;
				
				typedef Random_Access_Sequence<typename network_itf::get_type_of(activity_locations_container)> activity_locations_itf;
				typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(activity_locations_itf)>  activity_location_itf;
				
				typedef Pair_Associative_Container<typename network_itf::get_type_of(zones_container)> _Zone_Container_Interface;
				typedef Zone_Components::Prototypes::Zone<get_component_type(_Zone_Container_Interface)>  _Zone_Interface;
				
			
				zone_itf* zone = (zone_itf*)home_synthesis_zone;
				network_itf* network = _Parent_Person->template network_reference<network_itf*>();
				activity_locations_itf* activity_locations = network->template activity_locations_container<activity_locations_itf*>();

				// initialize location indices
				//_home_location_id = -1;
				_work_location_id = -1;
				_school_location_id = -1;
				
				// Available locations
				activity_location_ids_itf* loc_indices = zone->template Activity_Locations_Container<activity_location_ids_itf*>();

				//// assign person to a random activity location in the zone				
				//if (loc_indices->size() == 0)
				//{
				//	_home_location_id= (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * activity_locations->size());
				//}
				//else
				//{
				//	int index = -1;
				//	index = (int)((GLOBALS::Uniform_RNG.template Next_Rand<float>()*0.9999999) * loc_indices->size());
				//	_home_location_id = loc_indices->at(index);
				//}

				//// get the polaris zone of the synthesized person and increment its population counter;
				//_Zone_Interface* pzone = _Parent_Person->template Home_Location<_Zone_Interface*>();
				//pzone->template population<int&>()++;

				// Assign workers to a work location
				pop_unit_itf* properties = _Parent_Person->template Static_Properties<pop_unit_itf*>();
				if (properties->template Is_Employed<bool>()) 
				{
					_Parent_Person->template Choose_Work_Location<NT>();
				}

				if (properties->template School_Enrollment<SCHOOL_ENROLLMENT>() == SCHOOL_ENROLLMENT::ENROLLMENT_PUBLIC || properties->template School_Enrollment<SCHOOL_ENROLLMENT>() == SCHOOL_ENROLLMENT::ENROLLMENT_PRIVATE)
				{
					_Parent_Person->template Choose_School_Location<NT>();
				}

				//planner_itf* planner = this->_Parent_Person->template Planning_Faculty<planner_itf*>();
				//planner->template Write_To_Log<string>(_Parent_Person->template To_String<NT>());
				//planner->template Write_To_Log<string>("\n");
			}	
			/*template<typename TargetType> void Initialize(SynthesisZoneType home_synthesis_zone, requires(TargetType,!check(typename SynthesisZoneType, Zone_Components::Concepts::Is_Zone_Prototype) || !check(SynthesisZoneType, is_pointer)))
			{	
				assert_check(strip_modifiers(TargetType)::ParamType, Zone_Components::Concepts::Is_Zone_Prototype,"Error: must pass in home zone as a zone_prototype");
				assert_check(strip_modifiers(TargetType)::ParamType, is_pointer, "Error: must pass in home zone as a pointer.");
			}*/

			// Getter / setter for the average activity duration

			template<typename ActivitytypeType, typename ReturnTimeType> ReturnTimeType Average_Activity_Duration(ActivitytypeType act_type)
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
				
				duration = GLOBALS::Time_Converter.Convert_Value<Time_Minutes,ReturnTimeType>(value);
				return duration;
			}
			template<typename ActivitytypeType, typename TimeType> void Average_Activity_Duration(ActivitytypeType act_type, TimeType value)
			{
				// duration stored in minutes
				Time_Minutes duration = GLOBALS::Time_Converter.Convert_Value<TimeType,Time_Minutes>(value);
			
				typename average_activity_frequency_and_duration_container_type::iterator itr;
				itr = this->_average_activity_frequency_and_duration_container.find(act_type);
				if (itr != this->_average_activity_frequency_and_duration_container.end())
				{
					itr->second.second= duration;
				}
				else
				{
					this->_average_activity_frequency_and_duration_container.insert(pair<ActType, pair<TimeType,TimeType> >(act_type, pair<TimeType,TimeType>(0,duration)));
				}
			}

			// Getter / Setter for the activity frequency
			template<typename ActivitytypeType, typename ReturnType> ReturnType Average_Activity_Frequency(ActivitytypeType act_type)
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
			template<typename ActivitytypeType, typename TimeType> void Average_Activity_Frequency(ActivitytypeType act_type, TimeType value)
			{
				typename average_activity_frequency_and_duration_container_type::iterator itr;
				itr = this->_average_activity_frequency_and_duration_container.find(act_type);
				if (itr != this->_average_activity_frequency_and_duration_container.end())
				{
					itr->second.first = value;
				}
				else
				{
					this->_average_activity_frequency_and_duration_container.insert(pair<ActivitytypeType, pair<ValueType,ValueType> >(act_type, pair<ValueType,ValueType>(value,0)));
				}
			}

			// VOTT adjustment
			template<typename TargetType> TargetType Value_of_Travel_Time_Adjustment()
			{
				vehicle_interface* veh = _Parent_Person->vehicle<vehicle_interface*>();
				if (veh->is_autonomous<bool>())
				{
					TargetType adj = ((_Scenario_Interface*)_global_scenario)->cav_vott_adjustment<TargetType>();
					return adj;
				}
				else return 1.0;
			}
		};
		template<typename MasterType, typename InheritanceList> int* ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::Count_Array;
		template<typename MasterType, typename InheritanceList> int ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::Count;
		template<typename MasterType, typename InheritanceList> bool ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::_is_initialized = false;
		template<typename MasterType, typename InheritanceList> boost::unordered::unordered_map<int, pair<float,float>> ADAPTS_Person_Properties_Implementation<MasterType,  InheritanceList>::_average_activity_frequency_and_duration_container;
	}
}

