#pragma once
#include"Polaris_Link_Implementation.h"
namespace Link_Components
{
	namespace Types
	{
		enum Weather_Index
		{
			CLEAR_1 = 0,
			CLEAR_2,
			RAIN_1,
			RAIN_2,
			RAIN_3,
			SNOW_1,
			SNOW_2,
			SNOW_3,
			SNOW_4,
			TEMP_1,
			TEMP_2,
			TEMP_3,
			WIND_1,
			WIND_2,
			WIND_3,
			VISIBILITY_1,
			VISIBILITY_2,
			VISIBILITY_3
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Polaris_Link_Implementation<MasterType,ParentType,InheritanceList>::link_capacity_reduction_factors[18] 
			= {1.00, 0.98, 0.98, 0.93, 0.86, 0.96, 0.91, 0.89, 0.78, 0.99, 0.98, 0.91, 1.00, 0.99, 0.98, 0.93, 0.88, 0.89};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		float Polaris_Link_Implementation<MasterType,ParentType,InheritanceList>::link_free_flow_speed_reduction_factors[18][5]
			= {
				{1.00, 1.00, 1.00, 1.00, 1.00},
				{0.97, 0.96, 0.96, 0.95, 0.94},
				{0.97, 0.96, 0.96, 0.95, 0.94},
				{0.96, 0.95, 0.94, 0.93, 0.93},
				{0.94, 0.93, 0.93, 0.92, 0.91},
				{0.94, 0.92, 0.89, 0.87, 0.84},
				{0.92, 0.90, 0.88, 0.86, 0.83},
				{0.90, 0.88, 0.86, 0.84, 0.82},
				{0.88, 0.86, 0.85, 0.83, 0.81},
				{0.99, 0.99, 0.99, 0.98, 0.98},
				{0.99, 0.98, 0.98, 0.98, 0.97},
				{0.95, 0.95, 0.94, 0.93, 0.92},
				{1.00, 1.00, 1.00, 1.00, 1.00},
				{0.99, 0.98, 0.98, 0.97, 0.96},
				{0.98, 0.98, 0.97, 0.97, 0.96},
				{0.96, 0.95, 0.94, 0.94, 0.93},
				{0.95, 0.94, 0.93, 0.92, 0.91},
				{0.95, 0.94, 0.93, 0.92, 0.91}
			};
		
		feature_implementation_definition void Polaris_Link_Implementation<MasterType,ParentType,InheritanceList>::process_weather_event(TargetType weather_event)
		{
			typedef Network_Event<typename MasterType::weather_network_event_type> _Weather_Event_Interface;
			cout << "weather_type = " << ((_Weather_Event_Interface*)weather_event)->template weather_type<int>();
			float capacity_reduction_rate;
			float free_flow_speed_reduction_rate;
			
			int weather_index = get_weather_index<ComponentType,CallerType,TargetType>(weather_event);
			
			capacity_reduction_rate = link_capacity_reduction_factors[weather_index];
			_maximum_flow_rate *= capacity_reduction_rate;

			free_flow_speed_reduction_rate = find_free_flow_speed_reduction_rate<ComponentType,CallerType,TargetType>(weather_index);
			_free_flow_speed *= free_flow_speed_reduction_rate;

			_link_fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
		}

		feature_implementation_definition float Polaris_Link_Implementation<MasterType,ParentType,InheritanceList>::find_free_flow_speed_reduction_rate(int weather_index)
		{
			float free_flow_speed_reduction_rate;
			if (_original_free_flow_speed < 55)
			{
				free_flow_speed_reduction_rate = link_free_flow_speed_reduction_factors[weather_index][0];
			}
			else if (_original_free_flow_speed < 60)
			{
				free_flow_speed_reduction_rate = link_free_flow_speed_reduction_factors[weather_index][1];
			}
			else if (_original_free_flow_speed < 65)
			{
				free_flow_speed_reduction_rate = link_free_flow_speed_reduction_factors[weather_index][2];
			}
			else if (_original_free_flow_speed < 70)
			{
				free_flow_speed_reduction_rate = link_free_flow_speed_reduction_factors[weather_index][3];
			}
			else
			{
				free_flow_speed_reduction_rate = link_free_flow_speed_reduction_factors[weather_index][4];
			}
			return free_flow_speed_reduction_rate;
		}

