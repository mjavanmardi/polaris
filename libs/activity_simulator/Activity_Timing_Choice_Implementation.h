#pragma once

#include "Activity_Timing_Choice_Prototype.h"



namespace Person_Components
{
	namespace Implementations
	{
		implementation struct Activity_Timing_Chooser_Implementation : public Polaris_Component<MasterType,INHERIT(Activity_Timing_Chooser_Implementation),Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(Activity_Timing_Chooser_Implementation),Data_Object>::Component_Type ComponentType;
			
			// local types
			typedef pair<Time_Minutes,Time_Minutes> data_type;

			typedef pair<float,data_type> record_type;
			typedef std::map<float,data_type> map_type;
			typedef pair<int, float> start_record_type;
			typedef std::map<int, float> start_map_type;


			// pointer back to parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);

			//===========================================================================================================================================
			// Interface definitions	
			typedef Prototypes::Person<typename type_of(Parent_Planner)::type_of(Parent_Person)> person_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Properties)> person_properties_itf;
			typedef Prototypes::Person_Properties<typename person_itf::get_type_of(Static_Properties)> person_static_properties_itf;
			typedef Household_Components::Prototypes::Household<typename person_itf::get_type_of(Household)> household_itf;
			typedef Household_Components::Prototypes::Household_Properties<typename household_itf::get_type_of(Static_Properties)> household_static_properties_itf;
			typedef Prototypes::Person_Scheduler<typename person_itf::get_type_of(Scheduling_Faculty)> scheduler_itf;
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;
			typedef Network_Components::Prototypes::Network< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(network_reference)> _Network_Interface;
			typedef Network_Skimming_Components::Prototypes::Network_Skimming< typename _Network_Interface::get_type_of(skimming_faculty)> _Skim_Interface;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_value_type> los_itf;
			typedef Network_Skimming_Components::Prototypes::LOS<typename MasterType::los_invariant_value_type> los_invariant_itf;
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
			typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)>  _Activity_Location_Interface;
			typedef Random_Access_Sequence< typename _Activity_Location_Interface::get_type_of(origin_links)> _Links_Container_Interface;
			typedef Link_Components::Prototypes::Link<get_component_type(_Links_Container_Interface)>  _Link_Interface;
			typedef Pair_Associative_Container< typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
			typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)>  _Zone_Interface;
			typedef Random_Access_Sequence< typename _Network_Interface::get_type_of(zone_ids_container), int> _Zone_Ids_Interface;


			// static start time and duration lookup container for each activity type
			static m_container(concat(std::unordered_map<int, map_type >), start_time_duration_container, NONE, NONE);
			static m_container(concat(std::unordered_map<int, start_map_type>), start_time_container, NONE, NONE); // maps to the probabilities of start times alone, by activity type integer
			

			//=======================================================================================================
			// Time of day and duration choice model
			//-------------------------------------------------------------------------------------------------------
			template<typename ActivityRefType, typename ReturnType> pair<ReturnType,ReturnType> Choose_Time_Of_Day_Period(ActivityRefType activity_ref)
			{
				// returns a pair for start and end times of the selected period
			}


			//=======================================================================================================
			// accessors for drawing random start time and duration for given activity record from static container
			//-------------------------------------------------------------------------------------------------------
			template<typename ActivityRefType, typename ReturnType> ReturnType Get_Start_Time(ActivityRefType activity_ref)
			{
				pair<ReturnType, ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref);
				return return_val.first;
			}
			template<typename ActivityRefType, typename TimeType, typename ReturnType> ReturnType Get_Start_Time(ActivityRefType activity_ref, TimeType range_start, TimeType range_end)
			{
				pair<ReturnType, ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref,range_start,range_end);
				return return_val.first;
			}
			template<typename ActivityRefType, typename ReturnType> ReturnType Get_Duration(ActivityRefType activity_ref)
			{
				pair<ReturnType, ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref);
				return return_val.second;
			}
			template<typename ActivityRefType, typename ReturnType> pair<ReturnType, ReturnType> Get_Start_Time_and_Duration(ActivityRefType activity_ref)
			{
				// get interface to activity reference
				Activity_Components::Prototypes::Activity_Planner<strip_modifiers(ActivityRefType)::Component_Type>* act;
				act = (Activity_Components::Prototypes::Activity_Planner<strip_modifiers(ActivityRefType)::Component_Type>*) activity_ref;
				
				// draw random from uniform distribution
				float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();

				// use upper bound to draw the start time / duration pair with first cumulative probability greater than rand
				map_type::iterator itr = _start_time_duration_container[(int)(act->template Activity_Type<ACTIVITY_TYPES>())].upper_bound(rand);

				// make sure valid entry is found
				if (itr == _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].end()) 
				{
					THROW_WARNING("WARNING: no valid start-time / duration pair found for activity type '" << act->template Activity_Type<ACTIVITY_TYPES>() <<"' and random value = " << rand <<", using last pair in list.");
					itr = _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].end();
					itr--;
				}
				pair<ReturnType,ReturnType> return_val;

				// add random draw from between 0-5 minutes as this is the aggregation level of the start_time data
				return_val.first = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(itr->second.first - GLOBALS::Uniform_RNG.template Next_Rand<float>()*10.0f);
				
				// add random draw from between 0-5 minutes as this is the aggregation level of the duration data
				return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(itr->second.second + (0.5f - GLOBALS::Uniform_RNG.template Next_Rand<float>())*5.0f);
				
				// make sure duration is greater than 5 minutes
				if (return_val.second < GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(5.0f)) return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes, ReturnType>(5.0f);

				// make sure start + duration is less than END
				if (return_val.first >= GLOBALS::Time_Converter.template Convert_Value<Simulation_Timestep_Increment,ReturnType>(END))
				{
					return_val.first = return_val.first - GLOBALS::Time_Converter.template Convert_Value<Simulation_Timestep_Increment,ReturnType>(END);
				}
				if (return_val.second >= GLOBALS::Time_Converter.template Convert_Value<Simulation_Timestep_Increment,ReturnType>(END))
				{
					return_val.second = GLOBALS::Time_Converter.template Convert_Value<Simulation_Timestep_Increment,ReturnType>(END);
				}
				
				//cout << "START TIME:" << return_val.first << ",DUR:" << return_val.second << endl;
				return return_val;
			}
			template<typename ReturnTimeType, typename ActivityType, typename InputTimeType> pair<ReturnTimeType,ReturnTimeType> Get_Start_Time_and_Duration(ActivityType activity_ref, InputTimeType range_start, InputTimeType range_end)
			{
				assert(range_end > range_start);

				// get interface to activity reference
				Activity_Components::Prototypes::Activity_Planner<strip_modifiers(ActivityType)::Component_Type>* act;
				act = (Activity_Components::Prototypes::Activity_Planner<strip_modifiers(ActivityType)::Component_Type>*) activity_ref;

				// convert given range bounds to minutes
				Time_Minutes start = GLOBALS::Time_Converter.template Convert_Value<InputTimeType,Time_Minutes>(range_start);
				Time_Minutes end = GLOBALS::Time_Converter.template Convert_Value<InputTimeType,Time_Minutes>(range_end);
				
				// draw random from uniform distribution and get corresponding data item - loop while draw is outside of range
				float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();

				start_map_type::iterator range_start_itr = _start_time_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].lower_bound(start);
				start_map_type::iterator range_end_itr = _start_time_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].lower_bound(end);

				float p_range_start = range_start_itr->second;
				float p_range_end = 1.0;
				if (range_end_itr != _start_time_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].end()) p_range_end = range_end_itr->second;

				float rand2 = rand * (p_range_end - p_range_start) + p_range_start;
				map_type::iterator itr = _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].lower_bound(rand2);

				/*int iter = 0;
				float rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();
				map_type::iterator itr = _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].upper_bound(rand);
				while (itr->second.first < start || itr->second.first >= end)
				{
					rand = GLOBALS::Uniform_RNG.template Next_Rand<float>();
					itr = _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].upper_bound(rand);
					++iter;
					if (iter >= 100)
					{
						THROW_WARNING("Warning, could not find a start time value within given range: random start time has been selected.");
						break;
					}
				}*/

				// make sure valid entry is found
				if (itr == _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].end())
				{
					THROW_EXCEPTION("ERROR: no valid start-time / duration pair found for activity type '" << act->template Activity_Type<ACTIVITY_TYPES>() << "' and random value = " << rand);
				}
			
				pair<ReturnTimeType,ReturnTimeType> return_val;
				return_val.first = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnTimeType>(itr->second.first);
				return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnTimeType>(itr->second.second);
				return return_val;
			}

			template<typename ActivityRefType, typename ReturnType> pair<ReturnType, ReturnType> Timing_Choice(ActivityRefType activity_ref)
			{
				typedef Activity_Components::Prototypes::Activity_Planner<strip_modifiers(ActivityRefType)::Component_Type> Activity_interface;
				typedef typename Activity_interface::Involved_Persons_Container_type Involved_Persons_interface;
								
				person_itf* _Parent_Person = _Parent_Planner->template Parent_Person<person_itf*>();
				scheduler_itf* scheduler = _Parent_Person->template Scheduling_Faculty<scheduler_itf*>();
				household_itf* _Parent_Household = _Parent_Person->person_itf::template Household<household_itf*>();
				household_static_properties_itf* household_properties = _Parent_Household->template Static_Properties<household_static_properties_itf*>();
				person_static_properties_itf* person_properties = _Parent_Person->Static_Properties<person_static_properties_itf*>();

				// external knowledge references
				_Network_Interface* network = _Parent_Person->template network_reference<_Network_Interface*>();
				_Scenario_Interface* scenario = _Parent_Person->scenario_reference<_Scenario_Interface*>();
				_Zones_Container_Interface* zones = network->template zones_container<_Zones_Container_Interface*>();
				_Skim_Interface* skimmer = network->skimming_faculty<_Skim_Interface*>();

				Activity_interface* act = (Activity_interface*)activity_ref;
				_Activity_Location_Interface* loc = act->Location<_Activity_Location_Interface*>();
				_Zone_Interface* dest_zone = nullptr;
				if (act->Location_Is_Planned() && loc != nullptr) dest_zone = loc->zone<_Zone_Interface*>();
				_Zone_Interface* home_taz = _Parent_Household->Home_Location<_Zone_Interface*>();

				//==================================
				// create model variables
				//----------------------------------
				float Mode_auto = (float)act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::SOV;
				float Mode_transit = (float)act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::BUS;
				float Mode_passenger = (float)act->Mode<Vehicle_Components::Types::Vehicle_Type_Keys>() == Vehicle_Components::Types::Vehicle_Type_Keys::HOV;
				float Location_CBD = 0.0;
				float Location_city = 0.0;
				float Location_suburbs = 0.0;
				if (dest_zone != nullptr)
				{
					Location_CBD = dest_zone->areatype<int>() < 3 ? 1.0 : 0.0;
					Location_city = dest_zone->areatype<int>() == 3 ? 1.0 : 0.0;;
					Location_suburbs = dest_zone->areatype<int>() > 3 && dest_zone->areatype<int>() < 7 ? 1.0 : 0.0;
				}
				else
				{
					Location_CBD = home_taz->areatype<int>() < 3 ? 1.0 : 0.0;
					Location_city = home_taz->areatype<int>() == 3 ? 1.0 : 0.0;;
					Location_suburbs = home_taz->areatype<int>() > 3 && home_taz->areatype<int>() < 7 ? 1.0 : 0.0;
				}
				float Party_alone = act->Involved_Persons_Container<Involved_Persons_interface&>().size() == 0 ? 1.0 : 0.0;
				float Party_joint = !Party_alone;
				float Flex_start = act->Start_Time_Flexibility<Activity_Components::Types::FLEXIBILITY_VALUES>() == Activity_Components::Types::LOW_FLEXIBILITY;
				float Flex_duration = act->Duration_Flexibility<Activity_Components::Types::FLEXIBILITY_VALUES>() == Activity_Components::Types::LOW_FLEXIBILITY;
				
				float AM_peak_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(6.0, 9.0);
				float AM_Offpeak_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(9.0, 12.0);
				float PM_Offpeak_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(12.0, 16.0);
				float PM_peak_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(16.0,19.0);
				float Evening_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(19.0, 24.0);
				float Night_Occupancy = scheduler->Percent_Free_Time_In_Schedule<Time_Hours>(0.0, 6.0);

				float Age_18_24 = person_properties->Age<int>() >= 18 && person_properties->Age<int>() < 25 ? 1.0 : 0.0;
				float Age_25_34 = person_properties->Age<int>() >= 25 && person_properties->Age<int>() < 35 ? 1.0 : 0.0;
				float Age_60 = person_properties->Age<int>() >= 60 ? 1.0 : 0.0;
				float Income_low = (household_properties->Income<Dollars>() < 50000.0) ? 1.0 : 0.0;
				float Income_high = (household_properties->Income<Dollars>() >= 100000.0) ? 1.0 : 0.0;
				float Income_med = 1.0 - Income_low - Income_high;
				
				float Gender = person_properties->Gender<Person_Components::Types::GENDER>() == Person_Components::Types::MALE ? 1.0 : 0.0;
				float HH_size = household_properties->Household_size<float>();
				float Student_fulltime = person_properties->Is_Student<bool>();
				float Work_fulltime = person_properties->Is_Fulltime_Worker();
				float Work_parttime = person_properties->Is_Parttime_Worker();
				float Workers = household_properties->Number_of_workers<int>() >= 2 ? 1.0 : 0.0;
				float degree = person_properties->Educational_Attainment<Types::EDUCATION_LEVEL>() == Types::EDUC_UNDERGRADUATE || person_properties->Educational_Attainment<Types::EDUCATION_LEVEL>() == Types::EDUC_GRADUATE ? 1.0 : 0.0;

				// use actual person properties instead of static person properties here
				float Telework = _Parent_Person->Properties<person_properties_itf*>()->Telecommute_Frequency<Types::TELECOMMUTE_FREQUENCY>() == Types::TC_DAILY || _Parent_Person->Properties<person_properties_itf*>()->Telecommute_Frequency<Types::TELECOMMUTE_FREQUENCY>() == Types::TC_WEEKLY ? 1.0 : 0.0 ;

				// Travel time and variability estimation
				float AM_Peak_TTV = 0;
				float AM_Offpeak_TTV = 0;
				float PM_Offpeak_TTV = 0;
				float PM_Peak_TTV = 0;
				float Evening_TTV = 0;
				float Night_TTV = 0;
				float AM_Peak_TT = 0;
				float AM_Offpeak_TT = 0;
				float PM_Offpeak_TT = 0;
				float PM_Peak_TT = 0;
				float Evening_TT = 0;
				float Night_TT = 0;

				// Known destination, use exact travel time characteristics
				if (dest_zone != nullptr)
				{
					pair<Time_Minutes, float> tts_ampeak = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 6.0, 9.0);
					AM_Peak_TT = tts_ampeak.first;
					AM_Peak_TTV = tts_ampeak.second;
					pair<Time_Minutes, float> tts_amoff = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 9.0, 12.0);
					AM_Offpeak_TT = tts_amoff.first;
					AM_Offpeak_TTV = tts_amoff.second;
					pair<Time_Minutes, float> tts_pmoff = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 12.0, 16.0);
					PM_Offpeak_TT = tts_pmoff.first;
					PM_Offpeak_TTV = tts_pmoff.second;
					pair<Time_Minutes, float> tts_pmpeak = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 16.0, 19.0);
					PM_Peak_TT = tts_pmpeak.first;
					PM_Peak_TTV = tts_pmpeak.second;
					pair<Time_Minutes, float> tts_evening = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 19.0, 24.0);
					Evening_TT = tts_evening.first;
					Evening_TTV = tts_evening.second;
					pair<Time_Minutes, float> tts_night = network->template Get_TTime_Statistics<_Zone_Interface*, Vehicle_Components::Types::Vehicle_Type_Keys, Time_Hours, Time_Minutes>(home_taz, dest_zone, Vehicle_Components::Types::SOV, 0.0, 6.0);
					Night_TT = tts_evening.first;
					Night_TTV = tts_evening.second;
				}
				// Unknown destination, use TT characteristics from the accessibilty calculations
				{
					AM_Peak_TT =	home_taz->template avg_ttime_auto_ampeak<Time_Minutes>();
					PM_Peak_TT =	home_taz->template avg_ttime_auto_pmpeak<Time_Minutes>();
					AM_Offpeak_TT = home_taz->template avg_ttime_auto_offpeak<Time_Minutes>();
					PM_Offpeak_TT = home_taz->template avg_ttime_auto_offpeak<Time_Minutes>();
					Evening_TT =	home_taz->template avg_ttime_auto_night<Time_Minutes>();
					Night_TT =		home_taz->template avg_ttime_auto_night<Time_Minutes>();
					AM_Peak_TTV =	home_taz->template avg_ttime_var_auto_ampeak<float>();
					PM_Peak_TTV =	home_taz->template avg_ttime_var_auto_pmpeak<float>();
					AM_Offpeak_TTV= home_taz->template avg_ttime_var_auto_offpeak<float>();
					PM_Offpeak_TTV= home_taz->template avg_ttime_var_auto_offpeak<float>();
					Evening_TTV =	home_taz->template avg_ttime_var_auto_night<float>();
					Night_TTV =		home_taz->template avg_ttime_var_auto_night<float>();
				}

				// RUM portion of utility
				Activity_Components::Types::ACTIVITY_TYPES act_type = act->Activity_Type<Activity_Components::Types::ACTIVITY_TYPES>();
				float D_EO = act_type == Activity_Components::Types::EAT_OUT_ACTIVITY? 1.0 : 0.0;
				float D_S = act_type == Activity_Components::Types::SOCIAL_ACTIVITY ? 1.0 : 0.0;
				float D_MS = act_type == Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY || act_type == Activity_Components::Types::OTHER_SHOPPING_ACTIVITY ? 1.0 : 0.0;
				float D_OS = act_type == Activity_Components::Types::OTHER_SHOPPING_ACTIVITY ? 1.0 : 0.0;
				float D_PB = act_type == Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY || act_type == Activity_Components::Types::HEALTHCARE_ACTIVITY || act_type == Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY || act_type == Activity_Components::Types::ERRANDS_ACTIVITY ? 1.0 : 0.0;
				float D_L = act_type == Activity_Components::Types::LEISURE_ACTIVITY || act_type == Activity_Components::Types::RECREATION_ACTIVITY ? 1.0 : 0.0;
				float D_RC = act_type == Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY ? 1.0 : 0.0;;

				float U_am_peak =	 _C_AMPEAK_EAT_OUT_ACTIVITY * D_EO	  + _C_AMPEAK_SOCIAL_ACTIVITY * D_S		+ _C_AMPEAK_MAJOR_SHOPPING_ACTIVITY * D_MS	  + _C_AMPEAK_OTHER_SHOPPING_ACTIVITY * D_OS		+ _C_AMPEAK_PERSONAL_BUSINESS_ACTIVITY *	D_PB + _C_AMPEAK_LEISURE_ACTIVITY * D_L		+ _C_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
				float U_am_offpeak = _C_AMOFFPEAK_EAT_OUT_ACTIVITY * D_EO + _C_AMOFFPEAK_SOCIAL_ACTIVITY * D_S	+ _C_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY * D_MS + _C_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY * D_OS		+ _C_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY * D_PB + _C_AMOFFPEAK_LEISURE_ACTIVITY * D_L	+ _C_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
				float U_pm_offpeak = _C_PMOFFPEAK_EAT_OUT_ACTIVITY * D_EO + _C_PMOFFPEAK_SOCIAL_ACTIVITY * D_S	+ _C_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY * D_MS + _C_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY * D_OS		+ _C_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY * D_PB + _C_PMOFFPEAK_LEISURE_ACTIVITY * D_L	+ _C_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
				float U_pm_peak =	 _C_PMPEAK_EAT_OUT_ACTIVITY * D_EO	  + _C_PMPEAK_SOCIAL_ACTIVITY * D_S		+ _C_PMPEAK_MAJOR_SHOPPING_ACTIVITY * D_MS	  + _C_PMPEAK_OTHER_SHOPPING_ACTIVITY * D_OS		+ _C_PMPEAK_PERSONAL_BUSINESS_ACTIVITY *	D_PB + _C_PMPEAK_LEISURE_ACTIVITY * D_L		+ _C_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
				float U_evening =	 _C_EVENING_EAT_OUT_ACTIVITY * D_EO	  + _C_EVENING_SOCIAL_ACTIVITY  * D_S	+ _C_EVENING_MAJOR_SHOPPING_ACTIVITY * D_MS	  + _C_EVENING_OTHER_SHOPPING_ACTIVITY * D_OS		+ _C_EVENING_PERSONAL_BUSINESS_ACTIVITY *D_PB	 + _C_EVENING_LEISURE_ACTIVITY * D_L	+ _C_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
				float U_night =		 _C_NIGHT_EAT_OUT_ACTIVITY * D_EO	  +	_C_NIGHT_SOCIAL_ACTIVITY  * D_S		+ _C_NIGHT_MAJOR_SHOPPING_ACTIVITY * D_MS	  + _C_NIGHT_OTHER_SHOPPING_ACTIVITY * D_OS			+ _C_NIGHT_PERSONAL_BUSINESS_ACTIVITY *D_PB		 + _C_NIGHT_LEISURE_ACTIVITY * D_L		+ _C_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY * D_RC;
	
				U_am_peak += _S_AMPEAK_AGE_60*Age_60 + _S_AMPEAK_HH_WORKERS*Workers + _S_AMPEAK_FLEX_START*Flex_start + _S_AMPEAK_FLEX_DURATION*Flex_duration + _S_AMPEAK_PARTY_JOINT*Party_joint;
				U_am_offpeak += _S_AMOFFPEAK_CONSTANT + _S_AMOFFPEAK_WORK_PARTTIME*Work_parttime + _S_AMOFFPEAK_STUDENT_FULLTIME*Student_fulltime + _S_AMOFFPEAK_MODE_PASSENGER*Mode_passenger;
				U_pm_offpeak += _S_PMOFFPEAK_CONSTANT + _S_PMOFFPEAK_INCOME_LOW*Income_low + _S_PMOFFPEAK_MODE_TRANSIT*Mode_transit;
				U_pm_peak += _S_PMPEAK_CONSTANT + _S_PMPEAK_WORK_FULLTIME*Work_fulltime + _S_PMPEAK_INCOME_HIGH*Income_high + _S_PMPEAK_HH_WORKERS*Workers + _S_PMPEAK_FLEX_START*Flex_start;
				U_evening += _S_EVENING_CONSTANT + _S_EVENING_TELEWORK*Telework + _S_EVENING_INCOME_LOW*Income_low + _S_EVENING_DEGREE_COLLEGE*degree;

				// RRM portion of utility
				float R_am_peak = FLT_MAX;
				float R_am_offpeak = FLT_MAX;
				float R_pm_offpeak = FLT_MAX;
				float R_pm_peak = FLT_MAX;
				float R_evening = FLT_MAX;

				R_am_peak =		- 1.0*log(1.0 + exp(_S_AMPEAK_TT*(AM_Offpeak_TT - AM_Peak_TT))) - log(1.0 + exp(_S_AMPEAK_TT*(PM_Peak_TT - AM_Peak_TT))) - log(1.0 + exp(_S_AMPEAK_TT*(PM_Offpeak_TT - AM_Peak_TT))) - log(1.0 + exp(_S_AMPEAK_TT*(Evening_TT - AM_Peak_TT))) - log(1.0 + exp(_S_AMPEAK_TT*(Night_TT - AM_Peak_TT)));
				R_am_offpeak =  - 1.0*log(1.0 + exp(_S_AMOFFPEAK_TT*(AM_Peak_TT - AM_Offpeak_TT))) - log(1.0 + exp(_S_AMOFFPEAK_TT*(PM_Peak_TT - AM_Offpeak_TT))) - log(1.0 + exp(_S_AMOFFPEAK_TT*(PM_Offpeak_TT - AM_Offpeak_TT))) - log(1.0 + exp(_S_AMOFFPEAK_TT*(Evening_TT - AM_Offpeak_TT))) - log(1.0 + exp(_S_AMOFFPEAK_TT*(Night_TT - AM_Offpeak_TT)))
								- 1.0*log(1.0 + exp(_S_AMOFFPEAK_TTV*(AM_Peak_TTV - AM_Offpeak_TTV))) - log(1.0 + exp(_S_AMOFFPEAK_TTV*(PM_Peak_TTV - AM_Offpeak_TTV))) - log(1.0 + exp(_S_AMOFFPEAK_TTV*(PM_Offpeak_TTV - AM_Offpeak_TTV))) - log(1.0 + exp(_S_AMOFFPEAK_TTV*(Evening_TTV - AM_Offpeak_TTV))) - log(1.0 + exp(_S_AMOFFPEAK_TTV*(Night_TTV - AM_Offpeak_TTV)))
								- 1.0*log(1.0 + exp(_S_AMOFFPEAK_OCCUPANCY*(AM_peak_Occupancy - AM_Offpeak_Occupancy))) - log(1.0 + exp(_S_AMOFFPEAK_OCCUPANCY*(PM_peak_Occupancy - AM_Offpeak_Occupancy))) - log(1.0 + exp(_S_AMOFFPEAK_OCCUPANCY*(PM_Offpeak_Occupancy - AM_Offpeak_Occupancy))) - log(1.0 + exp(_S_AMOFFPEAK_OCCUPANCY*(Evening_Occupancy - AM_Offpeak_Occupancy))) - log(1.0 + exp(_S_AMOFFPEAK_OCCUPANCY*(Night_Occupancy - AM_Offpeak_Occupancy)));
				R_pm_offpeak =  - 1.0*log(1.0 + exp(_S_PMOFFPEAK_TT*(AM_Peak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMOFFPEAK_TT*(PM_Peak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMOFFPEAK_TT*(AM_Offpeak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMOFFPEAK_TT*(Evening_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMOFFPEAK_TT*(Night_TT - PM_Offpeak_TT)))
								- 1.0*log(1.0 + exp(_S_PMOFFPEAK_OCCUPANCY*(AM_peak_Occupancy - PM_Offpeak_Occupancy))) - log(1.0 + exp(_S_PMOFFPEAK_OCCUPANCY*(PM_peak_Occupancy - PM_Offpeak_Occupancy))) - log(1.0 + exp(_S_PMOFFPEAK_OCCUPANCY*(AM_Offpeak_Occupancy - PM_Offpeak_Occupancy))) - log(1.0 + exp(_S_PMOFFPEAK_OCCUPANCY*(Evening_Occupancy - PM_Offpeak_Occupancy))) - log(1.0 + exp(_S_PMOFFPEAK_OCCUPANCY*(Night_Occupancy - PM_Offpeak_Occupancy)));
				R_pm_peak =		- 1.0*log(1.0 + exp(_S_PMPEAK_TT*(AM_Peak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMPEAK_TT*(PM_Peak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMPEAK_TT*(AM_Offpeak_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMPEAK_TT*(Evening_TT - PM_Offpeak_TT))) - log(1.0 + exp(_S_PMPEAK_TT*(Night_TT - PM_Offpeak_TT)))
								- 1.0*log(1.0 + exp(_S_PMPEAK_OCCUPANCY*(AM_peak_Occupancy - PM_peak_Occupancy))) - log(1.0 + exp(_S_PMPEAK_OCCUPANCY*(PM_Offpeak_Occupancy - PM_peak_Occupancy))) - log(1.0 + exp(_S_PMPEAK_OCCUPANCY*(AM_Offpeak_Occupancy - PM_peak_Occupancy))) - log(1.0 + exp(_S_PMPEAK_OCCUPANCY*(Evening_Occupancy - PM_peak_Occupancy))) - log(1.0 + exp(_S_PMPEAK_OCCUPANCY*(Night_Occupancy - PM_peak_Occupancy)));
				R_evening =		- 1.0*log(1.0 + exp(_S_EVENING_TT*(AM_Offpeak_TT - Evening_TT))) - log(1.0 + exp(_S_EVENING_TT*(PM_Peak_TT - Evening_TT))) - log(1.0 + exp(_S_EVENING_TT*(PM_Offpeak_TT - Evening_TT))) - log(1.0 + exp(_S_EVENING_TT*(AM_Peak_TT - Evening_TT))) - log(1.0 + exp(_S_EVENING_TT*(Night_TT - Evening_TT)));

				U_am_peak += R_am_peak;
				U_am_offpeak += R_am_offpeak;
				U_pm_offpeak += R_pm_offpeak;
				U_pm_peak += R_pm_peak;
				U_evening += R_evening;
				float U_sum = exp(U_am_peak) + exp(U_am_offpeak) + exp(U_pm_offpeak) + exp(U_pm_peak) + exp(U_evening) + exp(U_night);

				float P_am_peak = exp(U_am_peak) / U_sum;
				float P_am_offpeak = exp(U_am_offpeak) / U_sum + P_am_peak;
				float P_pm_offpeak = exp(U_pm_offpeak) / U_sum + P_am_offpeak;
				float P_pm_peak = exp(U_pm_peak) / U_sum + P_pm_offpeak;
				float P_evening = exp(U_evening) / U_sum + P_pm_peak;
				float P_night = exp(U_night) / U_sum + P_evening;

				float r = GLOBALS::Uniform_RNG.Next_Rand<float>();

				if		(r < P_am_peak)		return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 6.0, 9.0);
				else if (r < P_am_offpeak)	return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 9.0, 12.0);
				else if (r < P_pm_offpeak)	return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 12.0, 16.0);
				else if (r < P_pm_peak)		return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 16.0, 19.0);
				else if (r < P_evening)		return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 19.0, 24.0);
				else						return Get_Start_Time_and_Duration<ReturnType, ActivityRefType, Time_Hours>(activity_ref, 0.0, 6.0);
			}


			// Static initialization
			static m_data(bool, is_initialized, NONE, NONE);
			static void read_distribution()
			{
				// make sure this is only run once
				if (_is_initialized) return;
				_is_initialized = true;


				// Initialize hashmap for start time
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::PRIMARY_WORK_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::SCHOOL_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::EAT_OUT_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::ERRANDS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::HEALTHCARE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::LEISURE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::OTHER_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::SOCIAL_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::PART_TIME_WORK_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(Activity_Components::Types::WORK_AT_HOME_ACTIVITY, map_type()));

				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::PRIMARY_WORK_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::SCHOOL_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::EAT_OUT_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::ERRANDS_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::HEALTHCARE_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::LEISURE_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::MAJOR_SHOPPING_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::OTHER_SHOPPING_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::PERSONAL_BUSINESS_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::PICK_UP_OR_DROP_OFF_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::RELIGIOUS_OR_CIVIC_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::SERVICE_VEHICLE_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::SOCIAL_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::PART_TIME_WORK_ACTIVITY, start_map_type()));
				_start_time_container.insert(pair<int, start_map_type>(Activity_Components::Types::WORK_AT_HOME_ACTIVITY, start_map_type()));

				// add items
				File_IO::File_Reader data_file;
				if (!data_file.Open(_START_DURATION_DISTRIBUTION_FILE.c_str())) cout << endl << "Could not open 'start_time_duration_data.txt' in the working directory.  Check to make sure the start-time duration file exists.";

				int act_type;
				float start, dur, cum_prob;

				while(data_file.Read())
				{
					act_type = data_file.Get_Int(0);
					start = data_file.Get_Data<float>(1);
					dur = data_file.Get_Data<float>(2);
					cum_prob = data_file.Get_Data<float>(3);

					if (cum_prob < 0.0 || cum_prob > 1.0) THROW_EXCEPTION("ERROR: cumulative probability value out of bounds (0 <= x <= 1) for activity type id '" << act_type << "' in file '" << _START_DURATION_DISTRIBUTION_FILE.c_str() << "'.");
					
					_start_time_duration_container[act_type].insert(record_type(cum_prob,data_type(start,dur)));

					// add the start time only probabilities for range draws
					if (_start_time_container[act_type].find((int)start) == _start_time_container[act_type].end()) _start_time_container[act_type].insert(start_record_type((int)start, cum_prob));
				}
				
				//copy primary work into work_at_home
				for (map_type::iterator itr = _start_time_duration_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].begin(); itr != _start_time_duration_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].end(); ++itr)
				{
					_start_time_duration_container[Activity_Components::Types::WORK_AT_HOME_ACTIVITY].insert(*itr);
				}
				for (start_map_type::iterator itr = _start_time_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].begin(); itr != _start_time_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].end(); ++itr)
				{
					_start_time_container[Activity_Components::Types::WORK_AT_HOME_ACTIVITY].insert(*itr);
				}

				//evaluate
			}

			static bool static_initialize(const string& option_file)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				rapidjson::Document document;
				//std::string option_file = reinterpret_cast<_Scenario_Interface*>(_global_scenario)->template telecommute_choice_implementation_param_file<string>();
				if (option_file.length() < 1)
				{
					cout << "Warning: option file for Activity_Timing_Choice_Model was not specified" << endl;
					read_distribution();
					return true;
				}

				_Scenario_Interface* scenario = static_cast<_Scenario_Interface*>(_global_scenario);
				if (!scenario->parse_option_file(document, option_file))
					return false;

				// check that model is defined if it is requested through scenario
				if (!document.HasMember("Activity_Timing_Choice_Model")) THROW_EXCEPTION("ERROR: Activity_Timing_Choice_Model parameter not found in '" << option_file << "', but specified in scenarion.json.");


				string section = "Activity_Timing_Choice_Model";

				// get the parameters...
				#pragma region
				scenario->set_parameter(document, section, "START_DURATION_DISTRIBUTION_FILE", _START_DURATION_DISTRIBUTION_FILE);
				scenario->set_parameter(document, section, "S_AMPEAK_AGE_60", _S_AMPEAK_AGE_60);
				scenario->set_parameter(document, section, "S_AMPEAK_HH_WORKERS", _S_AMPEAK_HH_WORKERS);
				scenario->set_parameter(document, section, "S_AMPEAK_FLEX_START", _S_AMPEAK_FLEX_START);
				scenario->set_parameter(document, section, "S_AMPEAK_FLEX_DURATION", _S_AMPEAK_FLEX_DURATION);
				scenario->set_parameter(document, section, "S_AMPEAK_PARTY_JOINT", _S_AMPEAK_PARTY_JOINT);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_CONSTANT", _S_AMOFFPEAK_CONSTANT);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_WORK_PARTTIME", _S_AMOFFPEAK_WORK_PARTTIME);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_STUDENT_FULLTIME", _S_AMOFFPEAK_STUDENT_FULLTIME);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_MODE_PASSENGER", _S_AMOFFPEAK_MODE_PASSENGER);
				scenario->set_parameter(document, section, "S_PMOFFPEAK_CONSTANT", _S_PMOFFPEAK_CONSTANT);
				scenario->set_parameter(document, section, "S_PMOFFPEAK_INCOME_LOW", _S_PMOFFPEAK_INCOME_LOW);
				scenario->set_parameter(document, section, "S_PMOFFPEAK_MODE_TRANSIT", _S_PMOFFPEAK_MODE_TRANSIT);
				scenario->set_parameter(document, section, "S_PMPEAK_CONSTANT", _S_PMPEAK_CONSTANT);
				scenario->set_parameter(document, section, "S_PMPEAK_WORK_FULLTIME", _S_PMPEAK_WORK_FULLTIME);
				scenario->set_parameter(document, section, "S_PMPEAK_INCOME_HIGH", _S_PMPEAK_INCOME_HIGH);
				scenario->set_parameter(document, section, "S_PMPEAK_HH_WORKERS", _S_PMPEAK_HH_WORKERS);
				scenario->set_parameter(document, section, "S_PMPEAK_FLEX_START", _S_PMPEAK_FLEX_START);
				scenario->set_parameter(document, section, "S_EVENING_CONSTANT", _S_EVENING_CONSTANT);
				scenario->set_parameter(document, section, "S_EVENING_TELEWORK", _S_EVENING_TELEWORK);
				scenario->set_parameter(document, section, "S_EVENING_INCOME_LOW", _S_EVENING_INCOME_LOW);
				scenario->set_parameter(document, section, "S_EVENING_DEGREE_COLLEGE", _S_EVENING_DEGREE_COLLEGE);
				scenario->set_parameter(document, section, "S_AMPEAK_TT", _S_AMPEAK_TT);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_OCCUPANCY", _S_AMOFFPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_TTV", _S_AMOFFPEAK_TTV);
				scenario->set_parameter(document, section, "S_AMOFFPEAK_TT", _S_AMOFFPEAK_TT);
				scenario->set_parameter(document, section, "S_PMOFFPEAK_OCCUPANCY", _S_PMOFFPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "S_PMOFFPEAK_TT", _S_PMOFFPEAK_TT);
				scenario->set_parameter(document, section, "S_PMPEAK_OCCUPANCY", _S_PMPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "S_PMPEAK_TT", _S_PMPEAK_TT);
				scenario->set_parameter(document, section, "S_EVENING_TT", _S_EVENING_TT);

				scenario->set_parameter(document, section, "D_AMPEAK_CONSTANT", _D_AMPEAK_CONSTANT);
				scenario->set_parameter(document, section, "D_AMPEAK_FLEX_DURATION", _D_AMPEAK_FLEX_DURATION);
				scenario->set_parameter(document, section, "D_AMPEAK_TELEWORK", _D_AMPEAK_TELEWORK);
				scenario->set_parameter(document, section, "D_AMPEAK_WORK_FULLTIME", _D_AMPEAK_WORK_FULLTIME);
				scenario->set_parameter(document, section, "D_AMPEAK_OCCUPANCY", _D_AMPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_CONSTANT", _D_AMOFFPEAK_CONSTANT);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_TELEWORK", _D_AMOFFPEAK_TELEWORK);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_INCOME_HIGH", _D_AMOFFPEAK_INCOME_HIGH);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_MODE_AUTO", _D_AMOFFPEAK_MODE_AUTO);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_WORK_FULLTIME", _D_AMOFFPEAK_WORK_FULLTIME);
				scenario->set_parameter(document, section, "D_AMOFFPEAK_OCCUPANCY", _D_AMOFFPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "D_PMOFFPEAK_CONSTANT", _D_PMOFFPEAK_CONSTANT);
				scenario->set_parameter(document, section, "D_PMOFFPEAK_HH_SIZE", _D_PMOFFPEAK_HH_SIZE);
				scenario->set_parameter(document, section, "D_PMOFFPEAK_MODE_PASSENGER", _D_PMOFFPEAK_MODE_PASSENGER);
				scenario->set_parameter(document, section, "D_PMOFFPEAK_AGE_18_24", _D_PMOFFPEAK_AGE_18_24);
				scenario->set_parameter(document, section, "D_PMOFFPEAK_OCCUPANCY", _D_PMOFFPEAK_OCCUPANCY);
				scenario->set_parameter(document, section, "D_PMPEAK_CONSTANT", _D_PMPEAK_CONSTANT);
				scenario->set_parameter(document, section, "D_PMPEAK_PARTY_ALONE", _D_PMPEAK_PARTY_ALONE);
				scenario->set_parameter(document, section, "D_PMPEAK_AGE_60", _D_PMPEAK_AGE_60);
				scenario->set_parameter(document, section, "D_EVENING_CONSTANT", _D_EVENING_CONSTANT);
				scenario->set_parameter(document, section, "D_EVENING_WORK_FULLTIME", _D_EVENING_WORK_FULLTIME);
				scenario->set_parameter(document, section, "D_EVENING_MODE_TRANSIT", _D_EVENING_MODE_TRANSIT);
				scenario->set_parameter(document, section, "D_EVENING_AGE_60", _D_EVENING_AGE_60);
				scenario->set_parameter(document, section, "D_NIGHT_INCOME_HIGH", _D_NIGHT_INCOME_HIGH);
				scenario->set_parameter(document, section, "D_NIGHT_LOCATION_SUBURBS", _D_NIGHT_LOCATION_SUBURBS);

				scenario->set_parameter(document, section, "THETA_AMPEAK", _THETA_AMPEAK);
				scenario->set_parameter(document, section, "THETA_AMOFFPEAK", _THETA_AMOFFPEAK);
				scenario->set_parameter(document, section, "THETA_PMOFFPEAK", _THETA_PMOFFPEAK);
				scenario->set_parameter(document, section, "THETA_PMPEAK", _THETA_PMPEAK);
				scenario->set_parameter(document, section, "THETA_EVENING", _THETA_EVENING);
				scenario->set_parameter(document, section, "THETA_NIGHT", _THETA_NIGHT);

				scenario->set_parameter(document, section, "SIGMA_AMPEAK", _SIGMA_AMPEAK);
				scenario->set_parameter(document, section, "SIGMA_AMOFFPEAK", _SIGMA_AMOFFPEAK);
				scenario->set_parameter(document, section, "SIGMA_PMOFFPEAK", _SIGMA_PMOFFPEAK);
				scenario->set_parameter(document, section, "SIGMA_PMPEAK", _SIGMA_PMPEAK);
				scenario->set_parameter(document, section, "SIGMA_EVENING", _SIGMA_EVENING);
				scenario->set_parameter(document, section, "SIGMA_NIGHT", _SIGMA_NIGHT);
				#pragma endregion

				// fill in the default start-duration distribution
				read_distribution();
				return true;
			}

			static void print_parameters()
			{
				cout << "Activity_Timing_Choice_Implementation parameters" << endl;
				cout << "\tSTART_DURATION_DISTRIBUTION_FILE = "START_DURATION_DISTRIBUTION_FILE<string>() << endl;
				cout << "Start time - Hybrid RUM-RRM model parameters" << endl;
				cout << "\tS_AMPEAK_AGE_60 = " << S_AMPEAK_AGE_60<float>() << endl;
				cout << "\tS_AMPEAK_HH_WORKERS = " << S_AMPEAK_HH_WORKERS<float>() << endl;
				cout << "\tS_AMPEAK_FLEX_START = " << S_AMPEAK_FLEX_START<float>() << endl;
				cout << "\tS_AMPEAK_FLEX_DURATION = " << S_AMPEAK_FLEX_DURATION<float>() << endl;
				cout << "\tS_AMPEAK_PARTY_JOINT = " << S_AMPEAK_PARTY_JOINT<float>() << endl;
				cout << "\tS_AMOFFPEAK_CONSTANT = " << S_AMOFFPEAK_CONSTANT<float>() << endl;
				cout << "\tS_AMOFFPEAK_WORK_PARTTIME = " << S_AMOFFPEAK_WORK_PARTTIME<float>() << endl;
				cout << "\tS_AMOFFPEAK_STUDENT_FULLTIME = " << S_AMOFFPEAK_STUDENT_FULLTIME<float>() << endl;
				cout << "\tS_AMOFFPEAK_MODE_PASSENGER = " << S_AMOFFPEAK_MODE_PASSENGER<float>() << endl;
				cout << "\tS_PMOFFPEAK_CONSTANT = " << S_PMOFFPEAK_CONSTANT<float>() << endl;
				cout << "\tS_PMOFFPEAK_INCOME_LOW = " << S_PMOFFPEAK_INCOME_LOW<float>() << endl;
				cout << "\tS_PMOFFPEAK_MODE_TRANSIT = " << S_PMOFFPEAK_MODE_TRANSIT<float>() << endl;
				cout << "\tS_PMPEAK_CONSTANT = " << S_PMPEAK_CONSTANT<float>() << endl;
				cout << "\tS_PMPEAK_WORK_FULLTIME = " << S_PMPEAK_WORK_FULLTIME<float>() << endl;
				cout << "\tS_PMPEAK_INCOME_HIGH = " << S_PMPEAK_INCOME_HIGH<float>() << endl;
				cout << "\tS_PMPEAK_HH_WORKERS = " << S_PMPEAK_HH_WORKERS<float>() << endl;
				cout << "\tS_PMPEAK_FLEX_START = " << S_PMPEAK_FLEX_START<float>() << endl;
				cout << "\tS_EVENING_CONSTANT = " << S_EVENING_CONSTANT<float>() << endl;
				cout << "\tS_EVENING_TELEWORK = " << S_EVENING_TELEWORK<float>() << endl;
				cout << "\tS_EVENING_INCOME_LOW = " << S_EVENING_INCOME_LOW<float>() << endl;
				cout << "\tS_EVENING_DEGREE_COLLEGE = " << S_EVENING_DEGREE_COLLEGE<float>() << endl;
				cout << "\tS_AMPEAK_TT = " << S_AMPEAK_TT<float>() << endl;
				cout << "\tS_AMOFFPEAK_OCCUPANCY = " << S_AMOFFPEAK_OCCUPANCY<float>() << endl;
				cout << "\tS_AMOFFPEAK_TTV = " << S_AMOFFPEAK_TTV<float>() << endl;
				cout << "\tS_AMOFFPEAK_TT = " << S_AMOFFPEAK_TT<float>() << endl;
				cout << "\tS_PMOFFPEAK_OCCUPANCY = " << S_PMOFFPEAK_OCCUPANCY<float>() << endl;
				cout << "\tS_PMOFFPEAK_TT = " << S_PMOFFPEAK_TT<float>() << endl;
				cout << "\tS_PMPEAK_OCCUPANCY = " << S_PMPEAK_OCCUPANCY<float>() << endl;
				cout << "\tS_PMPEAK_TT = " << S_PMPEAK_TT<float>() << endl;
				cout << "\tS_EVENING_TT = " << S_EVENING_TT<float>() << endl;
				cout << endl<<"Duration AFT Hazard-Model Parameters" << endl;
				cout << "\tD_AMPEAK_CONSTANT = " << D_AMPEAK_CONSTANT<float>() << endl;
				cout << "\tD_AMPEAK_FLEX_DURATION = " << D_AMPEAK_FLEX_DURATION<float>() << endl;
				cout << "\tD_AMPEAK_TELEWORK = " << D_AMPEAK_TELEWORK<float>() << endl;
				cout << "\tD_AMPEAK_WORK_FULLTIME = " << D_AMPEAK_WORK_FULLTIME<float>() << endl;
				cout << "\tD_AMPEAK_OCCUPANCY = " << D_AMPEAK_OCCUPANCY<float>() << endl;
				cout << "\tD_AMOFFPEAK_CONSTANT = " << D_AMOFFPEAK_CONSTANT<float>() << endl;
				cout << "\tD_AMOFFPEAK_TELEWORK = " << D_AMOFFPEAK_TELEWORK<float>() << endl;
				cout << "\tD_AMOFFPEAK_INCOME_HIGH = " << D_AMOFFPEAK_INCOME_HIGH<float>() << endl;
				cout << "\tD_AMOFFPEAK_MODE_AUTO = " << D_AMOFFPEAK_MODE_AUTO<float>() << endl;
				cout << "\tD_AMOFFPEAK_WORK_FULLTIME = " << D_AMOFFPEAK_WORK_FULLTIME<float>() << endl;
				cout << "\tD_AMOFFPEAK_OCCUPANCY = " << D_AMOFFPEAK_OCCUPANCY<float>() << endl;
				cout << "\tD_PMOFFPEAK_CONSTANT = " << D_PMOFFPEAK_CONSTANT<float>() << endl;
				cout << "\tD_PMOFFPEAK_HH_SIZE = " << D_PMOFFPEAK_HH_SIZE<float>() << endl;
				cout << "\tD_PMOFFPEAK_MODE_PASSENGER = " << D_PMOFFPEAK_MODE_PASSENGER<float>() << endl;
				cout << "\tD_PMOFFPEAK_AGE_18_24 = " << D_PMOFFPEAK_AGE_18_24<float>() << endl;
				cout << "\tD_PMOFFPEAK_OCCUPANCY = " << D_PMOFFPEAK_OCCUPANCY<float>() << endl;
				cout << "\tD_PMPEAK_CONSTANT = " << D_PMPEAK_CONSTANT<float>() << endl;
				cout << "\tD_PMPEAK_PARTY_ALONE = " << D_PMPEAK_PARTY_ALONE<float>() << endl;
				cout << "\tD_PMPEAK_AGE_60 = " << D_PMPEAK_AGE_60<float>() << endl;
				cout << "\tD_EVENING_CONSTANT = " << D_EVENING_CONSTANT<float>() << endl;
				cout << "\tD_EVENING_WORK_FULLTIME = " << D_EVENING_WORK_FULLTIME<float>() << endl;
				cout << "\tD_EVENING_MODE_TRANSIT = " << D_EVENING_MODE_TRANSIT<float>() << endl;
				cout << "\tD_EVENING_AGE_60 = " << D_EVENING_AGE_60<float>() << endl;
				cout << "\tD_NIGHT_INCOME_HIGH = " << D_NIGHT_INCOME_HIGH<float>() << endl;
				cout << "\tD_NIGHT_LOCATION_SUBURBS = " << D_NIGHT_LOCATION_SUBURBS<float>() << endl;
				cout << endl << "Copula Parameters" << endl;
				cout << "\tTHETA_AMPEAK = " << THETA_AMPEAK<float>() << endl;
				cout << "\tTHETA_AMOFFPEAK = " << THETA_AMOFFPEAK<float>() << endl;
				cout << "\tTHETA_PMOFFPEAK = " << THETA_PMOFFPEAK<float>() << endl;
				cout << "\tTHETA_PMPEAK = " << THETA_PMPEAK<float>() << endl;
				cout << "\tTHETA_EVENING = " << THETA_EVENING<float>() << endl;
				cout << "\tTHETA_NIGHT = " << THETA_NIGHT<float>() << endl;
				cout << endl <<"Scale Parameters" << endl;
				cout << "\tSIGMA_AMPEAK = " << SIGMA_AMPEAK<float>() << endl;
				cout << "\tSIGMA_AMOFFPEAK = " << SIGMA_AMOFFPEAK<float>() << endl;
				cout << "\tSIGMA_PMOFFPEAK = " << SIGMA_PMOFFPEAK<float>() << endl;
				cout << "\tSIGMA_PMPEAK = " << SIGMA_PMPEAK<float>() << endl;
				cout << "\tSIGMA_EVENING = " << SIGMA_EVENING<float>() << endl;
				cout << "\tSIGMA_NIGHT = " << SIGMA_NIGHT<float>() << endl<<endl;

				// CALIBRATION PARAMS
				cout << "\tC_NIGHT_EAT_OUT_ACTIVITY = " << C_NIGHT_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_SOCIAL_ACTIVITY = " << C_NIGHT_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_MAJOR_SHOPPING_ACTIVITY = " << C_NIGHT_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_OTHER_SHOPPING_ACTIVITY = " << C_NIGHT_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_PERSONAL_BUSINESS_ACTIVITY = " << C_NIGHT_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_LEISURE_ACTIVITY = " << C_NIGHT_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;

				cout << "\tC_AMPEAK_EAT_OUT_ACTIVITY = " << C_AMPEAK_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_SOCIAL_ACTIVITY = " << C_AMPEAK_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_MAJOR_SHOPPING_ACTIVITY = " << C_AMPEAK_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_OTHER_SHOPPING_ACTIVITY = " << C_AMPEAK_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_PERSONAL_BUSINESS_ACTIVITY = " << C_AMPEAK_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_LEISURE_ACTIVITY = " << C_AMPEAK_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;

				cout << "\tC_AMOFFPEAK_EAT_OUT_ACTIVITY = " << C_AMOFFPEAK_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_SOCIAL_ACTIVITY = " << C_AMOFFPEAK_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY = " << C_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY = " << C_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY = " << C_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_LEISURE_ACTIVITY = " << C_AMOFFPEAK_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;

				cout << "\tC_PMOFFPEAK_EAT_OUT_ACTIVITY = " << C_PMOFFPEAK_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_SOCIAL_ACTIVITY = " << C_PMOFFPEAK_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY = " << C_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY = " << C_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY = " << C_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_LEISURE_ACTIVITY = " << C_PMOFFPEAK_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;

				cout << "\tC_PMPEAK_EAT_OUT_ACTIVITY = " << C_PMPEAK_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_SOCIAL_ACTIVITY = " << C_PMPEAK_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_MAJOR_SHOPPING_ACTIVITY = " << C_PMPEAK_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_OTHER_SHOPPING_ACTIVITY = " << C_PMPEAK_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_PERSONAL_BUSINESS_ACTIVITY = " << C_PMPEAK_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_LEISURE_ACTIVITY = " << C_PMPEAK_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;

				cout << "\tC_EVENING_EAT_OUT_ACTIVITY = " << C_EVENING_EAT_OUT_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_SOCIAL_ACTIVITY = " << C_EVENING_SOCIAL_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_MAJOR_SHOPPING_ACTIVITY = " << C_EVENING_MAJOR_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_OTHER_SHOPPING_ACTIVITY = " << C_EVENING_OTHER_SHOPPING_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_PERSONAL_BUSINESS_ACTIVITY = " << C_EVENING_PERSONAL_BUSINESS_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_LEISURE_ACTIVITY = " << C_EVENING_LEISURE_ACTIVITY<float>() << endl;
				cout << "\tC_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY = " << C_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY<float>() << endl;


			}

			//==============================================================================================================================
			// PARAMETER DECLARATIONS
			//--------------------------------------------------
			#pragma region
			// Start time, hybrid_rrm_rum parameters
			m_static_data(string, START_DURATION_DISTRIBUTION_FILE, NONE, NONE);
			m_static_data(float, S_AMPEAK_AGE_60, NONE, NONE);
			m_static_data(float, S_AMPEAK_HH_WORKERS, NONE, NONE);
			m_static_data(float, S_AMPEAK_FLEX_START, NONE, NONE);
			m_static_data(float, S_AMPEAK_FLEX_DURATION, NONE, NONE);
			m_static_data(float, S_AMPEAK_PARTY_JOINT, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_WORK_PARTTIME, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_STUDENT_FULLTIME, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_MODE_PASSENGER, NONE, NONE);
			m_static_data(float, S_PMOFFPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, S_PMOFFPEAK_INCOME_LOW, NONE, NONE);
			m_static_data(float, S_PMOFFPEAK_MODE_TRANSIT, NONE, NONE);
			m_static_data(float, S_PMPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, S_PMPEAK_WORK_FULLTIME, NONE, NONE);
			m_static_data(float, S_PMPEAK_INCOME_HIGH, NONE, NONE);
			m_static_data(float, S_PMPEAK_HH_WORKERS, NONE, NONE);
			m_static_data(float, S_PMPEAK_FLEX_START, NONE, NONE);
			m_static_data(float, S_EVENING_CONSTANT, NONE, NONE);
			m_static_data(float, S_EVENING_TELEWORK, NONE, NONE);
			m_static_data(float, S_EVENING_INCOME_LOW, NONE, NONE);
			m_static_data(float, S_EVENING_DEGREE_COLLEGE, NONE, NONE);
			m_static_data(float, S_AMPEAK_TT, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_TTV, NONE, NONE);
			m_static_data(float, S_AMOFFPEAK_TT, NONE, NONE);
			m_static_data(float, S_PMOFFPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, S_PMOFFPEAK_TT, NONE, NONE);
			m_static_data(float, S_PMPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, S_PMPEAK_TT, NONE, NONE);
			m_static_data(float, S_EVENING_TT, NONE, NONE);
			// Duration AFT parameters
			m_static_data(float, D_AMPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, D_AMPEAK_FLEX_DURATION, NONE, NONE);
			m_static_data(float, D_AMPEAK_TELEWORK, NONE, NONE);
			m_static_data(float, D_AMPEAK_WORK_FULLTIME, NONE, NONE);
			m_static_data(float, D_AMPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_TELEWORK, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_INCOME_HIGH, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_MODE_AUTO, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_WORK_FULLTIME, NONE, NONE);
			m_static_data(float, D_AMOFFPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, D_PMOFFPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, D_PMOFFPEAK_HH_SIZE, NONE, NONE);
			m_static_data(float, D_PMOFFPEAK_MODE_PASSENGER, NONE, NONE);
			m_static_data(float, D_PMOFFPEAK_AGE_18_24, NONE, NONE);
			m_static_data(float, D_PMOFFPEAK_OCCUPANCY, NONE, NONE);
			m_static_data(float, D_PMPEAK_CONSTANT, NONE, NONE);
			m_static_data(float, D_PMPEAK_PARTY_ALONE, NONE, NONE);
			m_static_data(float, D_PMPEAK_AGE_60, NONE, NONE);
			m_static_data(float, D_EVENING_CONSTANT, NONE, NONE);
			m_static_data(float, D_EVENING_WORK_FULLTIME, NONE, NONE);
			m_static_data(float, D_EVENING_MODE_TRANSIT, NONE, NONE);
			m_static_data(float, D_EVENING_AGE_60, NONE, NONE);
			m_static_data(float, D_NIGHT_INCOME_HIGH, NONE, NONE);
			m_static_data(float, D_NIGHT_LOCATION_SUBURBS, NONE, NONE);
			// COPULA PARAMETERS
			m_static_data(float, THETA_AMPEAK, NONE, NONE);
			m_static_data(float, THETA_AMOFFPEAK, NONE, NONE);
			m_static_data(float, THETA_PMOFFPEAK, NONE, NONE);
			m_static_data(float, THETA_PMPEAK, NONE, NONE);
			m_static_data(float, THETA_EVENING, NONE, NONE);
			m_static_data(float, THETA_NIGHT, NONE, NONE);
			// SCALE PARAMETERS
			m_static_data(float, SIGMA_AMPEAK, NONE, NONE);
			m_static_data(float, SIGMA_AMOFFPEAK, NONE, NONE);
			m_static_data(float, SIGMA_PMOFFPEAK, NONE, NONE);
			m_static_data(float, SIGMA_PMPEAK, NONE, NONE);
			m_static_data(float, SIGMA_EVENING, NONE, NONE);
			m_static_data(float, SIGMA_NIGHT, NONE, NONE);
			// CALIBRATION PARAMETERS
			m_static_data(float, C_NIGHT_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_EAT_OUT_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_SOCIAL_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_MAJOR_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_OTHER_SHOPPING_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_PERSONAL_BUSINESS_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_LEISURE_ACTIVITY, NONE, NONE);
			m_static_data(float, C_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY, NONE, NONE);

			#pragma endregion
			static void default_static_initializer()
			{
				_START_DURATION_DISTRIBUTION_FILE = "start_time_duration_data.txt";
				_S_AMPEAK_AGE_60 = 2.60;
				_S_AMPEAK_HH_WORKERS = -2.19;
				_S_AMPEAK_FLEX_START = -0.91;
				_S_AMPEAK_FLEX_DURATION = -2.73;
				_S_AMPEAK_PARTY_JOINT = -1.04;
				//_S_AMOFFPEAK_CONSTANT = 12.97;
				_S_AMOFFPEAK_WORK_PARTTIME = 0.79;
				_S_AMOFFPEAK_STUDENT_FULLTIME = -1.64;
				_S_AMOFFPEAK_MODE_PASSENGER = -0.92;
				//_S_PMOFFPEAK_CONSTANT = 9.53;
				_S_PMOFFPEAK_INCOME_LOW = 0;
				_S_PMOFFPEAK_MODE_TRANSIT = 0;
				//_S_PMPEAK_CONSTANT = 6.13;
				_S_PMPEAK_WORK_FULLTIME = 0.78;
				_S_PMPEAK_INCOME_HIGH = 0.78;
				_S_PMPEAK_HH_WORKERS = -0.89;
				_S_PMPEAK_FLEX_START = 0;
				//_S_EVENING_CONSTANT = 5.45;
				_S_EVENING_TELEWORK = 1.00;
				_S_EVENING_INCOME_LOW = -0.82;
				_S_EVENING_DEGREE_COLLEGE = 0;
				_S_AMPEAK_TT = -0.20;
				_S_AMOFFPEAK_OCCUPANCY = -0.31;
				_S_AMOFFPEAK_TTV = 0;
				_S_AMOFFPEAK_TT = -0.03;
				_S_PMOFFPEAK_OCCUPANCY = -0.71;
				_S_PMOFFPEAK_TT = -0.08;
				_S_PMPEAK_OCCUPANCY = -0.32;
				_S_PMPEAK_TT = -0.72;
				_S_EVENING_TT = -0.01;
				
				_D_AMPEAK_CONSTANT = 3.25;
				_D_AMPEAK_FLEX_DURATION = 0;
				_D_AMPEAK_TELEWORK = 2.26;
				_D_AMPEAK_WORK_FULLTIME = -4.56;
				_D_AMPEAK_OCCUPANCY = 0;
				_D_AMOFFPEAK_CONSTANT = 3.97;
				_D_AMOFFPEAK_TELEWORK = 0.44;
				_D_AMOFFPEAK_INCOME_HIGH = -0.67;
				_D_AMOFFPEAK_MODE_AUTO = -2.03;
				_D_AMOFFPEAK_WORK_FULLTIME = 0;
				_D_AMOFFPEAK_OCCUPANCY = 0;
				_D_PMOFFPEAK_CONSTANT = 3.74;
				_D_PMOFFPEAK_HH_SIZE = 0.18;
				_D_PMOFFPEAK_MODE_PASSENGER = -0.61;
				_D_PMOFFPEAK_AGE_18_24 = 0;
				_D_PMOFFPEAK_OCCUPANCY = 0;
				_D_PMPEAK_CONSTANT = 3.79;
				_D_PMPEAK_PARTY_ALONE = 0;
				_D_PMPEAK_AGE_60 = 0.23;
				_D_EVENING_CONSTANT = 3.88;
				_D_EVENING_WORK_FULLTIME = -0.40;
				_D_EVENING_MODE_TRANSIT = -0.62;
				_D_EVENING_AGE_60 = 0;
				_D_NIGHT_INCOME_HIGH = -4.22;
				_D_NIGHT_LOCATION_SUBURBS = -4.37;
				
				_THETA_AMPEAK = 0;
				_THETA_AMOFFPEAK = 0;
				_THETA_PMOFFPEAK = 0;
				_THETA_PMPEAK = 0;
				_THETA_EVENING = 0;
				_THETA_NIGHT = 0;
				
				_SIGMA_AMPEAK = 0;
				_SIGMA_AMOFFPEAK = 0;
				_SIGMA_PMOFFPEAK = 0;
				_SIGMA_PMPEAK = 0;
				_SIGMA_EVENING = 0;
				_SIGMA_NIGHT = 0;


				// CALIBRATION PARAMS
				_C_NIGHT_EAT_OUT_ACTIVITY = 2.61059606720439;
				_C_NIGHT_SOCIAL_ACTIVITY = 6.98700764856677;
				_C_NIGHT_MAJOR_SHOPPING_ACTIVITY = -3.04589869582877;
				_C_NIGHT_OTHER_SHOPPING_ACTIVITY = 14.4675887869341;
				_C_NIGHT_PERSONAL_BUSINESS_ACTIVITY = 6.19204064493498;
				_C_NIGHT_LEISURE_ACTIVITY = 5.22978952120311;
				_C_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY = 1.29043965022455;

				_C_AMPEAK_EAT_OUT_ACTIVITY = 3.74971721416919;
				_C_AMPEAK_SOCIAL_ACTIVITY = 4.12153758642278;
				_C_AMPEAK_MAJOR_SHOPPING_ACTIVITY = 2.87623756528352;
				_C_AMPEAK_OTHER_SHOPPING_ACTIVITY = 9.12986388828419;
				_C_AMPEAK_PERSONAL_BUSINESS_ACTIVITY = 4.31657785577272;
				_C_AMPEAK_LEISURE_ACTIVITY = 4.15654954891012;
				_C_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = 4.09986826521304;

				_C_AMOFFPEAK_EAT_OUT_ACTIVITY = 11.331718935833;
				_C_AMOFFPEAK_SOCIAL_ACTIVITY = 11.7664566926882;
				_C_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY = 12.0934757525265;
				_C_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY = 8.78159267310069;
				_C_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY = 12.1356466602979;
				_C_AMOFFPEAK_LEISURE_ACTIVITY = 12.0079277768501;
				_C_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = 11.0542186913043;

				_C_PMOFFPEAK_EAT_OUT_ACTIVITY = 8.30801000730919;
				_C_PMOFFPEAK_SOCIAL_ACTIVITY = 7.79734725904626;
				_C_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY = 8.83601808851042;
				_C_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY = 8.66499856916522;
				_C_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY = 8.10043980112096;
				_C_PMOFFPEAK_LEISURE_ACTIVITY = 6.99350342514512;
				_C_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = 6.59312958980496;

				_C_PMPEAK_EAT_OUT_ACTIVITY = 8.94357322024723;
				_C_PMPEAK_SOCIAL_ACTIVITY = 8.34256878541166;
				_C_PMPEAK_MAJOR_SHOPPING_ACTIVITY = 9.21056324724484;
				_C_PMPEAK_OTHER_SHOPPING_ACTIVITY = 9.06804560100913;
				_C_PMPEAK_PERSONAL_BUSINESS_ACTIVITY = 8.45537889023645;
				_C_PMPEAK_LEISURE_ACTIVITY = 8.49209511098696;
				_C_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY = 8.59599163870728;

				_C_EVENING_EAT_OUT_ACTIVITY = 3.12114894459854;
				_C_EVENING_SOCIAL_ACTIVITY = 3.77645741628581;
				_C_EVENING_MAJOR_SHOPPING_ACTIVITY = 2.7159448813639;
				_C_EVENING_OTHER_SHOPPING_ACTIVITY = 9.10898595252838;
				_C_EVENING_PERSONAL_BUSINESS_ACTIVITY = 2.47001185979996;
				_C_EVENING_LEISURE_ACTIVITY = 2.52539693300196;
				_C_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY = 3.24576153255019;
			}
		};


		// INITIALIZE TIMING CHOICE MODEL STATIC PARAMETERS
		template<typename MasterType, typename InheritanceList> typename Activity_Timing_Chooser_Implementation<MasterType, InheritanceList>::type_of(is_initialized) Activity_Timing_Chooser_Implementation<MasterType,InheritanceList>::_is_initialized = false;
		template<typename MasterType, typename InheritanceList> std::unordered_map<int, std::map<float,pair<Time_Minutes,Time_Minutes>>> Activity_Timing_Chooser_Implementation<MasterType,  InheritanceList>::_start_time_duration_container;
		template<typename MasterType, typename InheritanceList> std::unordered_map<int, std::map<int, float>> Activity_Timing_Chooser_Implementation<MasterType, InheritanceList>::_start_time_container;

		#pragma region Choice option parameters	
		// start parameters
		define_static_member_variable(Activity_Timing_Chooser_Implementation, START_DURATION_DISTRIBUTION_FILE);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_AGE_60);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_HH_WORKERS);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_FLEX_START);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_FLEX_DURATION);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_PARTY_JOINT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_WORK_PARTTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_STUDENT_FULLTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_MODE_PASSENGER);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMOFFPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMOFFPEAK_INCOME_LOW);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMOFFPEAK_MODE_TRANSIT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_WORK_FULLTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_INCOME_HIGH);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_HH_WORKERS);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_FLEX_START);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_EVENING_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_EVENING_TELEWORK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_EVENING_INCOME_LOW);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_EVENING_DEGREE_COLLEGE);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMPEAK_TT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_TTV);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_AMOFFPEAK_TT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMOFFPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMOFFPEAK_TT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_PMPEAK_TT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, S_EVENING_TT);
		//DURATION parameters
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMPEAK_FLEX_DURATION);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMPEAK_TELEWORK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMPEAK_WORK_FULLTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_TELEWORK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_INCOME_HIGH);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_MODE_AUTO);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_WORK_FULLTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_AMOFFPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMOFFPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMOFFPEAK_HH_SIZE);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMOFFPEAK_MODE_PASSENGER);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMOFFPEAK_AGE_18_24);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMOFFPEAK_OCCUPANCY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMPEAK_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMPEAK_PARTY_ALONE);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_PMPEAK_AGE_60);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_EVENING_CONSTANT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_EVENING_WORK_FULLTIME);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_EVENING_MODE_TRANSIT);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_EVENING_AGE_60);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_NIGHT_INCOME_HIGH);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, D_NIGHT_LOCATION_SUBURBS);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_AMPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_AMOFFPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_PMOFFPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_PMPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_EVENING);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, THETA_NIGHT);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_AMPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_AMOFFPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_PMOFFPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_PMPEAK);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_EVENING);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, SIGMA_NIGHT);
		// Calibration parameters
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_NIGHT_RELIGIOUS_OR_CIVIC_ACTIVITY);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_AMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMOFFPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_PMPEAK_RELIGIOUS_OR_CIVIC_ACTIVITY);

		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_EAT_OUT_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_SOCIAL_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_MAJOR_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_OTHER_SHOPPING_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_PERSONAL_BUSINESS_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_LEISURE_ACTIVITY);
		define_static_member_variable(Activity_Timing_Chooser_Implementation, C_EVENING_RELIGIOUS_OR_CIVIC_ACTIVITY);


		#pragma endregion
	}
}
