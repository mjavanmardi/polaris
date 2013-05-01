#pragma once
#include "Network_Event_Prototype.h"
#include "Link_Prototype.h"

define_static_typelist_loop(Initialize_Type,Initialize_List);
//define_feature_dispatcher(Initialize,Initialize_Object);

namespace Network_Event_Components
{

	namespace Types
	{
		enum WEATHER_TYPE
		{
			CLEAR_DRY_PAVEMENT = 0,
			CLEAR_WET_PAVEMENT,
			RAIN,
			SNOW,
			TEMP,
			WIND,
			VISIBILITY
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Base_Network_Event : public Polaris_Component<APPEND_CHILD(Base_Network_Event),MasterType,Execution_Object,ParentType,true>
		{
			typedef Link_Components::Prototypes::Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;
			
			feature_implementation static void Initialize_Type(const vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				for(vector<shared_ptr<polaris::io::Event_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				{
					_event_keys.push_back( (*itr)->getKey() );
				}
			}
			
			feature_implementation static void Accept_Subscriber(TargetType callback,int subscriber)
			{
				_callbacks_by_component_index[subscriber]=callback;
			}

			feature_implementation void Start()
			{
				int start = _start_time;

				if(start == _iteration) ++start;

				if(start < _end_time)
				{
					Load_Event<ComponentType>(&ComponentType::Incident_Conditional<ComponentType,NT,NT>,&ComponentType::Incident_Event<ComponentType,NT,NT>, start, 0);
				}
				else
				{
					THROW_WARNING("Invalid Network Event");
				}
			}

			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;
				
				_active = false;
				
				_start_time = instance.lock()->getStart_Time();
				_end_time = instance.lock()->getEnd_Time();

				_notes = instance.lock()->getNote();

				_start_time = 80 + rand()%20;
				_end_time = 2000;

				const vector<int>& links=instance.lock()->getLinks();

				unordered_map<int,vector<typename MasterType::link_type*>>& db_map=((Network_Prototype<typename type_of(MasterType::network),ComponentType>*)_global_network)->db_id_to_links_map<unordered_map<int,vector<typename MasterType::link_type*>>&>();

				for(vector<int>::const_iterator itr=links.begin();itr!=links.end();itr++)
				{
					int link = *itr;

					if(db_map.count(link))
					{
						vector<typename MasterType::link_type*>& links=db_map[link];

						vector<typename type_of(MasterType::link)*>::iterator vitr;

						for(vitr=links.begin();vitr!=links.end();vitr++)
						{
							_affected_links.push_back( (Link_Interface*)(*vitr) );
						}
					}
				}
			}
			
			declare_feature_conditional_implementation(Incident_Conditional)
			{
				ComponentType* pthis = (ComponentType*)_this;

				if(_iteration >= pthis->_end_time)
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = pthis->_end_time;
					response.next._sub_iteration = 0;
				}

				response.result = true;
			}

			feature_implementation void Notify_Subscribers()
			{
				int subscriber = Link_Interface::Component_Type::component_index;

				if(_callbacks_by_component_index.count(subscriber))
				{
					typename Network_Event_Callback<ComponentType>::type callback=_callbacks_by_component_index[subscriber];

					for(vector<Link_Interface*>::iterator itr=_affected_links.begin();itr!=_affected_links.end();itr++)
					{
						(*callback)( (void*)(*itr), (Network_Event<ComponentType,NT>*)this );
					}
				}
			}

			declare_feature_event_implementation(Incident_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				if( _iteration >= pthis->_end_time || _iteration < pthis->_start_time )
				{
					pthis->_active = false;

					pthis->Notify_Subscribers<ComponentType,ComponentType,NT>();
				}
				else
				{
					pthis->_active = true;

					pthis->Notify_Subscribers<ComponentType,ComponentType,NT>();
				}
			}