		feature_implementation_definition int Polaris_Link_Implementation<MasterType,ParentType,InheritanceList>::get_weather_index(TargetType weather_event)
		{
			typedef Network_Event<typename MasterType::weather_network_event_type> _Weather_Event_Interface;
			int weather_index = 0;
			float rain_precipitation_depth;
			float snow_precipitation_depth;
			float temperature;
			float wind_speed;
			float visibility;

			switch(((_Weather_Event_Interface*)weather_event)->template weather_type<Network_Event_Components::Types::WEATHER_TYPE>())
			{
			case Network_Event_Components::Types::CLEAR_DRY_PAVEMENT:
				weather_index = Link_Components::Types::CLEAR_1;
				break;
			case Network_Event_Components::Types::CLEAR_WET_PAVEMENT:
				weather_index = Link_Components::Types::CLEAR_2;
				break;
			case Network_Event_Components::Types::RAIN:
				rain_precipitation_depth = ((_Weather_Event_Interface*)weather_event)->template precipitation_depth<float>();
				if (rain_precipitation_depth <= 0.10) 
				{
					weather_index = Link_Components::Types::RAIN_1;
				}
				else if (rain_precipitation_depth <= 0.25)
				{
					weather_index = Link_Components::Types::RAIN_2;
				}
				else
				{
					weather_index = Link_Components::Types::RAIN_3;
				}
				break;
			case Network_Event_Components::Types::SNOW:
				snow_precipitation_depth = ((_Weather_Event_Interface*)weather_event)->template precipitation_depth<float>();
				if (snow_precipitation_depth <= 0.05)
				{
					weather_index = Link_Components::Types::SNOW_1;
				}
				else if (snow_precipitation_depth <= 0.1)
				{
					weather_index = Link_Components::Types::SNOW_2;
				}
				else if (snow_precipitation_depth <= 0.5)
				{
					weather_index = Link_Components::Types::SNOW_3;
				}
				else
				{
					weather_index = Link_Components::Types::SNOW_4;
				}
				break;
			case Network_Event_Components::Types::TEMP:
				temperature = ((_Weather_Event_Interface*)weather_event)->template temperature<float>();
				if (temperature < 50)
				{
					weather_index = Link_Components::Types::TEMP_1;
				}
				else if (temperature < 34)
				{
					weather_index = Link_Components::Types::TEMP_2;
				}
				else if (temperature < -4)
				{
					weather_index = Link_Components::Types::TEMP_3;
				}
				break;
			case Network_Event_Components::Types::WIND:
				wind_speed = ((_Weather_Event_Interface*)weather_event)->template wind_speed<float>();
				if (wind_speed < 10)
				{
					weather_index = Link_Components::Types::WIND_1;
				}
				else if (wind_speed <= 20)
				{
					weather_index = Link_Components::Types::WIND_2;
				}
				else
				{
					weather_index = Link_Components::Types::WIND_3;
				}
				break;
			case Network_Event_Components::Types::VISIBILITY:
				visibility = ((_Weather_Event_Interface*)weather_event)->template visibility<float>();
				if (visibility < 1)
				{
					weather_index = Link_Components::Types::VISIBILITY_1;
				}
				else if (visibility <= 0.5)
				{
					weather_index = Link_Components::Types::VISIBILITY_2;
				}
				else
				{
					weather_index = Link_Components::Types::VISIBILITY_3;
				}
				break;
			}

			return weather_index;
		}
	}
}