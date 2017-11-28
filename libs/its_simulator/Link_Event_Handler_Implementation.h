#pragma once
#include"Link_Implementation.h"
#include"Network_Event_Implementations.h"
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
			SNOW_5,
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
		// Adjustment factors recommended by the Guidebook on the Analysis of ATDM Strategies.

		template<typename MasterType,typename InheritanceList>
		float Link_Implementation<MasterType, InheritanceList>::link_capacity_adjustment_factors_for_weather[19]
			= { 1.00, 0.98, 0.98, 0.93, 0.86, 0.96, 0.91, 0.89, 0.78, 0.20, 0.99, 0.98, 0.91, 1.00, 0.99, 0.98, 0.93, 0.88, 0.89 };
		
		template<typename MasterType,typename InheritanceList>
		float Link_Implementation<MasterType, InheritanceList>::link_free_flow_speed_adjustment_factors_for_weather[19][5]
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
				{0.20, 0.20, 0.20, 0.20, 0.20},
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

		template<typename MasterType,typename InheritanceList>
		float Link_Implementation<MasterType, InheritanceList>::link_capacity_adjustment_factors_for_accident[8][5]
			= {
				{0.93, 0.79, 0.01, 0.01, 0.01},
				{0.95, 0.81, 0.35, 0.01, 0.01},
				{0.99, 0.83, 0.49, 0.16, 0.01},
				{0.99, 0.85, 0.58, 0.25, 0.13},
				{0.99, 0.87, 0.65, 0.40, 0.20},
				{0.99, 0.89, 0.71, 0.50, 0.26},
				{0.99, 0.91, 0.75, 0.57, 0.36},
				{0.99, 0.93, 0.28, 0.63, 0.41}
			};

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Link_Implementation<MasterType,InheritanceList>::process_accident_event()
		{
			typedef Network_Event<typename MasterType::accident_network_event_type> _Accident_Event_Interface;
			//cout << "accident " << _current_accident_event->_accident_type << " being processed" << endl;
			float capacity_adjustment_rate;
			float free_flow_speed_adjustment_rate;
			float prev_num_lanes = _num_lanes;
			if (((typename MasterType::accident_network_event_type*)_current_accident_event)->lanes_closed<int>() >= _num_lanes)
			{
				capacity_adjustment_rate = 0.01;
				_num_lanes = 1;
			}
			else
			{
				int lanes = min(8, _num_lanes);
				int lanes_closed = min(3, ((typename MasterType::accident_network_event_type*)_current_accident_event)->lanes_closed<int>());	
				capacity_adjustment_rate = link_capacity_adjustment_factors_for_accident[lanes - 1][lanes_closed + 1];
				_num_lanes = _num_lanes - ((typename MasterType::accident_network_event_type*)_current_accident_event)->lanes_closed<int>();
			}
			_lane_adjustment_due_to_accident = prev_num_lanes - _num_lanes;
			_capacity_adjustment_factor_due_to_accident = capacity_adjustment_rate;
			
			//_maximum_flow_rate *= capacity_adjustment_rate;
			
			free_flow_speed_adjustment_rate = 1.0; // nominal adjustment factor of 1.0 according to the Guidebook
			_speed_adjustment_factor_due_to_accident = free_flow_speed_adjustment_rate;
			//_free_flow_speed *= free_flow_speed_adjustment_rate;
			reset_features<TargetType>();
			//_link_fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Link_Implementation<MasterType,InheritanceList>::revert_accident_event()
		{
			_capacity_adjustment_factor_due_to_accident = 1.0;
			_speed_adjustment_factor_due_to_accident = 1.0;
			_num_lanes += _lane_adjustment_due_to_accident;
			reset_features<TargetType>();
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Link_Implementation<MasterType,InheritanceList>::reset_features()
		{
			_maximum_flow_rate = _original_maximum_flow_rate * _capacity_adjustment_factor_due_to_accident;
			_maximum_flow_rate *= _capacity_adjustment_factor_due_to_weather;
			_free_flow_speed = free_flow_speed_estimate<TargetType>() * _speed_adjustment_factor_due_to_accident;
			_free_flow_speed *= _speed_adjustment_factor_due_to_weather;
			//_free_flow_speed = min(_free_flow_speed, _speed_limit + 5.0f);
			_link_fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Link_Implementation<MasterType,InheritanceList>::process_weather_event()
		{
			float capacity_adjustment_rate;
			float free_flow_speed_adjustment_rate;
			
			int weather_index = get_weather_index<TargetType>((typename MasterType::weather_network_event_type*)_current_weather_event);
			
			capacity_adjustment_rate = link_capacity_adjustment_factors_for_weather[weather_index];
			//capacity_adjustment_rate = 0.2;
			_capacity_adjustment_factor_due_to_weather = capacity_adjustment_rate;
			//_maximum_flow_rate *= capacity_adjustment_rate;

			free_flow_speed_adjustment_rate = find_free_flow_speed_adjustment_rate_for_weather<TargetType>(weather_index);
			//free_flow_speed_adjustment_rate = 0.2;
			_speed_adjustment_factor_due_to_weather = free_flow_speed_adjustment_rate;
			reset_features<TargetType>();
			//_free_flow_speed *= free_flow_speed_adjustment_rate;

			//_link_fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 void Link_Implementation<MasterType,InheritanceList>::revert_weather_event()
		{
			_capacity_adjustment_factor_due_to_weather = 1.0;
			_speed_adjustment_factor_due_to_weather = 1.0;
			reset_features<TargetType>();
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 float Link_Implementation<MasterType,InheritanceList>::find_free_flow_speed_adjustment_rate_for_weather(int weather_index)
		{
			float free_flow_speed_adjustment_rate;
			if (_original_free_flow_speed < 55)
			{
				free_flow_speed_adjustment_rate = link_free_flow_speed_adjustment_factors_for_weather[weather_index][0];
			}
			else if (_original_free_flow_speed < 60)
			{
				free_flow_speed_adjustment_rate = link_free_flow_speed_adjustment_factors_for_weather[weather_index][1];
			}
			else if (_original_free_flow_speed < 65)
			{
				free_flow_speed_adjustment_rate = link_free_flow_speed_adjustment_factors_for_weather[weather_index][2];
			}
			else if (_original_free_flow_speed < 70)
			{
				free_flow_speed_adjustment_rate = link_free_flow_speed_adjustment_factors_for_weather[weather_index][3];
			}
			else
			{
				free_flow_speed_adjustment_rate = link_free_flow_speed_adjustment_factors_for_weather[weather_index][4];
			}
			return free_flow_speed_adjustment_rate;
		}

		template<typename MasterType,typename InheritanceList>
		template<typename TargetType>
		 int Link_Implementation<MasterType,InheritanceList>::get_weather_index(TargetType weather_event)
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
				snow_precipitation_depth = ((_Weather_Event_Interface*)weather_event)->template snow_depth<float>();
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
				else if (snow_precipitation_depth <= 1.0)
				{
					weather_index = Link_Components::Types::SNOW_4;
				}
				else
				{
					weather_index = Link_Components::Types::SNOW_5;
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
