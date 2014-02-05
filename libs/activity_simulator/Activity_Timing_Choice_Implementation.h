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
			typedef map<float,data_type> map_type;


			// pointer back to parent class
			m_prototype(Prototypes::Person_Planner< typename MasterType::person_planner_type>,Parent_Planner, NONE, NONE);


			// static start time and duration lookup container for each activity type
			static m_container(concat(boost::unordered::unordered_map<int, map_type >), start_time_duration_container, NONE, NONE);
			

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
			tag_feature_signature_as_available(Get_Start_Time,1);
			template<typename ActivityRefType, typename TimeType, typename ReturnType> ReturnType Get_Start_Time(ActivityRefType activity_ref, TimeType range_start, TimeType range_end)
			{
				pair<ReturnType, ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref,range_start,range_end);
				return return_val.first;
			}
			tag_feature_signature_as_available(Get_Start_Time,3);
			template<typename ActivityRefType, typename ReturnType> ReturnType Get_Duration(ActivityRefType activity_ref)
			{
				pair<ReturnType, ReturnType> return_val = Get_Start_Time_and_Duration(activity_ref);
				return return_val.second;
			}
			tag_feature_signature_as_available(Get_Duration,1);
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
					THROW_WARNING("ERROR: no valid start-time / duration pair found for activity type '" << act->template Activity_Type<ACTIVITY_TYPES>() <<"' and random value = " << rand);
					itr = _start_time_duration_container[(int)act->template Activity_Type<ACTIVITY_TYPES>()].begin();
				}
				pair<ReturnType,ReturnType> return_val;

				// add random draw from between 0-30 minutes as this is the aggregation level of the start_time data
				return_val.first = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(itr->second.first + (0.5f - GLOBALS::Uniform_RNG.template Next_Rand<float>())*30.0f);
				
				// add random draw from between 0-5 minutes as this is the aggregation level of the duration data
				return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(itr->second.second + (0.5f - GLOBALS::Uniform_RNG.template Next_Rand<float>())*5.0f);
				
				// make sure duration is greater than 5 minutes
				if (return_val.second < GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,ReturnType>(5.0f)) return_val.second = GLOBALS::Time_Converter.template Convert_Value<Time_Minutes, ReturnType>(5.0f);
				
				return return_val;
			}
			tag_feature_signature_as_available(Get_Start_Time_and_Duration,1);
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
			tag_feature_signature_as_available(Get_Start_Time_and_Duration,3);


			// Static initialization
			static m_data(bool, is_initialized, NONE, NONE);
			static void static_initializer(string filename)
			{
				// make sure this is only run once
				if (_is_initialized) return;
				_is_initialized = true;


				// Initialize hashmap for start time
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::PRIMARY_WORK_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::SCHOOL_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::EAT_OUT_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::ERRANDS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::HEALTHCARE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::LEISURE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::MAJOR_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::OTHER_SHOPPING_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::PERSONAL_BUSINESS_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::PICK_UP_OR_DROP_OFF_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::RELIGIOUS_OR_CIVIC_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::SERVICE_VEHICLE_ACTIVITY,map_type()));
				_start_time_duration_container.insert(pair<int,map_type>(ACTIVITY_TYPES::SOCIAL_ACTIVITY,map_type()));


				// add items
				ifstream data_file;
				data_file.open(filename.c_str(), ios::in);
				if (!data_file.is_open()) cout << endl << "Could not open 'start_time_duration_data.txt' in the working directory.  Check to make sure the start-time duration file exists.";

				string line;
				int act_type;
				float prob, start, dur;

				getline(data_file,line);

				while(data_file >> act_type >> start >> dur >> prob )
				{
					_start_time_duration_container[act_type].insert(record_type(prob,data_type(start,dur)));
				}
			}
		};

		static_member_initialization(Activity_Timing_Chooser_Implementation,is_initialized,false);
		//static_member_definition(Activity_Timing_Chooser_Implementation, start_time_duration_container);
		template<typename MasterType, typename InheritanceList> Pair_Associative_Container<boost::unordered::unordered_map<int, map<float,pair<Time_Minutes,Time_Minutes>>>> Activity_Timing_Chooser_Implementation<MasterType,  InheritanceList>::_start_time_duration_container;
	}
}
