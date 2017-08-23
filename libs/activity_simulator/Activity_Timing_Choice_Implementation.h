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


			// pointer back to parent class
			m_prototype(Prototypes::Person_Planner, typename MasterType::person_planner_type, Parent_Planner, NONE, NONE);
			typedef Scenario_Components::Prototypes::Scenario< typename type_of(Parent_Planner)::type_of(Parent_Person)::type_of(scenario_reference)> _Scenario_Interface;

			// static start time and duration lookup container for each activity type
			static m_container(concat(std::unordered_map<int, map_type >), start_time_duration_container, NONE, NONE);
			

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
				int iter = 0;
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
				}

				// make sure valid entry is found
				if (itr == _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].end()) THROW_EXCEPTION("ERROR: no valid start-time / duration pair found for activity type '" << act->template Activity_Type<ACTIVITY_TYPES>() <<"' and random value = " << rand);
			
				pair<ReturnTimeType,ReturnTimeType> return_val;
				return_val.first = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnTimeType>(itr->second.first);
				return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnTimeType>(itr->second.second);
				return return_val;
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

				// add items
				ifstream data_file;
				data_file.open(_START_DURATION_DISTRIBUTION_FILE.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open 'start_time_duration_data.txt' in the working directory.  Check to make sure the start-time duration file exists.";

				string line;
				int act_type;
				float prob, start, dur;

				getline(data_file,line);

				while(data_file >> act_type >> start >> dur >> prob )
				{
					_start_time_duration_container[act_type].insert(record_type(prob,data_type(start,dur)));
				}

				//copy primary work into work_at_home
				for (map_type::iterator itr = _start_time_duration_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].begin(); itr != _start_time_duration_container[Activity_Components::Types::PRIMARY_WORK_ACTIVITY].end(); ++itr)
				{
					_start_time_duration_container[Activity_Components::Types::WORK_AT_HOME_ACTIVITY].insert(*itr);
				}
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
			#pragma endregion
			static void default_static_initializer()
			{
				_START_DURATION_DISTRIBUTION_FILE = "start_time_duration_data.txt";
				_S_AMPEAK_AGE_60 = 2.60;
				_S_AMPEAK_HH_WORKERS = -2.19;
				_S_AMPEAK_FLEX_START = -0.91;
				_S_AMPEAK_FLEX_DURATION = -2.73;
				_S_AMPEAK_PARTY_JOINT = -1.04;
				_S_AMOFFPEAK_CONSTANT = 12.97;
				_S_AMOFFPEAK_WORK_PARTTIME = 0.79;
				_S_AMOFFPEAK_STUDENT_FULLTIME = -1.64;
				_S_AMOFFPEAK_MODE_PASSENGER = -0.92;
				_S_PMOFFPEAK_CONSTANT = 9.53;
				_S_PMOFFPEAK_INCOME_LOW = 0;
				_S_PMOFFPEAK_MODE_TRANSIT = 0;
				_S_PMPEAK_CONSTANT = 6.13;
				_S_PMPEAK_WORK_FULLTIME = 0.78;
				_S_PMPEAK_INCOME_HIGH = 0.78;
				_S_PMPEAK_HH_WORKERS = -0.89;
				_S_PMPEAK_FLEX_START = 0;
				_S_EVENING_CONSTANT = 5.45;
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

			}
		};


		// INITIALIZE TIMING CHOICE MODEL STATIC PARAMETERS
		template<typename MasterType, typename InheritanceList> typename Activity_Timing_Chooser_Implementation<MasterType, InheritanceList>::type_of(is_initialized) Activity_Timing_Chooser_Implementation<MasterType,InheritanceList>::_is_initialized = false;
		template<typename MasterType, typename InheritanceList> std::unordered_map<int, std::map<float,pair<Time_Minutes,Time_Minutes>>> Activity_Timing_Chooser_Implementation<MasterType,  InheritanceList>::_start_time_duration_container;

		#pragma region Choice option parameters	
		// zero-inflation parameters
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

	}
}
