//*********************************************************
//	Antares_Network_Event_Implementation.h - Graphical Network_Events
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Network_Event_Prototype.h"

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
		template<typename MasterType,typename ParentType=NULLTYPE,typename InheritanceList=NULLTYPELIST,template<class,class,class> class InheritanceTemplate=NULLTEMPLATE_3>
		struct Base_Antares_Network_Event : public InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Base_Antares_Network_Event)>
		{
			typedef typename InheritanceTemplate<MasterType,NT,APPEND_CHILD(Base_Antares_Network_Event)>::ComponentType ComponentType;
			
			static void on_select(const list<void*>& removed,const list<void*>& added,const list<void*>& selected,vector<pair<string,string>>& bucket)
			{
				if(removed.size())
				{
					_event_layer->Clear_Accented<NT>();

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

			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys,string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Base_Antares_Network_Event)>::Initialize_Type<ComponentType,CallerType,NT>(keys);

				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Lines();
				cfg.grouped=true;
				cfg.head_size_value=3;
				cfg.storage_period=1;
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

			feature_implementation void Accent_Self()
			{
				if(_active)
				{
					vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr;

					Link_Line_Segment* segments = new Link_Line_Segment[ _affected_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = _affected_links.size();
					group.segments = segments;

					Link_Line_Segment* current_segment = group.segments;

					for(itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
					{
						Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
						
						Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
						
						intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 2;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

						intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 2;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

						++current_segment;
					}
					
					_event_layer->Push_Element<Accented_Element>(&group);

					delete[] segments;
				}
			}

			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Base_Antares_Network_Event)>::Initialize<ComponentType,ComponentType,weak_ptr<polaris::io::Event_Instance>& >(instance);
			}
			
			declare_feature_conditional_implementation(Incident_Conditional)
			{
				ComponentType* pthis = (ComponentType*)_this;
				
				if( _iteration >= pthis->_end_time )
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else if(_iteration < pthis->_start_time)
				{
					response.next._iteration = pthis->_start_time;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = _iteration + 1;
					response.next._sub_iteration = 0;
				}

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				InheritanceTemplate<MasterType,ParentType,APPEND_CHILD(Base_Antares_Network_Event)>::Incident_Event<ComponentType,ComponentType,NT>(_this);

				if(pthis->_active)
				{
					vector<Link_Prototype<typename type_of(MasterType::link),ComponentType>*>::iterator itr;

					Link_Line_Segment* segments = new Link_Line_Segment[ pthis->_affected_links.size() ];
					
					Link_Line_Group group;
					group.num_primitives = pthis->_affected_links.size();
					group.segments = segments;
					group.object = pthis;

					Link_Line_Segment* current_segment = group.segments;

					for(itr = pthis->_affected_links.begin(); itr != pthis->_affected_links.end(); itr++)
					{
						Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
						
						Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
						
						intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
						
						current_segment->a._x = intersection->x_position<float>();
						current_segment->a._y = intersection->y_position<float>();
						current_segment->a._z = 2;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->a );

						intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

						current_segment->b._x = intersection->x_position<float>();
						current_segment->b._y = intersection->y_position<float>();
						current_segment->b._z = 2;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( current_segment->b );

						++current_segment;
					}
					
					_event_layer->Push_Element<Regular_Element>(&group);

					delete[] segments;
				}
			}

			static member_prototype(Antares_Layer,event_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList,template<class,class,class> class InheritanceTemplate>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Base_Antares_Network_Event<MasterType,ParentType,InheritanceList,InheritanceTemplate>::ComponentType>* Base_Antares_Network_Event<MasterType,ParentType,InheritanceList,InheritanceTemplate>::_event_layer;

		implementation struct Antares_Weather_Network_Event : public Base_Antares_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Weather_Network_Event),Weather_Network_Event>
		{
#pragma pack(push,1)
			struct Colored_Particle
			{
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> particle;
			};
#pragma pack(pop)

			declare_feature_event_implementation(Incident_Event)
			{
				Base_Antares_Network_Event::Incident_Event<ComponentType,ComponentType,NT>(_this);

				if( _particle_layer->draw<bool>() )
				{
					((ComponentType*)_this)->Update_Particles<ComponentType,ComponentType,NT>();
				}
			}
			
			feature_implementation void Update_Particles()
			{
				const int num_drop_vals = 5;

				int drops[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					drops[i] = rand()%100+75;
				}

				const int* const drops_begin = &drops[0];
				const int* const drops_end = drops_begin + num_drop_vals;

				const int* drops_iterator = drops_begin;

				for(vector<Colored_Particle>::iterator itr=_precipitation_particles.begin();itr!=_precipitation_particles.end();itr++,drops_iterator++)
				{
					if(drops_iterator == drops_end) drops_iterator=drops_begin;

					itr->particle._z -= *drops_iterator; 

					if(itr->particle._z < 0) itr->particle._z = 10000;

					_particle_layer->Push_Element<Regular_Element>( &(*itr) );
				}
			}

			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				Base_Antares_Network_Event::Initialize<ComponentType,ComponentType,weak_ptr<polaris::io::Event_Instance>& >(instance);

				Colored_Particle precipitation_particle;

				for(vector<Link_Interface*>::iterator itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
				{
					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
					
					Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* intersection;
					
					if( rand()%2 )
					{
						intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
						
						float saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 150*saturation;
						precipitation_particle.color._g = 200*saturation;
						precipitation_particle.color._b = 255*saturation;

						precipitation_particle.particle._x = intersection->x_position<float>();
						precipitation_particle.particle._y = intersection->y_position<float>();
						precipitation_particle.particle._z = rand()%10000;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( precipitation_particle.particle );

						_precipitation_particles.push_back(precipitation_particle);
					}

					if( rand()%2 )
					{
						intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

						float saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 150*saturation;
						precipitation_particle.color._g = 200*saturation;
						precipitation_particle.color._b = 255*saturation;

						precipitation_particle.particle._x = intersection->x_position<float>();
						precipitation_particle.particle._y = intersection->y_position<float>();
						precipitation_particle.particle._z = rand()%10000;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( precipitation_particle.particle );

						_precipitation_particles.push_back(precipitation_particle);
					}
				}

			}

			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Weather_Events"));

				_particle_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Weather_Event_Precipitation"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Points();
				cfg.head_size_value = 2;
				cfg.primitive_color = true;

				cfg.head_color._r = 100;
				cfg.head_color._g = 150;
				cfg.head_color._b = 255;
				cfg.head_color._a = 200;

				_particle_layer->Initialize<NULLTYPE>(cfg);
			}

			member_data(vector<Colored_Particle>,precipitation_particles,none,none);

			static member_prototype(Antares_Layer,particle_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Weather_Network_Event<MasterType,ParentType,InheritanceList>::ComponentType>* Antares_Weather_Network_Event<MasterType,ParentType,InheritanceList>::_particle_layer;

		
		implementation struct Antares_Accident_Network_Event : public Base_Antares_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Accident_Network_Event),Accident_Network_Event>
		{
#pragma pack(push,1)
			struct Colored_Particle
			{
				True_Color_RGBA<MasterType> color;
				Point_3D<MasterType> particle;
			};
#pragma pack(pop)

			declare_feature_event_implementation(Incident_Event)
			{
				Base_Antares_Network_Event::Incident_Event<ComponentType,ComponentType,NT>(_this);

				if( _particle_layer->draw<bool>() )
				{
					((ComponentType*)_this)->Update_Particles<ComponentType,ComponentType,NT>();
				}
			}
			
			feature_implementation void Update_Particles()
			{
				const int num_drop_vals = 5;

				int drops[num_drop_vals];

				for(int i=0;i<num_drop_vals;i++)
				{
					drops[i] = rand()%10+10;
				}

				const int* const drops_begin = &drops[0];
				const int* const drops_end = drops_begin + num_drop_vals;

				const int* drops_iterator = drops_begin;

				for(vector<Colored_Particle>::iterator itr=_precipitation_particles.begin();itr!=_precipitation_particles.end();itr++,drops_iterator++)
				{
					if(drops_iterator == drops_end) drops_iterator=drops_begin;

					itr->particle._z += *drops_iterator; 

					if(itr->particle._z > (1000 - (*drops_iterator)*10)) itr->particle._z = 0;

					_particle_layer->Push_Element<Regular_Element>( &(*itr) );
				}
			}

			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				Base_Antares_Network_Event::Initialize<ComponentType,ComponentType,weak_ptr<polaris::io::Event_Instance>& >(instance);

				Colored_Particle precipitation_particle;

				for(vector<Link_Interface*>::iterator itr = _affected_links.begin(); itr != _affected_links.end(); itr++)
				{
					Link_Prototype<typename type_of(MasterType::link),ComponentType>* link = (Link_Prototype<typename type_of(MasterType::link),ComponentType>*)(*itr);
					
					Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* upstream_intersection;
					Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* downstream_intersection;

					upstream_intersection = link->upstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();
					
					float saturation = (float)(rand()%50+50);
					saturation/=100.0f;

					precipitation_particle.color._r = 255*saturation;
					precipitation_particle.color._g = 175*saturation;
					precipitation_particle.color._b = 50*saturation;

					precipitation_particle.particle._x = upstream_intersection->x_position<float>();
					precipitation_particle.particle._y = upstream_intersection->y_position<float>();
					precipitation_particle.particle._z = rand()%1000;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( precipitation_particle.particle );

					_precipitation_particles.push_back(precipitation_particle);


					downstream_intersection = link->downstream_intersection< Intersection_Prototype<typename type_of(MasterType::intersection),ComponentType>* >();

					saturation = (float)(rand()%50+50);
					saturation/=100.0f;

					precipitation_particle.color._r = 255*saturation;
					precipitation_particle.color._g = 175*saturation;
					precipitation_particle.color._b = 50*saturation;

					precipitation_particle.particle._x = downstream_intersection->x_position<float>();
					precipitation_particle.particle._y = downstream_intersection->y_position<float>();
					precipitation_particle.particle._z = rand()%1000;

					Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( precipitation_particle.particle );

					_precipitation_particles.push_back(precipitation_particle);

					const float parts = 25.0f;

					for(int i=0;i<25;i++)
					{
						saturation = (float)(rand()%50+50);
						saturation/=100.0f;

						precipitation_particle.color._r = 255*saturation;
						precipitation_particle.color._g = 175*saturation;
						precipitation_particle.color._b = 50*saturation;

						precipitation_particle.particle._x = upstream_intersection->x_position<float>()*((float)i)/(parts)+downstream_intersection->x_position<float>()*(parts-(float)i)/(parts);
						precipitation_particle.particle._y = upstream_intersection->y_position<float>()*((float)i)/(parts)+downstream_intersection->y_position<float>()*(parts-(float)i)/(parts);
						precipitation_particle.particle._z = rand()%1000;

						Scale_Coordinates<typename MasterType::type_of(canvas),NT,Target_Type<NT,void,Point_3D<MasterType>&>>( precipitation_particle.particle );

						_precipitation_particles.push_back(precipitation_particle);
					}
				}
			}

			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Accident_Events"));

				_particle_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(string("Accident Fallout"));

				Antares_Layer_Configuration cfg;
				cfg.Configure_Points();
				cfg.head_size_value = 4;
				cfg.primitive_color = true;

				cfg.head_color._r = 255;
				cfg.head_color._g = 175;
				cfg.head_color._b = 50;
				cfg.head_color._a = 200;

				_particle_layer->Initialize<NULLTYPE>(cfg);
			}

			member_data(vector<Colored_Particle>,precipitation_particles,none,none);

			static member_prototype(Antares_Layer,particle_layer,typename type_of(MasterType::antares_layer),none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		Antares_Layer<typename type_of(MasterType::antares_layer),typename Antares_Accident_Network_Event<MasterType,ParentType,InheritanceList>::ComponentType>* Antares_Accident_Network_Event<MasterType,ParentType,InheritanceList>::_particle_layer;

		implementation struct Antares_Congestion_Network_Event : public Base_Antares_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Congestion_Network_Event),Congestion_Network_Event>
		{
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Congestion_Events"));
			}
		};

		implementation struct Antares_Lane_Closure_Network_Event : public Base_Antares_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Lane_Closure_Network_Event),Lane_Closure_Network_Event>
		{
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Lane_Closure_Events"));
			}
		};
	}

}

using namespace Network_Event_Components::Implementations;