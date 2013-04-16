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

			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys,string& name)
			{
				InheritanceTemplate<MasterType,NT,APPEND_CHILD(Base_Antares_Network_Event)>::Initialize_Type<ComponentType,CallerType,NT>(keys);

				_event_layer=Allocate_New_Layer< typename MasterType::type_of(canvas),NT,Target_Type< NT,Antares_Layer<type_of(event_layer),ComponentType>*, string& > >(name);

				Antares_Layer_Configuration cfg;
				cfg.Configure_Dynamic_Lines();
				cfg.grouped=true;
				//cfg.group_color=true;
				cfg.head_size_value=4;
				cfg.storage_period=1;
				
				for(vector<string>::iterator itr=ComponentType::_event_keys.begin();itr!=ComponentType::_event_keys.end();itr++)
				{
					cfg.attributes_schema.push_back(*itr);
				}

				cfg.head_color._r = 0;
				cfg.head_color._g = 0;
				cfg.head_color._b = 255;
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
				int num_primitives;
				Link_Line_Segment* segments;
			};
#pragma pack(pop)

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
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Weather_Events"));
			}
		};
		
		implementation struct Antares_Accident_Network_Event : public Base_Antares_Network_Event<MasterType,NT,APPEND_CHILD(Antares_Accident_Network_Event),Accident_Network_Event>
		{
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				Base_Antares_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(keys,string("Accident_Events"));
			}
		};

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