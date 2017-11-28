#pragma once
#include "Network_Event_Prototype.h"
#include "Link_Prototype.h"
#include "Zone_Prototype.h"

define_static_typelist_loop(Initialize_Type,Initialize_List);
//define_feature_dispatcher(Initialize,Initialize_Object);

namespace Network_Event_Components
{
	using std::shared_ptr;
	using std::weak_ptr;

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

		enum ACCIDENT_SEVERITY
		{
			SHOULDER_DISABLEMENT = 0,
			SHOULDER_ACCIDENT,
			ONE_LANE_BLOCKED,
			TWO_LANES_BLOCKED,
			THREE_LANES_BLOCKED
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Base_Network_Event : public Polaris_Component<MasterType,INHERIT(Base_Network_Event),Execution_Object>
		{
			typedef typename  Polaris_Component<MasterType,INHERIT(Base_Network_Event),Execution_Object>::Component_Type ComponentType;
			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> Link_Interface;
			typedef Zone_Components::Prototypes::Zone<typename MasterType::zone_type> Zone_Interface;
			typedef  Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename Link_Interface::get_type_of(activity_locations)::value_type>::type> Location_Interface;
			typedef Random_Access_Sequence<typename Link_Interface::get_type_of(activity_locations),Location_Interface*> Location_Container_Interface;

			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface2;
			typedef  Random_Access_Sequence< typename _Network_Interface::get_type_of(links_container), _Link_Interface2*> _Links_Container_Interface;


			template<typename TargetType> static void Initialize_Type(const std::vector<shared_ptr<polaris::io::Event_Key>>& keys)
			{
				for(typename std::vector<shared_ptr<polaris::io::Event_Key>>::const_iterator itr=keys.begin();itr!=keys.end();itr++)
				{
					_event_keys.push_back( (*itr)->getKey() );
				}
			}
			
			template<typename TargetType> static void Accept_Subscriber(TargetType callback,int subscriber)
			{
				_callbacks_by_component_id[subscriber]=callback;
			}

			template<typename TargetType> void Start()
			{
				int start = _start_time;

				if(start == iteration()) ++start;

				if(start < _end_time)
				{
					this->template Load_Event<ComponentType>(&ComponentType::Incident_Conditional,start, 0);
				}
				else
				{
					THROW_WARNING("Invalid Network Event");
				}
			}

			template<typename TargetType> void Initialize(int start_time, int end_time, std::vector<typename MasterType::link_type*>& affected_links)
			{
				_start_time = start_time;
				_end_time = end_time;

				for(typename std::vector<typename MasterType::link_type*>::iterator vitr=affected_links.begin();vitr!=affected_links.end();vitr++)
				{
					Link_Interface* link = (Link_Interface*)(*vitr);
					_affected_links.push_back( (Link_Interface*)(*vitr) );
					//Location_Container_Interface* locations = link->template activity_locations<Location_Container_Interface*>();

					//// push locations from link to affected locations container
					//for (typename Location_Container_Interface::iterator litr = locations->begin(); litr != locations->end(); ++litr)
					//{
					//	Location_Interface* loc = (*litr);
					//	this->_affected_locations.push_back(loc);
					//	zone_set.insert(loc->template zone<Zone_Interface*>());
					//}
				}
			}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;
				
				_active = false;
				
				_start_time = instance.lock()->getStart_Time();
				_end_time = instance.lock()->getEnd_Time();

				_notes = instance.lock()->getNote();

				const std::vector<int>& links=instance.lock()->getLinks();
				// temporary containers used to fill affected zone std::vector			
				std::unordered_set<Zone_Interface*> zone_set;
				if (links[0] == -1)
				{
					// affect all links
					_Links_Container_Interface& links_container = ((_Network_Interface*)_global_network)->template links_container<_Links_Container_Interface&>();
					typename _Links_Container_Interface::iterator link_itr;
					for (link_itr = links_container.begin(); link_itr != links_container.end(); link_itr++)
					{
						Link_Interface* link = (Link_Interface*)(*link_itr);
						_affected_links.push_back(link);
//cout << "link " << link->template internal_id<int>() << " affected" << endl;
						Location_Container_Interface* locations = link->template activity_locations<Location_Container_Interface*>();

						// push locations from link to affected locations container
						for (typename Location_Container_Interface::iterator litr = locations->begin(); litr != locations->end(); ++litr)
						{
							Location_Interface* loc = (Location_Interface*)(*litr);
								this->_affected_locations.push_back(loc);
								zone_set.insert(loc->template zone<Zone_Interface*>());
						}
					}
				}
				else
				{

					std::unordered_map<int,std::vector<typename MasterType::link_type*>>& db_map=((Network<typename MasterType::network_type>*)_global_network)->template db_id_to_links_map<std::unordered_map<int,std::vector<typename MasterType::link_type*>>&>();


				
					cout << endl << "INITIALIZE NETWORK EVENT:";
					for(typename std::vector<int>::const_iterator itr=links.begin();itr!=links.end();itr++)
					{
						int link = *itr;

						if(db_map.count(link))
						{
							std::vector<typename MasterType::link_type*>& links=db_map[link];

							typename std::vector<typename MasterType::link_type*>::iterator vitr;

							for(vitr=links.begin();vitr!=links.end();vitr++)
							{
								Link_Interface* link = (Link_Interface*)(*vitr);
								_affected_links.push_back( (Link_Interface*)(*vitr) );
								Location_Container_Interface* locations = link->template activity_locations<Location_Container_Interface*>();

								// push locations from link to affected locations container
								for (typename Location_Container_Interface::iterator litr = locations->begin(); litr != locations->end(); ++litr)
								{
									Location_Interface* loc = (Location_Interface*)(*litr);
									 this->_affected_locations.push_back(loc);
									 zone_set.insert(loc->template zone<Zone_Interface*>());
								}

							}
						}
					}
				}
			
				// create the affected zones std::list
				for (typename std::unordered_set<Zone_Interface*>::iterator zitr = zone_set.begin(); zitr != zone_set.end(); ++zitr)
				{
					Zone_Interface* zone = (Zone_Interface*)(*zitr);
					this->_affected_zones.push_back(zone);
					cout <<endl << "Affected zone: "<< zone->template uuid<int>();
				}

				// create the unaffected locations std::list
				Location_Container_Interface* all_locations = ((Network<typename MasterType::network_type>*)_global_network)->template activity_locations_container<Location_Container_Interface*>();
				for (typename Location_Container_Interface::iterator litr = all_locations->begin(); litr != all_locations->end(); ++litr)
				{
					bool add = true;
					for (typename std::vector<Location_Interface*>::iterator itr = this->_affected_locations.begin(); itr != this->_affected_locations.end(); ++itr)
					{
						if ((void*)*litr == (void*)*itr){add=false; break;}
					}
					if (add) this->_unaffected_locations.push_back((Location_Interface*)*litr);
				}
				cout << endl << "Total Locations: " << all_locations->size() << ", Affected Locations: " << this->_affected_locations.size() << ", Unaffected Locations: " << this->_unaffected_locations.size();
			}
			
