//*********************************************************
//	Antares_Depot_Implementation.h - Graphical Depot
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Depot_Prototype.h"

namespace Depot_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		implementation struct Antares_Tow_Truck : public Tow_Truck_Implementation<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck)>
		{
			typedef Link_Components::Implementations::Link_Line<MasterType> Link_Line;

			feature_implementation static void Initialize_Type()
			{
				Antares_Layer_Configuration cfg;

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Tow Truck Locations"));

				cfg.Configure_Dynamic_Points();
				cfg.head_texture = cfg.Add_Texture(string("Tow_Truck.png"));
				cfg.grouped=false;
				cfg.head_size_value=16;
				cfg.head_accent_size_value=32;
				cfg.target_sub_iteration=0;
				cfg.storage_offset=((Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1;
				cfg.storage_period=60*5;
				cfg.selection_callback=&on_select;
				cfg.double_click_callback=&on_double_click;
				cfg.submission_callback=&on_submit;

				cfg.head_color._r = 50;
				cfg.head_color._g = 150;
				cfg.head_color._b = 225;
				cfg.head_color._a = 255;
				
				_its_component_layer->Initialize<NULLTYPE>(cfg);
			}

			int Compute_Travel_Time(Network_Event< typename type_of(MasterType::base_network_event) >* net_event)
			{



				vector<Link_Interface*>& event_links = net_event->affected_links< vector<Link_Interface*>& >();

				if(event_links.size())
				{
					Intersection_Interface* upstream_intersection = _truck_location->upstream_intersection<Intersection_Interface*>();
					Intersection_Interface* downstream_intersection = _truck_location->downstream_intersection<Intersection_Interface*>();
							
					float truck_x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
					float truck_y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;


					upstream_intersection = event_links[0]->upstream_intersection<Intersection_Interface*>();
					downstream_intersection = event_links[0]->downstream_intersection<Intersection_Interface*>();

					float event_x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
					float event_y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;


					float distance = sqrt((truck_x - event_x)*(truck_x - event_x) + (truck_y - event_y)*(truck_y - event_y));

					int time = (int)(distance / 22.0f);

					return time;
				}
				else
				{
					return 0;
				}

			}

			static bool on_submit(const list<void*>& selected,const vector<string>& attribute_choices,const vector<string>& dropdown_choices)
			{
				string user_event_choice = dropdown_choices[0];
				bool update_successful = false;

				//for(list<void*>::const_iterator sitr=selected.begin();sitr!=selected.end();sitr++)
				//{
				//	ComponentType* its_component=(ComponentType*) (*sitr);
					
					ComponentType* its_component=(ComponentType*)selected.back();

					vector< Network_Event< typename type_of(MasterType::base_network_event) >* >& current_events = its_component->_associated_depot->current_events< vector< Network_Event< typename type_of(MasterType::base_network_event) >* >& >();

					for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=current_events.begin();itr!=current_events.end();itr++)
					{
						if(user_event_choice == (*itr)->notes<string&>())
						{
							Network_Event< typename type_of(MasterType::base_network_event) >* net_event = *itr;

							
							
							int travel_time = its_component->Compute_Travel_Time( net_event );

							const int towing_time = 20*60;



							int end_time = net_event->end_time<int>();

							end_time = min(end_time,_iteration + travel_time + towing_time);

							net_event->end_time<int>(end_time);

							//its_component->_displayed_events.clear();
							//its_component->_displayed_events.push_back( *itr );

							update_successful = true;
						}
					}
				//}

				if(update_successful) return true;
				else return false;
			}

			static void on_double_click(const list<void*>& selected,vector<pair<string,string>>& attributes,vector<vector<string>>& dropdowns)
			{
				ComponentType* its_component=(ComponentType*)selected.back();

				dropdowns.resize(1);

				vector< Network_Event< typename type_of(MasterType::base_network_event) >* >& current_events = its_component->_associated_depot->current_events< vector< Network_Event< typename type_of(MasterType::base_network_event) >* >& >();

				for(vector< Network_Event< typename type_of(MasterType::base_network_event) >* >::iterator itr=current_events.begin();itr!=current_events.end();itr++)
				{
					dropdowns[0].push_back( (*itr)->notes<string&>() );
				}
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

				//if(selected.size())
				//{
				//	((ComponentType*) (selected.back()))->Display_Attributes<ComponentType,ComponentType,NT>(bucket);
				//}
			}

#pragma pack(push,1)
			struct Truck_Location
			{
				void* object;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

			feature_implementation void Accent_Self()
			{

				Truck_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = _truck_location->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = _truck_location->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );

				_its_component_layer->Push_Element<Accented_Element>(&its_location);


				Link_Line link_line;

				link_line.color._r = 255;
				link_line.color._g = 50;
				link_line.color._b = 50;
				link_line.color._a = 200;

				link_line.data = nullptr;




				Link_Interface* link = _truck_location;
				
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

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);


		};

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Tow_Truck<MasterType,ParentType,InheritanceList>::ComponentType>* Antares_Tow_Truck<MasterType,ParentType,InheritanceList>::_its_component_layer;

		implementation struct Antares_Tow_Truck_Depot : public Tow_Truck_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot)>
		{
			typedef typename Tow_Truck_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot)>::ComponentType ComponentType;
			
			typedef Link_Components::Implementations::Link_Line<MasterType> Link_Line;
			typedef Intersection_Prototype<typename type_of(MasterType::intersection)> Intersection_Interface;
			typedef Depot<typename type_of(MasterType::depot)> Depot_Interface;

