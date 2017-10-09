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
		implementation struct Zone_Implementation:public Polaris_Component<MasterType,INHERIT(Zone_Implementation),Execution_Object>
		{
			typedef Polaris_Component<MasterType,INHERIT(Zone_Implementation),Execution_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;

			typedef Zone<typename MasterType::zone_type> _this_interface;

			int* production_counter;
			int* attraction_counter;

			Zone_Implementation()
			{
				production_counter = new int[num_sim_threads()+1];
				attraction_counter = new int[num_sim_threads()+1];
				_uuid = -1;
				_internal_id = -2;
			}
			template<typename TargetType> void Initialize()
			{
				_employment_simulated=0;
				_zone_is_available=true;
				_this_interface* pthis = (_this_interface*)this;
				pthis->update_increment<Time_Minutes>(2);

				((ComponentType*)this)->template Load_Event<ComponentType>(&Default_Zone_Conditional,concat(Simulation_Time.Future_Time<Time_Minutes,Simulation_Timestep_Increment>(2)),0);		
			}
			static void Default_Zone_Conditional(ComponentType* _this,Event_Response& response)
			{
				_this_interface* pthis = (_this_interface*)_this;

				response.next._iteration=Simulation_Time.Future_Time<Simulation_Timestep_Increment,Simulation_Timestep_Increment>(pthis->update_increment<Simulation_Timestep_Increment>());
				response.next._sub_iteration=Types::ZONE_UPDATE_SUBITERATION;
				
				_this->Default_Zone_Event();
			}

			void Default_Zone_Event()
			{
				//cout <<endl<<"Productions= " << this->template production_count<int>() << ", Attractions= " << this->template attraction_count<int>();
				for (uint i=0; i < num_sim_threads()+1; i++)
				{
					((ComponentType*)this)->production_counter[i]=0;
					((ComponentType*)this)->attraction_counter[i]=0;
				}
			}

			m_data(bool, zone_is_available, NONE, NONE);
			m_data(int, uuid, NONE, NONE);
			m_data(int, internal_id, NONE, NONE);
			m_data(double, X, NONE, NONE);
			m_data(double, Y, NONE, NONE);
			m_data(float, race_percent_white, NONE, NONE);
			m_data(float, race_percent_black, NONE, NONE);
			m_data(int, areatype, NONE,NONE);

			member_component_and_feature_accessor(average_household_income,Value,Basic_Units::Prototypes::Currency,Basic_Units::Implementations::Currency_Implementation<NT>);
			member_component_and_feature_accessor(area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(entertainment_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(industrial_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(institutional_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(mixed_use_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(office_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(other_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(residential_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(retail_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);
			member_component_and_feature_accessor(school_area,Value,Basic_Units::Prototypes::Area,Basic_Units::Implementations::Area_Implementation<NT>);

			member_component_and_feature_accessor(avg_ttime_transit,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_ovtt_transit, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(min_ovtt_transit, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_fare_transit, Value, Basic_Units::Prototypes::Currency, Basic_Units::Implementations::Currency_Implementation<NT>);
			member_component_and_feature_accessor(avg_wait_transit, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_distance, Value, Basic_Units::Prototypes::Length, Basic_Units::Implementations::Length_Implementation<NT>);
			//member_component_and_feature_accessor(avg_ttime_auto_to_transit_accessible_zones,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_ttime_auto_ampeak,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_ttime_auto_pmpeak, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_ttime_auto_offpeak,Value,Basic_Units::Prototypes::Time,Basic_Units::Implementations::Time_Implementation<NT>);
			member_component_and_feature_accessor(avg_ttime_auto_night, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<NT>);
			m_data(float, avg_ttime_var_auto_ampeak, NONE, NONE);
			m_data(float, avg_ttime_var_auto_pmpeak, NONE, NONE);
			m_data(float, avg_ttime_var_auto_offpeak, NONE, NONE);
			m_data(float, avg_ttime_var_auto_night, NONE, NONE);

			m_data(int, pop_households, NONE, NONE);
			m_data(int, pop_persons, NONE, NONE);
			m_data(int, pop_group_quarters, NONE, NONE);
			m_data(int, employment_total, NONE, NONE);
			m_data(int, employment_retail, NONE, NONE);
			m_data(int, employment_government, NONE, NONE);
			m_data(int, employment_manufacturing, NONE, NONE);
			m_data(int, employment_services, NONE, NONE);
			m_data(int, employment_industrial, NONE, NONE);
			m_data(int, employment_other, NONE, NONE);
			m_data(int, accessibility_employment_retail, NONE, NONE);
			m_data(int, accessibility_employment_government, NONE, NONE);
			m_data(int, accessibility_employment_manufacturing, NONE, NONE);
			m_data(int, accessibility_employment_services, NONE, NONE);
			m_data(int, accessibility_employment_industrial, NONE, NONE);
			m_data(int, accessibility_employment_other, NONE, NONE);

			m_data(int, employment_simulated, NONE, NONE);


			//m_data(typename Basic_Units::Implementations::Time_Implementation<MasterType>,_Update_Increment, NONE, NONE);
			member_component_and_feature_accessor(update_increment, Value, Basic_Units::Prototypes::Time, Basic_Units::Implementations::Time_Implementation<MasterType>);

			// production and attraction counters
			template<typename TargetType> void reset_counters()
			{
				for (int i=0; i< num_sim_threads()+1; i++) production_counter[i]=0;
				for (int i=0; i< num_sim_threads()+1; i++) attraction_counter[i]=0;
			}
			template<typename TargetType> TargetType production_count(requires(TargetType,check(TargetType, !is_reference) && check(strip_modifiers(TargetType), is_integral)))
			{
				TargetType productions = 0;
				for (int i=0; i< num_sim_threads()+1; i++) productions+=production_counter[i];
				return productions;
			} 	
			template<typename TargetType> TargetType attraction_count(requires(TargetType,check(TargetType, !is_reference) && check(strip_modifiers(TargetType), is_integral)))
			{
				int attractions = 0;
				for (int i=0; i< num_sim_threads()+1; i++) attractions+=attraction_counter[i];
				return attractions;
			}
			template<typename TargetType> TargetType production_count(requires(TargetType,check(TargetType, is_reference) && check(strip_modifiers(TargetType), is_integral)))
			{
				return (TargetType)production_counter[__thread_id];
			}	
			template<typename TargetType> TargetType attraction_count(requires(TargetType,check(TargetType, is_reference) && check(strip_modifiers(TargetType), is_integral)))
			{
				return (TargetType)attraction_counter[__thread_id];
			}	
			tag_getter_as_available(production_count);	
			tag_getter_as_available(attraction_count);

			template<typename AreaType, typename ReturnType> ReturnType population_density(requires(ReturnType, check(ReturnType, is_arithmetic)))
			{
				return (ReturnType)this->_pop_persons / (ReturnType)this->area<AreaType>();
			}

			m_container(std::vector<typename MasterType::activity_location_type*>, origin_activity_locations, NONE, NONE);

			m_container(std::vector<typename MasterType::link_type*>, destination_activity_locations, NONE, NONE);

			m_container(std::vector<float>, origin_activity_location_choice_cdfs, NONE, NONE);

			m_container(std::vector<float>, destination_activity_location_choice_cdfs, NONE, NONE);

			m_container(std::vector<typename MasterType::activity_location_type*>, home_locations, NONE, NONE);
			m_container(std::vector<typename MasterType::activity_location_type*>, work_locations, NONE, NONE);
			m_container(std::vector<typename MasterType::activity_location_type*>, discretionary_locations, NONE, NONE);
			m_container(std::vector<typename MasterType::activity_location_type*>, school_locations, NONE, NONE);

			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, origin_links, NONE, NONE);
			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, destination_links, NONE, NONE);
			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, origin_walk_links, NONE, NONE);
			m_container(std::vector<Link_Components::Prototypes::Link<typename MasterType::link_type>*>, destination_walk_links, NONE, NONE);

		};
	}

}
