#pragma once

#include "Zone_Prototype.h"

namespace Zone_Components
{
	namespace Types
	{
		enum SUBITERATIONS
		{
			ZONE_UPDATE_SUBITERATION=20
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Zone_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Zone_Implementation),MasterType,Execution_Object,ParentType>
		{
			typedef Zone_Prototype<ComponentType> _this_interface;

			int production_counter[_num_threads];
			int attraction_counter[_num_threads];

			feature_implementation void Initialize()
			{
				_zone_is_available=true;
				_population = 100;
				_this_interface* pthis = (_this_interface*)this;
				pthis->update_increment<Time_Minutes>(2);
				load_event(ComponentType,Default_Zone_Conditional,Default_Zone_Event,concat(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(2)),0,NULLTYPE);
			}
			declare_feature_conditional(Default_Zone_Conditional)
			{
				_this_interface* pthis = (_this_interface*)_this;

				response.result=true;
				response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(pthis->update_increment<Simulation_Timestep_Increment>());
				response.next._sub_iteration=Types::ZONE_UPDATE_SUBITERATION;
			}

			declare_feature_event(Default_Zone_Event)
			{
				//_this_interface* pthis = (_this_interface*)_this;

				//cout <<endl<<"Productions= " << pthis->production_count<int>() << ", Attractions= " << pthis->attraction_count<int>();
				//for (int i=0; i < _num_threads; i++)
				//{
				//	((ComponentType*)_this)->production_counter[i]=0;
				//	((ComponentType*)_this)->attraction_counter[i]=0;
				//}
			}

			member_data(bool, zone_is_available,check(ReturnValueType, is_integral), check(SetValueType, is_integral));
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(double, X, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, Y, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, population, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data_component(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Update_Increment,none,none);
			member_component_feature(update_increment, _Update_Increment, Value, Basic_Units::Prototypes::Time_Prototype);

			// production and attraction counters
			feature_implementation void reset_counters()
			{
				for (int i=0; i< _num_threads; i++) production_counter[i]=0;
				for (int i=0; i< _num_threads; i++) attraction_counter[i]=0;
			}
			feature_implementation TargetType production_count(requires(check_as_given(TargetType, !is_reference) && check(TargetType, is_integral)))
			{
				TargetType productions = 0;
				for (int i=0; i< _num_threads; i++) productions+=production_counter[i];
				return productions;
			} 	
			feature_implementation TargetType attraction_count(requires(check_as_given(TargetType, !is_reference) && check(TargetType, is_integral)))
			{
				int attractions = 0;
				for (int i=0; i< _num_threads; i++) attractions+=attraction_counter[i];
				return attractions;
			}
			feature_implementation TargetType production_count(requires(check_as_given(TargetType, is_reference) && check(TargetType, is_integral)))
			{
				return (TargetType)production_counter[_thread_id];
			}	
			feature_implementation TargetType attraction_count(requires(check_as_given(TargetType, is_reference) && check(TargetType, is_integral)))
			{
				return (TargetType)attraction_counter[_thread_id];
			}	
			tag_getter_as_available(production_count);	
			tag_getter_as_available(attraction_count);

			member_container(vector<typename MasterType::activity_location_type*>, origin_activity_locations, none, none);

			member_container(vector<typename MasterType::link_type*>, destination_activity_locations, none, none);

			member_container(vector<float>, origin_activity_location_choice_cdfs, none, none);

			member_container(vector<float>, destination_activity_location_choice_cdfs, none, none);

		};
	}

}