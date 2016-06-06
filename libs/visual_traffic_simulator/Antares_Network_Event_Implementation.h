//*********************************************************
//	Antares_Network_Event_Implementation.h - Graphical Network_Events
//*********************************************************

#pragma once
#include "Dependencies.h"
#include "Traffic_Simulator\Network_Event_Implementations.h"

namespace Network_Event_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{

	}
	
	namespace Implementations
	{
		template<typename MasterType,typename InheritanceList=NULLTYPELIST,template<class,class> class InheritanceTemplate=NULLTEMPLATE_2>
		struct Base_Antares_Network_Event : public InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>
		{
			typedef typename InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>::ComponentType ComponentType;
			
			static void on_select(const std::list<void*>& removed,const std::list<void*>& added,const std::list<void*>& selected,std::vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_event_layer->Clear_Accented<NT>();

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
				
				key_value_pair.first="Event";

				key_value_pair.second=_notes;
				
				bucket.push_back(key_value_pair);
			}


			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys,string& name)
			{
				InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>::Initialize_Type<NT>(keys);

				_event_layer=Allocate_New_Layer<MT>(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Lines();
				cfg.grouped=true;
				cfg.head_size_value=3;
				cfg.storage_period=1;
				cfg.head_accent_size_value=6;
				cfg.target_sub_iteration = 0;
				cfg.selection_callback=&on_select;

				cfg.head_color._r = 255;
				cfg.head_color._g = 0;
				cfg.head_color._b = 0;
				cfg.head_color._a = 255;

				_event_layer->Initialize<NULLTYPE>(cfg);
			}

#pragma pack(push,1)
			struct Link_Line_Segment
			{
				Point_3D<MasterType> a;
				Point_3D<MasterType> b;
			};
#pragma pack(pop)

#pragma pack(push,1)
			struct Link_Line_Group
			{
				void* object;
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)

			template<typename TargetType> void Accent_Self()
			{
				if(_active)
				{
					std::vector<Link<typename MasterType::link_type>*>::iterator itr;

					Link_Line_Segment* segments = new Link_Line_Segment[ _affected_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = _affected_links.size();
					group.segments = segments;

					Link_Line_Segment* current_segment = group.segments;

					for(itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
					{
						Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
						
						Intersection<typename MasterType::intersection_type>* intersection;
						
						intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 2;

						Scale_Coordinates<MT>( current_segment->a );

						intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 2;

						Scale_Coordinates<MT>( current_segment->b );

						++current_segment;
					}
					
					_event_layer->Push_Element<Accented_Element>(&group);

					delete[] segments;
				}
			}
			
			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>::Initialize<weak_ptr<polaris::io::Event_Instance>&>(instance);
			}

			template<typename TargetType> void Initialize(int start_time, int end_time, std::vector<typename MasterType::link_type*>& affected_links)
			{
				InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>::Initialize<MasterType::link_type*>(start_time,end_time,affected_links);
			}
			
			//template<typename TargetType> void Incident_Conditional()
			//{
			//	ComponentType* pthis = (ComponentType*)_this;
			//	
			//	if( iteration() >= pthis->_end_time )
			//	{
			//		response.next._iteration = END;
			//		response.next._sub_iteration = 0;
			//	}
			//	else if(iteration() < pthis->_start_time)
			//	{
			//		response.next._iteration = pthis->_start_time;
			//		response.next._sub_iteration = 0;
			//	}
			//	else
			//	{
			//		response.next._iteration = _iteration + 1;
			//		response.next._sub_iteration = 0;
			//	}

			//	response.result = true;
			//}

			//declare_event(Incident_Event)
			void Incident_Event()
			{
				ComponentType* pthis = (ComponentType*)this;

				InheritanceTemplate<MasterType,INHERIT(Base_Antares_Network_Event)>::Incident_Event();

				if(pthis->_active)
				{
					std::vector<Link<typename MasterType::link_type>*>::iterator itr;

					Link_Line_Segment* segments = new Link_Line_Segment[ pthis->_affected_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = pthis->_affected_links.size();
					group.segments = segments;
					group.object = pthis;

					Link_Line_Segment* current_segment = group.segments;

					for(itr = pthis->_affected_links.begin(); itr != pthis->_affected_links.end(); itr++)
					{
						Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
						
						Intersection<typename MasterType::intersection_type>* intersection;
						
						intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 2;

						Scale_Coordinates<MT>( current_segment->a );

						intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 2;

						Scale_Coordinates<MT>( current_segment->b );

						++current_segment;
					}
					
					_event_layer->Push_Element<Regular_Element>(&group);

					delete[] segments;
				}
			}

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,event_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList,template<class,class> class InheritanceTemplate>
		Antares_Layer<typename MasterType::antares_layer_type>* Base_Antares_Network_Event<MasterType,InheritanceList,InheritanceTemplate>::_event_layer;

		implementation struct Antares_Weather_Network_Event : public Base_Antares_Network_Event<MasterType,INHERIT(Antares_Weather_Network_Event),Weather_Network_Event>
		{
#pragma pack(push,1)
			struct Colored_Particle
			{
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> particle;
			};
#pragma pack(pop)

			//declare_event(Incident_Event)
			void Incident_Event()
			{
				Base_Antares_Network_Event::Incident_Event();

				if( _particle_layer->draw<bool>() )
				{
					Update_Particles<NT>();
				}
			}
			
			template<typename TargetType> void Update_Particles()
			{
				const int num_drop_vals = 5;

				int drops[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					drops[i] = rand()%50+25;
				}
				
				int x_delta[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					x_delta[i] = rand()%30 - 15;
				}

				int y_delta[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					y_delta[i] = rand()%30 - 15;
				}

				const int* const drops_begin = &drops[0];
				const int* const drops_end = drops_begin + num_drop_vals;

				const int* drops_iterator = drops_begin;
				
				const int* const x_begin = &x_delta[0];
				const int* const x_end = x_begin + num_drop_vals;

				const int* x_iterator = x_begin;
				
				const int* const y_begin = &y_delta[0];
				const int* const y_end = y_begin + num_drop_vals;

				const int* y_iterator = y_begin;

				for(std::vector<pair<Point_3D<MasterType>,Colored_Particle>>::iterator itr=_precipitation_particles.begin();itr!=_precipitation_particles.end();itr++,drops_iterator++,x_iterator++,y_iterator++)
				{
					if(drops_iterator == drops_end) drops_iterator=drops_begin;
					if(x_iterator == x_end) x_iterator=x_begin;
					if(y_iterator == y_end) y_iterator=y_begin;

					itr->second.particle._z -= *drops_iterator; 

					itr->second.particle._x -= *x_iterator;
					itr->second.particle._y -= *y_iterator;

					if(itr->second.particle._z < 0)
					{
						//itr->second.particle._z = (5000 - (*drops_iterator)*10);
						itr->second.particle._z = (5000 - (*drops_iterator)*2);
						itr->second.particle._x = itr->first._x;
						itr->second.particle._y = itr->first._y;
					}

					_particle_layer->Push_Element<Regular_Element>( &(itr->second) );
				}
			}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				Base_Antares_Network_Event::Initialize<weak_ptr<polaris::io::Event_Instance>& >(instance);

				Colored_Particle precipitation_particle;

				for(std::vector<Link_Interface*>::iterator itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
				{
					Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
					
					Intersection<typename MasterType::intersection_type>* intersection;
					
					if( rand()%4 )
					{
						intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
						
						float saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 150*saturation;
						precipitation_particle.color._g = 200*saturation;
						precipitation_particle.color._b = 255*saturation;

						precipitation_particle.particle._x = intersection->x_position<float>();
						precipitation_particle.particle._y = intersection->y_position<float>();
						precipitation_particle.particle._z = rand()%5000;

						Scale_Coordinates<MT>( precipitation_particle.particle );

						_precipitation_particles.push_back(pair<Point_3D<MasterType>,Colored_Particle>(Point_3D<MasterType>(),precipitation_particle));

						_precipitation_particles.back().first._x = precipitation_particle.particle._x;
						_precipitation_particles.back().first._y = precipitation_particle.particle._y;

//TODO
//						//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( _precipitation_particles.back().first );
					}

					if( rand()%4 )
					{
						intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

						float saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 150*saturation;
						precipitation_particle.color._g = 200*saturation;
						precipitation_particle.color._b = 255*saturation;

						precipitation_particle.particle._x = intersection->x_position<float>();
						precipitation_particle.particle._y = intersection->y_position<float>();
						precipitation_particle.particle._z = rand()%5000;

						Scale_Coordinates<MT>( precipitation_particle.particle );

						_precipitation_particles.push_back(pair<Point_3D<MasterType>,Colored_Particle>(Point_3D<MasterType>(),precipitation_particle));

						_precipitation_particles.back().first._x = precipitation_particle.particle._x;
						_precipitation_particles.back().first._y = precipitation_particle.particle._y;

//TODO
//						//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( _precipitation_particles.back().first );
					}
				}

			}

			
			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*12.0f+2.0f);
				return size;
			}

			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<NT>(keys,string("Weather_Events"));

				_particle_layer=Allocate_New_Layer<MT>(string("Weather_Event_Precipitation"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Points();
				cfg.head_size_value = 4;
				cfg.primitive_color = true;
				cfg.target_sub_iteration = 0;
				cfg.pixel_size_callback = &pixel_size_callback;

				cfg.head_color._r = 100;
				cfg.head_color._g = 150;
				cfg.head_color._b = 255;
				cfg.head_color._a = 200;

				_particle_layer->Initialize<NULLTYPE>(cfg);
			}

			m_data(concat(std::vector<pair<Point_3D<MasterType>,Colored_Particle>>),precipitation_particles, NONE, NONE);

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,particle_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Weather_Network_Event<MasterType,InheritanceList>::_particle_layer;

		
		implementation struct Antares_Accident_Network_Event : public Base_Antares_Network_Event<MasterType,INHERIT(Antares_Accident_Network_Event),Accident_Network_Event>
		{
#pragma pack(push,1)
			struct Colored_Particle
			{
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> particle;
			};
#pragma pack(pop)

			//declare_event(Incident_Event)
			void Incident_Event()
			{
				Base_Antares_Network_Event::Incident_Event();

				if( _particle_layer->draw<bool>() )
				{
					Update_Particles<NT>();
				}
			}
			
			template<typename TargetType> void Update_Particles()
			{
				//const int num_drop_vals = 5;

				//int drops[num_drop_vals];

				//for(int i=0;i<num_drop_vals;i++)
				//{
				//	drops[i] = rand()%5+5;
				//}

				//const int* const drops_begin = &drops[0];
				//const int* const drops_end = drops_begin + num_drop_vals;

				//const int* drops_iterator = drops_begin;

				//for(std::vector<Colored_Particle>::iterator itr=_precipitation_particles.begin();itr!=_precipitation_particles.end();itr++,drops_iterator++)
				//{
				//	if(drops_iterator == drops_end) drops_iterator=drops_begin;

				//	itr->particle._z += *drops_iterator; 

				//	if(itr->particle._z > (1000 - (*drops_iterator)*10)) itr->particle._z = 0;

				//	_particle_layer->Push_Element<Regular_Element>( &(*itr) );
				//}



				const int num_drop_vals = 5;

				int drops[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					drops[i] = rand()%5+5;
				}
				
				int x_delta[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					x_delta[i] = rand()%8 - 4;
				}

				int y_delta[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					y_delta[i] = rand()%8 - 4;
				}

				const int* const drops_begin = &drops[0];
				const int* const drops_end = drops_begin + num_drop_vals;

				const int* drops_iterator = drops_begin;
				
				const int* const x_begin = &x_delta[0];
				const int* const x_end = x_begin + num_drop_vals;

				const int* x_iterator = x_begin;
				
				const int* const y_begin = &y_delta[0];
				const int* const y_end = y_begin + num_drop_vals;

				const int* y_iterator = y_begin;

				for(std::vector<pair<Point_3D<MasterType>,Colored_Particle>>::iterator itr=_precipitation_particles.begin();itr!=_precipitation_particles.end();itr++,drops_iterator++,x_iterator++,y_iterator++)
				{
					if(drops_iterator == drops_end) drops_iterator=drops_begin;
					if(x_iterator == x_end) x_iterator=x_begin;
					if(y_iterator == y_end) y_iterator=y_begin;

					itr->second.particle._z += *drops_iterator; 

					itr->second.particle._x -= *x_iterator;
					itr->second.particle._y -= *y_iterator;

					if(itr->second.particle._z > (1000 - (*drops_iterator)*2))
					{
						itr->second.particle._z = 0;
						itr->second.particle._x = itr->first._x;
						itr->second.particle._y = itr->first._y;
					}

					_particle_layer->Push_Element<Regular_Element>( &(itr->second) );
				}
			}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				Base_Antares_Network_Event::Initialize<weak_ptr<polaris::io::Event_Instance>& >(instance);

				Colored_Particle precipitation_particle;

				for(std::vector<Link_Interface*>::iterator itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
				{
					Link<typename MasterType::link_type>* link = (Link<typename MasterType::link_type>*)(*itr);
					
					Intersection<typename MasterType::intersection_type>* upstream_intersection;
					Intersection<typename MasterType::intersection_type>* downstream_intersection;

					upstream_intersection = link->upstream_intersection< Intersection<typename MasterType::intersection_type>* >();
					
					float saturation = (float)(rand()%50+50);
					saturation/=100.0f;

					precipitation_particle.color._r = 255*saturation;
					precipitation_particle.color._g = 175*saturation;
					precipitation_particle.color._b = 50*saturation;

					precipitation_particle.particle._x = upstream_intersection->x_position<float>();
					precipitation_particle.particle._y = upstream_intersection->y_position<float>();
					precipitation_particle.particle._z = rand()%1000;

					Scale_Coordinates<MT>( precipitation_particle.particle );

					//_precipitation_particles.push_back(precipitation_particle);

					_precipitation_particles.push_back(pair<Point_3D<MasterType>,Colored_Particle>(Point_3D<MasterType>(),precipitation_particle));

					_precipitation_particles.back().first._x = precipitation_particle.particle._x;
					_precipitation_particles.back().first._y = precipitation_particle.particle._y;

//TODO
//					//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( _precipitation_particles.back().first );


					downstream_intersection = link->downstream_intersection< Intersection<typename MasterType::intersection_type>* >();

					saturation = (float)(rand()%50+50);
					saturation/=100.0f;

					precipitation_particle.color._r = 255*saturation;
					precipitation_particle.color._g = 175*saturation;
					precipitation_particle.color._b = 50*saturation;

					precipitation_particle.particle._x = downstream_intersection->x_position<float>();
					precipitation_particle.particle._y = downstream_intersection->y_position<float>();
					precipitation_particle.particle._z = rand()%1000;

					Scale_Coordinates<MT>( precipitation_particle.particle );

					//_precipitation_particles.push_back(precipitation_particle);

					_precipitation_particles.push_back(pair<Point_3D<MasterType>,Colored_Particle>(Point_3D<MasterType>(),precipitation_particle));

					_precipitation_particles.back().first._x = precipitation_particle.particle._x;
					_precipitation_particles.back().first._y = precipitation_particle.particle._y;

//TODO
//					//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( _precipitation_particles.back().first );


					const float parts = 100.0f;

					for(int i=0;i<100;i++)
					{
						saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 255*saturation;
						precipitation_particle.color._g = 175*saturation;
						precipitation_particle.color._b = 50*saturation;

						precipitation_particle.particle._x = upstream_intersection->x_position<float>()*((float)i)/(parts)+downstream_intersection->x_position<float>()*(parts-(float)i)/(parts);
						precipitation_particle.particle._y = upstream_intersection->y_position<float>()*((float)i)/(parts)+downstream_intersection->y_position<float>()*(parts-(float)i)/(parts);
						precipitation_particle.particle._z = rand()%1000;

						Scale_Coordinates<MT>( precipitation_particle.particle );

						//_precipitation_particles.push_back(precipitation_particle);

						_precipitation_particles.push_back(pair<Point_3D<MasterType>,Colored_Particle>(Point_3D<MasterType>(),precipitation_particle));

						_precipitation_particles.back().first._x = precipitation_particle.particle._x;
						_precipitation_particles.back().first._y = precipitation_particle.particle._y;

//TODO
//						//Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( _precipitation_particles.back().first );
					}
				}
			}

			static float pixel_size_callback(float meter_per_pixel_mid)
			{
				float size = floor((1.57777/meter_per_pixel_mid)*16.0f+4.0f);
				return size;
			}

			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<NT>(keys,string("Accident_Events"));

				_particle_layer=Allocate_New_Layer<MT>(string("Accident Smoke"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Points();
				cfg.head_size_value = 4;
				cfg.primitive_color = true;
				cfg.target_sub_iteration = 0;
				cfg.pixel_size_callback = &pixel_size_callback;

				cfg.head_color._r = 255;
				cfg.head_color._g = 175;
				cfg.head_color._b = 50;
				cfg.head_color._a = 200;

				_particle_layer->Initialize<NULLTYPE>(cfg);
			}

			m_data(concat(std::vector<pair<Point_3D<MasterType>,Colored_Particle>>),precipitation_particles, NONE, NONE);

			static m_prototype(Antares_Layer,typename MasterType::antares_layer_type,particle_layer, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList>
		Antares_Layer<typename MasterType::antares_layer_type>* Antares_Accident_Network_Event<MasterType,InheritanceList>::_particle_layer;

		implementation struct Antares_Congestion_Network_Event : public Base_Antares_Network_Event<MasterType,INHERIT(Antares_Congestion_Network_Event),Congestion_Network_Event>
		{
			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<NT>(keys,string("Congestion_Events"));
			}
		};

		implementation struct Antares_Lane_Closure_Network_Event : public Base_Antares_Network_Event<MasterType,INHERIT(Antares_Lane_Closure_Network_Event),Lane_Closure_Network_Event>
		{
			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<NT>(keys,string("Lane_Closure_Events"));
			}
		};
	}

}

using namespace Network_Event_Components::Implementations;