			static void Incident_Conditional(ComponentType* _this,Event_Response& response)
			{
				ComponentType* pthis = (ComponentType*)_this;
				
				if( iteration() >= pthis->_end_time )
				{
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else if(iteration() < pthis->_start_time)
				{
					response.next._iteration = pthis->_start_time;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = iteration() + 1;
					response.next._sub_iteration = 0;
				}
				
				_this->Incident_Event();
			}

			template<typename TargetType> void Notify_Subscribers()
			{
				size_t subscriber = Link_Interface::Component_Type::component_id;

				if(_callbacks_by_component_id.count(subscriber))
				{
					typename Network_Event_Callback<ComponentType>::type callback=_callbacks_by_component_id[subscriber];

					for(typename std::vector<Link_Interface*>::iterator itr=_affected_links.begin();itr!=_affected_links.end();itr++)
					{
						(*callback)( (void*)(*itr), (Network_Event<ComponentType>*)this );
					}
				}
			}

			void Incident_Event()
			{
				if(iteration() == _start_time)
				{
					_active = true;
					Notify_Subscribers<NT>();
				}
				else if(iteration() == _end_time)
				{
					_active = false;
					Notify_Subscribers<NT>();
				}
			}

			m_data(std::vector<Link_Interface*>,affected_links, NONE, NONE);
			m_data(std::vector<Location_Interface*>,affected_locations, NONE, NONE);
			m_data(std::vector<Location_Interface*>,unaffected_locations, NONE, NONE);
			m_data(std::vector<Zone_Interface*>,affected_zones, NONE, NONE);

			m_data(int,start_time, NONE, NONE);
			m_data(int,end_time, NONE, NONE);
			m_data(bool,active, NONE, NONE);
			m_data(string,notes, NONE, NONE);

			m_static_data(std::vector<string>,event_keys, NONE, NONE);
			m_static_prototype(Null_Prototype,typename MasterType::network_event_manager_type,network_event_manager, NONE, NONE);
			
			m_static_data(concat(std::unordered_map<size_t,typename Network_Event_Callback<ComponentType>::type>),callbacks_by_component_id, NONE, NONE);
		};
		
