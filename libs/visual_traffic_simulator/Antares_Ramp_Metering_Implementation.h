//*********************************************************
//	Antares_Ramp_Metering_Implementation.h - Graphical Sensors
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Antares_Link_Implementation.h"
#include "Traffic_Simulator\Ramp_Metering_Implementation.h"

namespace Ramp_Metering_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		implementation struct Antares_Ramp_Metering_Implementation : public Ramp_Metering_Implementation<MasterType,INHERIT(Antares_Ramp_Metering_Implementation)>
		{
			typedef typename Ramp_Metering_Implementation<MasterType,INHERIT(Antares_Ramp_Metering_Implementation)>::ComponentType ComponentType;
			
			typedef Link_Components::Implementations::Link_Line<MasterType> Link_Line;
			typedef Intersection<typename MasterType::intersection_type> Intersection_Interface;
			typedef Scenario<typename MasterType::scenario_type> Scenario_Interface;
			typedef Link<typename MasterType::link_type> Link_Interface;

#pragma pack(push,1)
			struct ITS_Location
			{
				void* object;
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

			static bool on_submit(const std::list<void*>& selected,const std::vector<string>& attribute_choices,const std::vector<string>& dropdown_choices)
			{
				string user_event_choice = dropdown_choices[0];

				bool toggle_operation;

				if(user_event_choice == "Enable")
				{
					toggle_operation=true;
				}
				else
				{
					toggle_operation=false;
				}

				bool update_successful = false;

				for(std::list<void*>::const_iterator sitr=selected.begin();sitr!=selected.end();sitr++)
				{
					ComponentType* its_component=(ComponentType*) (*sitr);

					its_component->enable<NT>(toggle_operation);

					update_successful = true;
				}

				if(update_successful) return true;
				else return false;
			}

			static void on_double_click(const std::list<void*>& selected,std::vector<pair<string,string>>& attributes,std::vector<std::vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				pair<string,string> key_value_pair;
				stringstream s;

				key_value_pair.first="Operation Status";

				if(its_component->_operation_status)
				{
					s << "Enabled";
				}
				else
				{
					s << "Disabled";
				}
				key_value_pair.second=s.str();
				attributes.push_back(key_value_pair);
				s.str("");


				dropdowns.resize(1);

				dropdowns[0].push_back( string("Enable") );
				dropdowns[0].push_back( string("Disable") );
			}

			static void on_select(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();
					MasterType::network_type::_link_lines->Clear_Accented<NT>();

					if(selected.size())
					{
						for(std::list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<NT>();
						}
					}
				}
				else if(added.size())
				{
					for(std::list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<NT>();
					}
				}

				if(selected.size())
				{
					((ComponentType*) (selected.back()))->Display_Attributes<NT>(bucket);
				}
			}
			
			template<typename TargetType> void Display_Attributes(std::vector<pair<string,string>>& bucket)
			{
				pair<string,string> key_value_pair;

				stringstream s;

				key_value_pair.first="Operation Status";

				if(_operation_status)
				{
					s << "Enabled";
				}
				else
				{
					s << "Disabled";
				}
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Starting Time";
				s << (_starting_time / 60);
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Ending Time";
				s << (_ending_time / 60);
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Update Interval";
				s << _metering_updating_interval_length;
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Alpha";
				s << _alpha;
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Beta";
				s << _beta;
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Downstream Freeway Occupancy";
				s << _downstream_freeway_link_occupancy;
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");

				key_value_pair.first="Flow Ratio";
				s << meter_flow_ratio<TargetType>();
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");
			}

			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*36.0f+20.0f);
				return size;
			}
			
			static float accented_pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*48.0f+26.0f);
				return size;
			}

			template<typename TargetType> static void Initialize_Type()
			{
				Ramp_Metering_Implementation<MasterType,INHERIT(Antares_Ramp_Metering_Implementation)>::Initialize_Type<TargetType>();

				_its_component_layer=Allocate_New_Layer<MT>(string("Ramp Meters"));
				
				Antares_Layer_Configuration cfg;

				cfg.Configure_Dynamic_Points();
				cfg.head_texture = cfg.Add_Texture(string("C:\\opt\\polarisdeps\\antares\\Ramp_Meter.png"));
				cfg.grouped=false;
				//cout << "Ramp meters go at offset: " << ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1 << " period " << ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() << endl;

				cfg.storage_period = ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				cfg.storage_offset = ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
				cfg.target_sub_iteration=Scenario_Components::Types::END_OF_ITERATION+1;
				cfg.head_size_value=24;
				cfg.head_accent_size_value=48;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;
				cfg.primitive_color=true;
				cfg.pixel_size_callback = &pixel_size_callback;
				cfg.accent_pixel_size_callback = &accented_pixel_size_callback;

				cfg.head_color._r = 255;
				cfg.head_color._g = 255;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			
			template<typename TargetType> void Initialize()
			{
				Ramp_Metering_Implementation<MasterType,INHERIT(Antares_Ramp_Metering_Implementation)>::Initialize<TargetType>();

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;

				Scale_Coordinates<MT>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<TargetType>();

				if(_operation_status)
				{
					its_location.color._r = (unsigned char)(255.0f*(1.0f - current_meter_flow_ratio));
					its_location.color._g = 255;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}
				else
				{
					its_location.color._r = 0;
					its_location.color._g = 0;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}

				_its_component_layer->Push_Element<Regular_Element>(&its_location);
			}
			

			template<typename TargetType> void Update_Ramp_Meters()
			{
				//cout << "Update ramp meter firing: " << _iteration << "," << sub_iteration() << " : " << Scenario_Components::Types::Type_Sub_Iteration_keys::RAMP_METERING_SUB_ITERATION << endl;

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;

				Scale_Coordinates<MT>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<TargetType>();

				//if(current_meter_flow_ratio < .5)
				//{
				//	its_location.color._r = 255;
				//	its_location.color._g = (unsigned char)(255.0f*(2.0f*(current_meter_flow_ratio)));
				//}
				//else
				//{
				//	its_location.color._r = (unsigned char)(255.0f*(2.0f*(1.0f - current_meter_flow_ratio)));
				//	its_location.color._g = 255;
				//}

				if(_operation_status)
				{
					its_location.color._r = (unsigned char)(255.0f*(1.0f - current_meter_flow_ratio));
					its_location.color._g = 255;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}
				else
				{
					its_location.color._r = 0;
					its_location.color._g = 0;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}
				_its_component_layer->Push_Element<Regular_Element>(&its_location);
			}

			template<typename TargetType> void Accent_Self()
			{
				ITS_Location its_location;

				its_location.object = nullptr;

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;

				Scale_Coordinates<MT>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<TargetType>();

				if(_operation_status)
				{
					its_location.color._r = (unsigned char)(255.0f*(1.0f - current_meter_flow_ratio));
					its_location.color._g = 255;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}
				else
				{
					its_location.color._r = 0;
					its_location.color._g = 0;
					its_location.color._b = 0;
					its_location.color._a = 255;
				}

				_its_component_layer->Push_Element<Accented_Element>(&its_location);



				Link_Line link_line;

				link_line.color._r = 255;
				link_line.color._g = 50;
				link_line.color._b = 50;
				link_line.color._a = 200;

				link_line.data = nullptr;

				Link_Interface* link = (Link_Interface*)_on_ramp_link;
				
				Intersection_Interface* intersection;
				
				intersection = link->upstream_intersection< Intersection_Interface* >();
				
				link_line.up_node._x = intersection->x_position<float>();
				link_line.up_node._y = intersection->y_position<float>();
				link_line.up_node._z = 3;

				Scale_Coordinates<MT>( link_line.up_node );

				intersection = link->downstream_intersection< Intersection_Interface* >();

				link_line.down_node._x = intersection->x_position<float>();
				link_line.down_node._y = intersection->y_position<float>();
				link_line.down_node._z = 3;

				Scale_Coordinates<MT>( link_line.down_node );

				MasterType::network_type::_link_lines->Push_Element<Accented_Element>(&link_line);
			}

			void Ramp_Metering_Event()
			{
				Ramp_Metering_Implementation<MasterType,INHERIT(Antares_Ramp_Metering_Implementation)>::Ramp_Metering_Event();

				ComponentType* pthis = (ComponentType*)this;

				pthis->Update_Ramp_Meters<NT>();
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,its_component_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Ramp_Metering_Implementation<MasterType,InheritanceList>::_its_component_layer;
	}
}
