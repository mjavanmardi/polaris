#pragma once
#include "Sensor_Prototype.h"
#include "Link_Prototype.h"
#ifndef FOR_LINUX_PORTING
#include "../Repository/Basic_Unit_Implementations.h"
#endif
namespace Sensor_Components
{
	namespace Types
	{
		struct Sensor_MOE_Data
		{
			int time_recorded;

			int volume;
			float speed;
			float density;
		};
	}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Link_Sensor:public Polaris_Component<APPEND_CHILD(Link_Sensor),MasterType,Execution_Object,ParentType>
		{
			typedef typename Polaris_Component<APPEND_CHILD(Link_Sensor),MasterType,Data_Object>::Component_Type ComponentType;
			
			declare_feature_conditional_implementation(Sensor_Conditional)
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> Scenario_Interface;

				response.next._iteration = _iteration + ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;

				response.result = true;
			}

			declare_feature_event_implementation(Sensor_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				pthis->Record_Data();
			}

			void Record_Data()
			{
				Types::Sensor_MOE_Data new_data;
				
				new_data.time_recorded=_iteration;

				_covered_link->get_prevailing_link_moe<NT>(new_data.volume,new_data.speed,new_data.density);

				_sensor_data.push_back(new_data);
			}

			feature_implementation static void Initialize_Type()
			{
			}

			feature_implementation void Initialize(polaris::io::Fixed_Sensor& instance)
			{
				using namespace polaris::io;

				
				_covered_link = nullptr;

				union Link_ID_Dir
				{
					struct
					{
						int id;
						int dir;
					};
					long long id_dir;
				} link_id_dir;

				link_id_dir.id = instance.link;
				link_id_dir.dir = instance.dir;

				unordered_map<long long,void*>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->template link_dbid_dir_to_ptr_map<unordered_map<long long,void*>&>();

				if(db_map.count(link_id_dir.id_dir))
				{
					covered_link_interface* polaris_link = (covered_link_interface*)db_map[link_id_dir.id_dir];

					_covered_link = polaris_link;

					polaris_link->template Push_ITS< ComponentType* >( (ComponentType*)this );

					typedef Scenario_Prototype<typename MasterType::scenario_type> Scenario_Interface;
					Load_Event<ComponentType>(&Sensor_Conditional<ComponentType,ComponentType,NT>,&Sensor_Event<ComponentType,ComponentType,NT>,((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1, Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS);
				}
			}

			member_data(vector<Types::Sensor_MOE_Data>,sensor_data,none,none);

			member_prototype(Link_Prototype,covered_link,typename type_of(MasterType::link),none,none);
		};
	}
}

using namespace Sensor_Components::Implementations;