		template<typename MasterType,typename InheritanceList>
		std::vector<string> Base_Network_Event<MasterType,InheritanceList>::_event_keys;
		
		template<typename MasterType,typename InheritanceList>
		std::unordered_map<size_t,typename Network_Event_Callback<typename Base_Network_Event<MasterType,InheritanceList>::ComponentType>::type> Base_Network_Event<MasterType,InheritanceList>::_callbacks_by_component_id;

		template<typename MasterType,typename InheritanceList>
		Null_Prototype<typename MasterType::network_event_manager_type>* Base_Network_Event<MasterType,InheritanceList>::_network_event_manager;

		implementation struct Weather_Network_Event : public Base_Network_Event<MasterType,INHERIT(Weather_Network_Event)>
		{
			//template<typename TargetType> static void Initialize_Type(void* obj){Base_Network_Event::template Initialize_Type<NT>(obj);}

			//template<typename TargetType> void Start(){Base_Network_Event::template Start<NT>();}
			template<typename TargetType> void Start(){((Base_Network_Event<MasterType,INHERIT(Weather_Network_Event)>*)this)->template Start<NT>();}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;
				//Base_Network_Event::template Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);

				((Base_Network_Event<MasterType,INHERIT(Weather_Network_Event)>*)this)-> Initialize<weak_ptr<Event_Instance>& >(instance);

				//_start_time = 14400;
				//_end_time = 43200;

				const std::vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(typename std::vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					//_weather_type = Types::WEATHER_TYPE::SNOW;
					//_precipitation_depth = .6;

					if( (*itr)->getKey()->getKey() == "type" )
					{
						if((*itr)->getValue() == "clear_dry_pavement")
						{
							_weather_type = Types::WEATHER_TYPE::CLEAR_DRY_PAVEMENT;
						}
						else if((*itr)->getValue() == "clear_wet_pavement")
						{
							_weather_type = Types::WEATHER_TYPE::CLEAR_WET_PAVEMENT;
						}
						else if((*itr)->getValue() == "rain")
						{
							_weather_type = Types::WEATHER_TYPE::RAIN;
						}
						else if((*itr)->getValue() == "snow")
						{
							_weather_type = Types::WEATHER_TYPE::SNOW;
						}
						else if((*itr)->getValue() == "temperature")
						{
							_weather_type = Types::WEATHER_TYPE::TEMP;
						}
						else if((*itr)->getValue() == "wind")
						{
							_weather_type = Types::WEATHER_TYPE::WIND;
						}
						else if ((*itr)->getValue() == "visibility")
						{
							_weather_type = Types::WEATHER_TYPE::VISIBILITY;
						}
					}
					else if( (*itr)->getKey()->getKey() == "snowdepthm" )
					{
						_snow_depth = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "precipm" )
					{
						_precipitation_depth = stof((*itr)->getValue());

					}
					else if( (*itr)->getKey()->getKey() == "vism" )
					{
						_visibility = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "wind" )
					{
						_wind_speed = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "temperature" )
					{
						_wind_speed = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "county" )
					{
						_county = (*itr)->getValue();
					}
				}
			}

