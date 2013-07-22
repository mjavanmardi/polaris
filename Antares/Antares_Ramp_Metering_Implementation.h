//*********************************************************
//	Antares_Ramp_Metering_Implementation.h - Graphical Sensors
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "Antares_Link_Implementation.h"
#include "User_Space\Ramp_Metering_Prototype.h"

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
		implementation struct Antares_Ramp_Metering_Implementation : public Polaris_Ramp_Metering_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Ramp_Metering_Implementation)>
		{
			typedef typename Polaris_Ramp_Metering_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Ramp_Metering_Implementation)>::ComponentType ComponentType;
			
			typedef Link_Components::Implementations::Link_Line<MasterType> Link_Line;
			typedef Intersection_Prototype<typename type_of(MasterType::intersection)> Intersection_Interface;
			typedef Scenario_Prototype<typename MasterType::scenario_type> Scenario_Interface;
			typedef Link_Prototype<typename type_of(MasterType::link)> Link_Interface;

#pragma pack(push,1)
			struct ITS_Location
			{
				void* object;
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

			static bool on_submit(const list<void*>& selected,const vector<string>& attribute_choices,const vector<string>& dropdown_choices)
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

				for(list<void*>::const_iterator sitr=selected.begin();sitr!=selected.end();sitr++)
				{
					ComponentType* its_component=(ComponentType*) (*sitr);

					its_component->enable<ComponentType,NT,NT>(toggle_operation);

					update_successful = true;
				}

				if(update_successful) return true;
				else return false;
			}

			static void on_double_click(const list<void*>& selected,vector<pair<string,string>>& attributes,vector<vector<string>>& dropdowns)
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

			static void on_select(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_its_component_layer->Clear_Accented<NT>();
					MasterType::network_type::_link_lines->Clear_Accented<NT>();

					if(selected.size())
					{
						for(list<void*>::const_iterator itr=selected.begin();itr!=selected.end();itr++)
						{
							((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
						}
					}
				}
				else if(added.size())
				{
					for(list<void*>::const_iterator itr=added.begin();itr!=added.end();itr++)
					{
						((ComponentType*)*itr)->Accent_Self<ComponentType,ComponentType,NT>();
					}
				}

				if(selected.size())
				{
					((ComponentType*) (selected.back()))->Display_Attributes<ComponentType,ComponentType,NT>(bucket);
				}
			}
			
			feature_implementation void Display_Attributes(vector<pair<string,string>>& bucket)
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
				s << meter_flow_ratio<ComponentType,CallerType,TargetType>();
				key_value_pair.second=s.str();
				bucket.push_back(key_value_pair);
				s.str("");
			}

			feature_implementation static void Initialize_Type()
			{
				

				Polaris_Ramp_Metering_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Ramp_Metering_Implementation)>::Initialize_Type<ComponentType,CallerType,TargetType>();

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Ramp Meters"));
				
				Antares_Layer_Configuration cfg;

				cfg.Configure_Dynamic_Points();
				cfg.head_texture = cfg.Add_Texture(string("Ramp_Meter.png"));
				cfg.grouped=false;
				cfg.storage_period = ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				cfg.storage_offset = ((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
				cfg.target_sub_iteration=0;
				cfg.head_size_value=24;
				cfg.head_accent_size_value=48;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;
				cfg.primitive_color=true;

				cfg.head_color._r = 255;
				cfg.head_color._g = 255;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;

				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			
			feature_implementation void Initialize()
			{
				Polaris_Ramp_Metering_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Ramp_Metering_Implementation)>::Initialize<ComponentType,CallerType,TargetType>();

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<ComponentType,CallerType,TargetType>();

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
			

			feature_implementation void Update_Ramp_Meters()
			{
				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<ComponentType,CallerType,TargetType>();

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

			feature_implementation void Accent_Self()
			{
				ITS_Location its_location;

				its_location.object = nullptr;

				Intersection_Interface* upstream_intersection = ((Link_Interface*)_on_ramp_link)->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = ((Link_Interface*)_on_ramp_link)->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );
				
				float current_meter_flow_ratio = meter_flow_ratio<ComponentType,CallerType,TargetType>();

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

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( link_line.up_node );

				intersection = link->downstream_intersection< Intersection_Interface* >();

				link_line.down_node._x = intersection->x_position<float>();
				link_line.down_node._y = intersection->y_position<float>();
				link_line.down_node._z = 3;

				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( link_line.down_node );

				MasterType::network_type::_link_lines->Push_Element<Accented_Element>(&link_line);
			}

			declare_feature_event(Ramp_Metering)
			{
				Polaris_Ramp_Metering_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Ramp_Metering_Implementation)>::Ramp_Metering<NT>(_this);

				ComponentType* pthis = (ComponentType*)_this;

				pthis->Update_Ramp_Meters<ComponentType,NT,NT>();
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Ramp_Metering_Implementation<MasterType,ParentType,InheritanceList>::ComponentType>* Antares_Ramp_Metering_Implementation<MasterType,ParentType,InheritanceList>::_its_component_layer;
	}
}