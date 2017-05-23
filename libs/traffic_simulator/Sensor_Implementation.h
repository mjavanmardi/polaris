#pragma once
#include "Sensor_Prototype.h"
#include "Link_Prototype.h"
#include "Detector_Implementation.h"
#ifndef FOR_LINUX_PORTING
#include "../repository/Basic_Unit_Implementations.h"
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
		implementation struct Link_Sensor:public Polaris_Component<MasterType,INHERIT(Link_Sensor),Execution_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Link_Sensor),Data_Object>::Component_Type ComponentType;
			typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;

// TODO: does not compile
			template<typename TargetType> void Sensor_Conditional()
			{
                //response.next._iteration = iteration() + ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
                //response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;

                //response.result = true;
			}
			
			declare_event(Sensor_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				pthis->Record_Data();

				if(pthis->_detector && (iteration() + 1) % (((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()*10) == 0)
				{
					pthis->Interpret_Data();
				}
			}
			
			void Interpret_Data()
			{
				double sum=0.0;
				unsigned int count=0;
				for(std::vector<Types::Sensor_MOE_Data>::iterator itr = _sensor_data.begin();itr!=_sensor_data.end();itr++)
				{
					sum += itr->density;
					count++;
				}

				double avg = sum/count;

				if( _sensor_data.size() )
				{
					_outlier_detected = _detector->Classify(avg);
				}
				else
				{
					_outlier_detected = false;
				}

				_sensor_data.clear();
			}

			void Record_Data()
			{
				Types::Sensor_MOE_Data new_data;
				
				new_data.time_recorded=iteration();

				_covered_link->template get_prevailing_link_moe<NT>(new_data.volume,new_data.speed,new_data.density);

				_sensor_data.push_back(new_data);
			}
			
			template<typename TargetType> bool Check_Outlier()
			{
				return _outlier_detected;
			}
			
			template<typename TargetType> static void Initialize_Type()
			{
			}

// TODO: does not compile
			/*template<typename TargetType> void Initialize(polaris::io::Fixed_Sensor& instance)
			{
				using namespace polaris::io;

				_detector = nullptr;
				_outlier_detected = false;

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

				link_id_dir.id = instance.getLink();
				link_id_dir.dir = instance.getDir();

				std::unordered_map<long long,void*>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template link_dbid_dir_to_ptr_map<unordered_map<long long,void*>&>();

				if(db_map.count(link_id_dir.id_dir))
				{
                    _covered_link = db_map[link_id_dir.id_dir];

                    //_covered_link = polaris_link;

                    _covered_link->template Push_ITS< ComponentType* >( (ComponentType*)this );

					typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;
					//TODO
//load_event_implementation(ComponentType, ComponentType::template Sensor_Conditional,ComponentType::template Sensor_Event,((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1, Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS, NULLTYPE);
				}
			}
			*/
			template<typename TargetType> void Attach_Detector(std::unordered_map<int, Detector1DU<double> >& detector_configuration)
			{
				if(!_covered_link) return;

				_outlier_detected = false;

				if(detector_configuration.count(_covered_link->template dbid<int>()*2+_covered_link->template direction<int>()))
				{
					_detector = new Detector1DU<double>();

					*_detector = detector_configuration[_covered_link->template dbid<int>()*2+_covered_link->template direction<int>()];
				}
			}


			m_data(Detector1DU<double>*,detector, NONE, NONE);
			
			m_data(bool,outlier_detected, NONE, NONE);

			m_data(std::vector<Types::Sensor_MOE_Data>,sensor_data, NONE, NONE);

			m_prototype(Null_Prototype,typename MasterType::link_type,covered_link, NONE, NONE);
		};
	}
}

using namespace Sensor_Components::Implementations;