			m_data(Types::WEATHER_TYPE,weather_type, NONE, NONE);
			m_data(float,precipitation_depth, NONE, NONE);
			m_data(float,snow_depth, NONE, NONE);
			m_data(float,visibility, NONE, NONE);
			m_data(string,county, NONE, NONE);
			m_data(float,temperature, NONE, NONE);
			m_data(float,wind_speed, NONE, NONE);
		};
		
		implementation struct Accident_Network_Event : public Base_Network_Event<MasterType,INHERIT(Accident_Network_Event)>
		{
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			//template<typename TargetType> static void Initialize_Type(void* obj){Base_Network_Event::template Initialize_Type<NT>(obj);}
			
			//template<typename TargetType> void Start(){Base_Network_Event::template Start<NT>();}
			template<typename TargetType> void Start(){((Base_Network_Event<MasterType,INHERIT(Accident_Network_Event)>*)this)->template Start<NT>();}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				//Base_Network_Event::template Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				((Base_Network_Event<MasterType,INHERIT(Accident_Network_Event)>*)this)->template Initialize<weak_ptr<Event_Instance>& >(instance);

				//_start_time = 28800 + rand()%(20*60);
				//_start_time = 500;
				this->_end_time = this->_end_time * ((_Scenario_Interface*)_global_scenario)->template accident_event_duration_reduction<float>();

				const std::vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(typename std::vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
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
					else if( (*itr)->getKey()->getKey() == "lanes_closed" )
					{
						_lanes_closed = stoi((*itr)->getValue());
					}
				}
			}