			member_data(vector<Link_Interface*>,affected_links,none,none);
			member_data(int,start_time,none,none);
			member_data(int,end_time,none,none);
			member_data(bool,active,none,none);
			member_data(string,notes,none,none);

			static member_data(vector<string>,event_keys,none,none);
			static member_prototype(Network_Event_Manager,network_event_manager,typename type_of(MasterType::network_event_manager),none,none);
			
			static member_data(concat(hash_map<int,typename Network_Event_Callback<ComponentType>::type>),callbacks_by_component_index,none,none);
		};
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		vector<string> Base_Network_Event<MasterType,ParentType,InheritanceList>::_event_keys;
		
		template<typename MasterType,typename ParentType,typename InheritanceList>
		hash_map<int,typename Network_Event_Callback<typename Base_Network_Event<MasterType,ParentType,InheritanceList>::ComponentType>::type> Base_Network_Event<MasterType,ParentType,InheritanceList>::_callbacks_by_component_index;

		template<typename MasterType,typename ParentType,typename InheritanceList>
		Network_Event_Manager<typename type_of(MasterType::network_event_manager),typename Base_Network_Event<MasterType,ParentType,InheritanceList>::ComponentType>* Base_Network_Event<MasterType,ParentType,InheritanceList>::_network_event_manager;

		implementation struct Weather_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Weather_Network_Event)>
		{
			//feature_implementation static void Initialize_Type(void* obj){Base_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(obj);}

			feature_implementation void Start(){Base_Network_Event::Start<ComponentType,CallerType,NT>();}
			
			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				Base_Network_Event::Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				
				const vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					if( (*itr)->getKey()->getKey() == "type" )
					{
						if((*itr)->getValue() == "snow")
						{
							_weather_type = Types::WEATHER_TYPE::SNOW;
						}
					}
					else if( (*itr)->getKey()->getKey() == "snowdepthm" )
					{
						_precipitation_depth = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "vism" )
					{
						_visibility = stoi((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "county" )
					{
						_county = (*itr)->getValue();
					}
				}
			}

			member_data(Types::WEATHER_TYPE,weather_type,none,none);
			member_data(float,precipitation_depth,none,none);
			member_data(int,visibility,none,none);
			member_data(string,county,none,none);
			member_data(float,temperature,none,none);
			member_data(float,wind_speed,none,none);
		};
		