#pragma pack(push,1)
			struct ITS_Location
			{
				void* object;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

#pragma pack(push,1)
			struct Truck_Location
			{
				void* object;
				Point_3D<MasterType> position;
			};
#pragma pack(pop)

			

			template<typename ComponentType,typename CallerType,typename TargetType>
			static void Depot_Condition(void* _this,Conditional_Response& response)
			{
				Tow_Truck_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot)>::Depot_Condition<ComponentType,CallerType,TargetType>(_this,response);

				response.next._iteration = _iteration + 60*5;
				response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_VISUALIZATION_SUB_ITERATIONS;

				response.result = true;

				//Swap_Event(&Depot_Event<ComponentType,CallerType,TargetType>);
			}
			
			template<typename ComponentType,typename CallerType,typename TargetType>
			static void Depot_Event(void* _this)
			{
				ComponentType* pthis = (ComponentType*)_this;

				const vector<polaris::LinkID>& truck_locations = pthis->_depot_service->getTruckLinks(_iteration);

				Truck_Location truck_location;

				pthis->_tow_trucks.clear();

				unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->template db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();
				
				typename type_of(MasterType::tow_truck) tow_truck;

				Tow_Truck_Interface* tow_truck_itf = (Tow_Truck_Interface*)&tow_truck;


				for(vector<polaris::LinkID>::const_iterator itr = truck_locations.begin();itr!=truck_locations.end();itr++)
				{
					if(db_map.count((*itr)))
					{
						Link_Interface* link = (Link_Interface*)db_map[(*itr)][0];

						tow_truck_itf->associated_depot<Depot_Interface*>( (Depot_Interface*)_this );
						tow_truck_itf->truck_location<Link_Interface*>( link );

						pthis->_tow_trucks.push_back(tow_truck);

						truck_location.object = &pthis->_tow_trucks.back();

						Intersection_Interface* upstream_intersection = link->upstream_intersection<Intersection_Interface*>();
						Intersection_Interface* downstream_intersection = link->downstream_intersection<Intersection_Interface*>();
						
						truck_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
						truck_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
						
						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( truck_location.position );

						type_of(MasterType::tow_truck)::_its_component_layer->Push_Element<Regular_Element>(&truck_location);
					}
				}
			}

			feature_implementation static void Initialize_Type()
			{
				Tow_Truck_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot)>::Initialize_Type<ComponentType,CallerType,NT>();

				Antares_Layer_Configuration cfg;

				_its_component_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(its_component_layer),ComponentType>*, string& > >(string("Tow Truck Depot"));

				cfg.Configure_Static_Points();
				cfg.head_texture = cfg.Add_Texture(string("HAR.png"));
				cfg.grouped=false;
				cfg.head_size_value=32;
				cfg.head_accent_size_value=64;
				cfg.selection_callback=&on_select;

				cfg.head_color._r = 255;
				cfg.head_color._g = 255;
				cfg.head_color._b = 255;
				cfg.head_color._a = 255;
				
				_its_component_layer->Initialize<NULLTYPE>(cfg);

				Tow_Truck_Interface::Initialize_Type<NT>();

			}

			feature_implementation void Initialize(TargetType configuration)
			{
				Tow_Truck_Depot<MasterType,NT,APPEND_CHILD(Antares_Tow_Truck_Depot)>::Initialize<ComponentType,CallerType,TargetType>(configuration);

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = _resident_link->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = _resident_link->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
				its_location.position._z = 5;
				
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );

				_its_component_layer->Push_Element<Regular_Element>(&its_location);
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
			}

			feature_implementation void Accent_Self()
			{

				ITS_Location its_location;

				its_location.object = (void*)((ComponentType*)this);

				Intersection_Interface* upstream_intersection = _resident_link->upstream_intersection<Intersection_Interface*>();
				Intersection_Interface* downstream_intersection = _resident_link->downstream_intersection<Intersection_Interface*>();
				
				its_location.position._x = (upstream_intersection->x_position<float>() + downstream_intersection->x_position<float>())/2.0f;
				its_location.position._y = (upstream_intersection->y_position<float>() + downstream_intersection->y_position<float>())/2.0f;
					
				Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( its_location.position );

				_its_component_layer->Push_Element<Accented_Element>(&its_location);


				Link_Line link_line;

				link_line.color._r = 255;
				link_line.color._g = 50;
				link_line.color._b = 50;
				link_line.color._a = 200;

				link_line.data = nullptr;

				for(vector<Link_Interface*>::iterator itr = _covered_links.begin(); itr != _covered_links.end(); itr++)
				{
					Link_Interface* link = (*itr);
					
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
			}

			static member_prototype(Antares_Layer,its_component_layer,typename type_of(MasterType::antares_layer),none,none);
			
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Tow_Truck_Depot<MasterType,ParentType,InheritanceList>::ComponentType>* Antares_Tow_Truck_Depot<MasterType,ParentType,InheritanceList>::_its_component_layer;


	}
}