			m_data(int,lanes, NONE, NONE);
			m_data(int,lanes_closed, NONE, NONE);
			m_data(int,severity, NONE, NONE);
			m_data(string,accident_type, NONE, NONE);
			m_data(int,vehicles_involved, NONE, NONE);
			m_data(string,vehicle_type, NONE, NONE);
			m_data(string,injury_type, NONE, NONE);
		};

		implementation struct Congestion_Network_Event : public Base_Network_Event<MasterType,INHERIT(Congestion_Network_Event)>
		{
			//template<typename TargetType> static void Initialize_Type(void* obj){Base_Network_Event::template Initialize_Type<NT>(obj);}
			
			//template<typename TargetType> void Start(){Base_Network_Event::template Start<NT>();}
			template<typename TargetType> void Start(){((Base_Network_Event<MasterType,INHERIT(Congestion_Network_Event)>*)this)->template Start<NT>();}

			template<typename TargetType> void Initialize(int start_time, int end_time, std::vector<typename MasterType::link_type*>& affected_links)
			{
				((Base_Network_Event<MasterType,INHERIT(Congestion_Network_Event)>*)this)->template Initialize<MasterType>(start_time,end_time,affected_links);
			}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				//Base_Network_Event::template Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				((Base_Network_Event<MasterType,INHERIT(Congestion_Network_Event)>*)this)->template Initialize<weak_ptr<Event_Instance>& >(instance);
				
				const std::vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(typename std::vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
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

			m_data(int,travel_time, NONE, NONE);
			m_data(float,slowdown_index, NONE, NONE);
		};
		
		implementation struct Lane_Closure_Network_Event : public Base_Network_Event<MasterType,INHERIT(Lane_Closure_Network_Event)>
		{
			//template<typename TargetType> static void Initialize_Type(void* obj){Base_Network_Event::template Initialize_Type<NT>(obj);}
		
			//template<typename TargetType> void Start(){Base_Network_Event::template Start<NT>();}
			template<typename TargetType> void Start(){((Base_Network_Event<MasterType,INHERIT(Lane_Closure_Network_Event)>*)this)->template Start<NT>();}

			template<typename TargetType> void Initialize(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				//Base_Network_Event::template Initialize< ComponentType,ComponentType,weak_ptr<Event_Instance>& >(instance);
				((Base_Network_Event<MasterType,INHERIT(Lane_Closure_Network_Event)>*)this)->template Initialize<weak_ptr<Event_Instance>& >(instance);

				const std::vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();

				for(typename std::vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					if( (*itr)->getKey()->getKey() == "lanes_closed" )
					{
						_lanes = stoi((*itr)->getValue());
					}
				}
			}

			m_data(int,lanes, NONE, NONE);
		};


		implementation struct Network_Event_Manager_Implementation : public Polaris_Component<MasterType,INHERIT(Network_Event_Manager_Implementation),Data_Object>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Network_Event_Manager_Implementation),Data_Object>::Component_Type ComponentType;
			typedef Network_Event<typename MasterType::base_network_event_type> Base_Network_Event_Interface;
			typedef Network_Event<typename MasterType::weather_network_event_type> Weather_Network_Event_Interface;
			typedef Network_Event<typename MasterType::accident_network_event_type> Accident_Network_Event_Interface;
			typedef Network_Event<typename MasterType::congestion_network_event_type> Congestion_Network_Event_Interface;
			typedef Network_Event<typename MasterType::lane_closure_network_event_type> Lane_Closure_Network_Event_Interface;
			
			typedef Link<typename MasterType::link_type> Link_Interface;

			template<typename TargetType> void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback,int component_id)
			{
				//TODO
				Network_Event<TargetType>::template Push_Subscriber<typename Network_Event_Callback<TargetType>::type>(callback,component_id);
			}

			template<typename TargetType> void Initialize()
			{
				Read_DB<NT>();
				//execute_static_typelist_loop(Initialize_List,MasterType::network_event_types,this);
			}

			template<typename TargetType> void Read_DB()
			{
				using namespace odb;
				using namespace polaris;

				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				string name( ((_Scenario_Interface*)_global_scenario)->template database_name<string&>());

				unique_ptr<database> db (open_sqlite_database (name));

				session s;

				transaction t (db->begin());
				result<io::Network_Event> r = db->template query<io::Network_Event> (query<io::Network_Event>::true_expr);

				for (result<io::Network_Event>::iterator itr=r.begin(); itr!=r.end(); ++itr)
				{
					const string& name = itr->getName();

					if(name == "Weather")
					{
						Weather_Network_Event_Interface::template Initialize_Type<const std::vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Accident")
					{
						Accident_Network_Event_Interface::template Initialize_Type<const std::vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Congestion")
					{
						Congestion_Network_Event_Interface::template Initialize_Type<const std::vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					else if(name == "Lane Closure")
					{
						Lane_Closure_Network_Event_Interface::template Initialize_Type<const std::vector<shared_ptr<io::Event_Key>>&>(itr->getKeys());
					}
					
					const std::vector<weak_ptr<io::Event_Instance> >& instances = itr->getInstances();

					for(typename std::vector<weak_ptr<io::Event_Instance>>::const_iterator vitr=instances.begin();vitr!=instances.end();vitr++)
					{
						weak_ptr<io::Event_Instance> _ptr=*vitr;

						if(name == "Weather")
						{
							Weather_Network_Event_Interface* net_event = (Weather_Network_Event_Interface*)Allocate<typename MasterType::weather_network_event_type>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							net_event->template Start<NT>();
							Add_Network_Event<typename Weather_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Accident")
						{
							Accident_Network_Event_Interface* net_event = (Accident_Network_Event_Interface*)Allocate<typename MasterType::accident_network_event_type>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							net_event->template Start<NT>();
							Add_Network_Event<typename Accident_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Congestion")
						{
							Congestion_Network_Event_Interface* net_event = (Congestion_Network_Event_Interface*)Allocate<typename MasterType::congestion_network_event_type>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							net_event->template Start<NT>();
							Add_Network_Event<typename Congestion_Network_Event_Interface::ComponentType>(net_event);
						}
						else if(name == "Lane Closure")
						{
							Lane_Closure_Network_Event_Interface* net_event = (Lane_Closure_Network_Event_Interface*)Allocate<typename MasterType::lane_closure_network_event_type>();
							net_event->Initialize< weak_ptr<io::Event_Instance>& >(_ptr);
							net_event->template Start<NT>();
							Add_Network_Event<typename Lane_Closure_Network_Event_Interface::ComponentType>(net_event);
						}
					}
				}
			}

// TODO: does not compile
			template<typename TargetType> void Get_Network_Events(int link_id,boost::container::vector< Network_Event<TargetType>* >& container/*,requires(TargetType,check_2(TargetType,typename type_of(MasterType::weather_network_event),is_same) || check_2(typename type_of(MasterType::traffic_management_center),is_same))*/)
			{
                std::list<Network_Event<TargetType>*>* events_of_type = (std::list<Network_Event<TargetType>*>*) & (_network_event_container[TargetType::component_id]);

                for(typename std::list< Network_Event<TargetType>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
                    Network_Event<TargetType>* network_event=*itr;

					if(network_event->template active<bool>())
					{
						std::vector<Link_Interface*>* affected_links = network_event->template affected_links<vector<Link_Interface*>*>();

						for(typename std::vector<Link_Interface*>::iterator vitr = affected_links->begin();vitr != affected_links->end();vitr++)
						{
							if((*vitr)->template internal_id<int>() == link_id)
							{
								container.push_back( *itr );
								break;
							}
						}
					}
				}
			}

			template<typename TargetType> void Get_Network_Events( std::vector< Network_Event<TargetType>* >& container, requires(TargetType,!check_2(TargetType,typename MasterType::base_network_event_type,is_same)))
			{
				std::list<Network_Event<TargetType>*>* events_of_type = (std::list<Network_Event<TargetType>*>*) & (_network_event_container[TargetType::component_id]);

				for(typename std::list< Network_Event<TargetType>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
					Network_Event<TargetType>* network_event=*itr;

					if(network_event->template active<bool>())
					{
						container.push_back( *itr );
					}
				}
			}
			
			template<typename TargetType> void Get_Network_Events( std::vector< Network_Event<TargetType>* >& container, requires(TargetType,check_2(TargetType,typename MasterType::base_network_event_type,is_same)))
			{
				for(typename std::unordered_map< size_t, std::list<Base_Network_Event_Interface*> >::iterator h_itr=_network_event_container.begin();h_itr!=_network_event_container.end();h_itr++)
				{
					std::list< Base_Network_Event_Interface* >* events_of_type=&h_itr->second;

					for(typename std::list< Base_Network_Event_Interface* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
					{
						Base_Network_Event_Interface* network_event=*itr;

						if(network_event->template active<bool>())
						{
							container.push_back( (Network_Event<typename MasterType::base_network_event_type>*) *itr );
						}
					}
				}
			}

			//template<typename TargetType> void Get_Network_Events(int link_id,std::vector< Network_Event<TargetType,NT>* >& container,requires(TargetType,!(check_2(TargetType,typename type_of(MasterType::weather_network_event),is_same) || check_2(typename type_of(MasterType::traffic_management_center),is_same))))
			//{
			//	static_assert(false,"Non-TMC are only allowed to withdraw weather events!");
			//}

			template<typename TargetType> void Add_Network_Event(Network_Event<TargetType>* network_event)
			{
				_network_event_container[TargetType::component_id].push_back( (Base_Network_Event_Interface*) network_event );
			}

// TODO: does not compile
			template<typename TargetType> void Remove_Network_Event(Network_Event<TargetType>* network_event)
			{
                std::list<Network_Event<typename TargetType::ControlType>*>* events_of_type = (std::list<Network_Event<typename TargetType::ControlType>*>*) & (_network_event_container[TargetType::ControlType::component_id]);

                for(typename std::list< Network_Event<typename TargetType::ControlType>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
				{
					if( (*itr) == network_event )
					{
						events_of_type->erase(itr);
					}
				}
			}

			m_data( concat(std::unordered_map< size_t, std::list<Base_Network_Event_Interface*> >), network_event_container, NONE , NONE);
		};
	}

}