		implementation struct Accident_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Accident_Network_Event)>
		{
			//feature_implementation static void Initialize_Type(void* obj){Base_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(obj);}
			
			feature_implementation void Start(){Base_Network_Event::Start<ComponentType,CallerType,NT>();}
			
			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				Base_Network_Event::Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				
				const vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					if( (*itr)->getKey()->getKey() == "lanes" )
					{
						_lanes = stoi((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "severity" )
					{
						_severity = stoi((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "vehicles" )
					{
						_vehicles_involved = stoi((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "accident_type" )
					{
						_accident_type = (*itr)->getValue();
					}
					else if( (*itr)->getKey()->getKey() == "vehicle_type" )
					{
						_vehicle_type = (*itr)->getValue();
					}
					else if( (*itr)->getKey()->getKey() == "injury" )
					{
						_injury_type = (*itr)->getValue();
					}
				}
			}

			member_data(int,lanes,none,none);
			member_data(int,severity,none,none);
			member_data(string,accident_type,none,none);
			member_data(int,vehicles_involved,none,none);
			member_data(string,vehicle_type,none,none);
			member_data(string,injury_type,none,none);
		};

		implementation struct Congestion_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Congestion_Network_Event)>
		{
			//feature_implementation static void Initialize_Type(void* obj){Base_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(obj);}
			
			feature_implementation void Start(){Base_Network_Event::Start<ComponentType,CallerType,NT>();}
			
			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				Base_Network_Event::Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				
				const vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					if( (*itr)->getKey()->getKey() == "travel_time" )
					{
						_travel_time = stoi((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "slowdown_index" )
					{
						_slowdown_index = stof((*itr)->getValue());
					}
				}
			}

			member_data(int,travel_time,none,none);
			member_data(float,slowdown_index,none,none);
		};
		
		implementation struct Lane_Closure_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Lane_Closure_Network_Event)>
		{
			//feature_implementation static void Initialize_Type(void* obj){Base_Network_Event::Initialize_Type<ComponentType,CallerType,NT>(obj);}
			
			feature_implementation void Start(){Base_Network_Event::Start<ComponentType,CallerType,NT>();}
			
			feature_implementation void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				Base_Network_Event::Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				
				const vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					if( (*itr)->getKey()->getKey() == "lanes_closed" )
					{
						_lanes = stoi((*itr)->getValue());
					}
				}
			}

			member_data(int,lanes,none,none);
		};


		implementation struct Network_Event_Manager_Implementation : public Polaris_Component<APPEND_CHILD(Network_Event_Manager_Implementation),MasterType,Data_Object,ParentType>
		{
			typedef Network_Event<typename MasterType::type_of(base_network_event),ComponentType> Base_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(weather_network_event),ComponentType> Weather_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(accident_network_event),ComponentType> Accident_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(congestion_network_event),ComponentType> Congestion_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(lane_closure_network_event),ComponentType> Lane_Closure_Network_Event_Interface;
			
			feature_implementation void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback)
			{
				Network_Event<TargetType,ComponentType>::Push_Subscriber<typename Network_Event_Callback<TargetType>::type>(callback,CallerType::component_index);
			}

			feature_implementation void Initialize()
			{
				Read_DB<ComponentType,ComponentType,NT>();
				//execute_static_typelist_loop(Initialize_List,MasterType::network_event_types,this);
			}

			feature_implementation void Read_DB()
			{
				using namespace odb;
				using namespace polaris;

				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				string name( ((_Scenario_Interface*)_global_scenario)->template database_name<string&>());

				unique_ptr<database> db (open_sqlite_database (name));

				session s;

				transaction t (db->begin());
				result<io::Network_Event> r = db->query<io::Network_Event> (query<io::Network_Event>::true_expr);

				for (result<io::Network_Event>::iterator itr=r.begin(); itr!=r.end(); ++itr)
				{
					const string& name = itr->getName();

					if(name == "Weather")
					{
						Weather_Network_Event_Interface::Initialize_Type<const vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Accident")
					{
						Accident_Network_Event_Interface::Initialize_Type<const vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Congestion")
					{
						Congestion_Network_Event_Interface::Initialize_Type<const vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Lane Closure")
					{
						Lane_Closure_Network_Event_Interface::Initialize_Type<const vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					
					const vector<weak_ptr<io::Event_Instance> >& instances = itr->getInstances();

					for(vector<weak_ptr<io::Event_Instance>>::const_iterator vitr=instances.begin();vitr!=instances.end();vitr++)
					{
						weak_ptr<io::Event_Instance> _ptr=*vitr;

						if(name == "Weather")
						{
							Weather_Network_Event_Interface* net_event = (Weather_Network_Event_Interface*)Allocate<MasterType::type_of(weather_network_event)>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							Create_Network_Event<ComponentType,ComponentType,typename Weather_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Accident")
						{
							Accident_Network_Event_Interface* net_event = (Accident_Network_Event_Interface*)Allocate<MasterType::type_of(accident_network_event)>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							Create_Network_Event<ComponentType,ComponentType,typename Accident_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Congestion")
						{
							Congestion_Network_Event_Interface* net_event = (Congestion_Network_Event_Interface*)Allocate<MasterType::type_of(congestion_network_event)>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							Create_Network_Event<ComponentType,ComponentType,typename Congestion_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Lane Closure")
						{
							Lane_Closure_Network_Event_Interface* net_event = (Lane_Closure_Network_Event_Interface*)Allocate<MasterType::type_of(lane_closure_network_event)>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							Create_Network_Event<ComponentType,ComponentType,typename Lane_Closure_Network_Event_Interface::ComponentType>(net_event);
						}
					}
				}
			}

			feature_implementation void Get_Network_Events(int link_id,vector< Network_Event<TargetType,NT>* >& container/*,requires(check_2(TargetType,typename type_of(MasterType::weather_network_event),is_same) || check_2(CallerType,typename type_of(MasterType::traffic_management_center),is_same))*/)
			{
				list<Network_Event<typename TargetType,NT>*>* events_of_type = (list<Network_Event<typename TargetType,NT>*>*) & (_network_event_container[TargetType::component_index]);

				for(list< Network_Event<typename TargetType,NT>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
					Network_Event<typename TargetType,NT>* network_event=*itr;

					if(network_event->active<bool>())
					{
						vector<Link_Interface*>* affected_links = network_event->affected_links<vector<Link_Interface*>*>();

						for(vector<Link_Interface*>::iterator vitr = affected_links->begin();vitr != affected_links->end();vitr++)
						{
							if((*vitr)->internal_id<int>() == link_id)
							{
								container.push_back( *itr );
								break;
							}
						}
					}
				}
			}

			feature_implementation void Get_Network_Events( vector< Network_Event<TargetType,NT>* >& container, requires(!check_2(TargetType,typename type_of(MasterType::base_network_event),is_same)))
			{
				list<Network_Event<typename TargetType,NT>*>* events_of_type = (list<Network_Event<typename TargetType,NT>*>*) & (_network_event_container[TargetType::component_index]);

				for(list< Network_Event<typename TargetType,NT>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
					Network_Event<typename TargetType,NT>* network_event=*itr;

					if(network_event->active<bool>())
					{
						container.push_back( *itr );
					}
				}
			}
			
			feature_implementation void Get_Network_Events( vector< Network_Event<TargetType,CallerType>* >& container, requires(check_2(TargetType,typename type_of(MasterType::base_network_event),is_same)))
			{
				for(hash_map< int, list<Base_Network_Event_Interface*> >::iterator h_itr=_network_event_container.begin();h_itr!=_network_event_container.end();h_itr++)
				{
					list< Base_Network_Event_Interface* >* events_of_type=&h_itr->second;

					for(list< Base_Network_Event_Interface* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
					{
						Base_Network_Event_Interface* network_event=*itr;

						if(network_event->active<bool>())
						{
							container.push_back( (Network_Event<typename MasterType::type_of(base_network_event),CallerType>*) *itr );
						}
					}
				}
			}

			//feature_implementation void Get_Network_Events(int link_id,vector< Network_Event<TargetType,NT>* >& container,requires(!(check_2(TargetType,typename type_of(MasterType::weather_network_event),is_same) || check_2(CallerType,typename type_of(MasterType::traffic_management_center),is_same))))
			//{
			//	static_assert(false,"Non-TMC are only allowed to withdraw weather events!");
			//}

			feature_implementation void Create_Network_Event(Network_Event<TargetType,CallerType>* network_event)
			{
				network_event->Start<NT>();

				_network_event_container[TargetType::component_index].push_back( (Base_Network_Event_Interface*) network_event );
			}

			feature_implementation void Remove_Network_Event(Network_Event<TargetType,NT>* network_event)
			{
				list<Network_Event<typename TargetType::ControlType,NT>*>* events_of_type = (list<Network_Event<typename TargetType::ControlType,NT>*>*) & (_network_event_container[TargetType::ControlType::component_index]);

				for(list< Network_Event<typename TargetType::ControlType,NT>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
					if( (*itr) == network_event )
					{
						events_of_type->erase(itr);
					}
				}
			}
			
			typedef Link_Prototype<typename type_of(MasterType::link),ComponentType> Link_Interface;

			member_data( concat(hash_map< int, list<Base_Network_Event_Interface*> >), network_event_container, none ,none);
		};
	}

}