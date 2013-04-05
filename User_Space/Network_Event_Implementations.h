#pragma once
#include "Network_Event_Prototype.h"

define_static_typelist_loop(Initialize_Type,Initialize_List);
//define_feature_dispatcher(Initialize,Initialize_Object);

namespace Network_Event_Components
{
	namespace Types
	{
		enum WEATHER_TYPE
		{
			SNOW
		};
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Base_Network_Event : public Polaris_Component<APPEND_CHILD(Base_Network_Event),MasterType,Execution_Object,ParentType,true>
		{

			//feature_implementation void Initialize()
			//{
			//	dispatch_to_feature(Initialize_Object,MasterType::network_event_types,this,NT);
			//}
			
			feature_implementation void Initialize()
			{
				Load_Event<ComponentType>(&ComponentType::Incident_Conditional<ComponentType,NT,NT>,&ComponentType::Incident_Event<ComponentType,NT,NT>, _iteration + 1, Scenario_Components::Types::END_OF_ITERATION - 1);
			}

			feature_implementation void Setup(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				_start_time = instance.lock()->getStart_Time();
				_end_time = instance.lock()->getEnd_Time();

				const vector<int>& links=instance.lock()->getLinks();

				for(vector<int>::const_iterator itr=links.begin();itr!=links.end();itr++)
				{
					_affected_links.push_back(*itr);
				}
			}

			member_data(vector<int>,affected_links,none,none);
			//member_data(int,time_detected,none,none);
			//member_data(int,expected_duration,none,none);
			member_data(int,start_time,none,none);
			member_data(int,end_time,none,none);
			//member_data(bool,expired,none,none);
			member_data(bool,active,none,none);
		};

		implementation struct Weather_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Weather_Network_Event)>
		{
			feature_implementation static void Initialize_Type(){}

			feature_implementation void Initialize()
			{
				Base_Network_Event::Initialize();
			}
			
			feature_implementation void Setup(weak_ptr<polaris::io::Event_Instance>& instance)
			{
				using namespace polaris::io;

				Base_Network_Event::Setup< Base_Network_Event,Weather_Network_Event,weak_ptr<Event_Instance>& >(instance);
				
				const vector<shared_ptr<Event_Instance_Value>>& values=instance.lock()->getValues();
				
				cout << "reading values" << endl;
				
				for(vector<shared_ptr<Event_Instance_Value>>::const_iterator itr=values.begin();itr!=values.end();itr++)
				{
					

					if( (*itr)->getKey()->getKey() == "type" )
					{
						if((*itr)->getValue() == "snow")
						{
							cout << "snow!" << endl;

							_weather_type = Types::WEATHER_TYPE::SNOW;
						}
					}
					else if( (*itr)->getKey()->getKey() == "snowdepthm" )
					{
						cout << "deep!" << endl;

						_precipitation_depth = stof((*itr)->getValue());
					}
					else if( (*itr)->getKey()->getKey() == "vism" )
					{
						_visibility = stoi((*itr)->getValue());
					}
				}
			}

			declare_feature_conditional_implementation(Incident_Conditional)
			{
				response.next._iteration =_iteration + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION - 1;

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Event)
			{


			}

			member_data(Types::WEATHER_TYPE,weather_type,none,none);
			//member_data(int,precipitation_rate,none,none);
			member_data(float,precipitation_depth,none,none);
			member_data(int,visibility,none,none);
			//member_data(int,temperature,none,none);
		};
		
		implementation struct Accident_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Accident_Network_Event)>
		{
			feature_implementation static void Initialize_Type(){}
			
			feature_implementation void Initialize()
			{
				Base_Network_Event::Initialize();
			}
			
			//feature_implementation void Setup(odb::result<Network_Event>::iterator& itr)
			//{
			//	Base_Network_Event::Setup<Base_Network_Event,Weather_Network_Event,odb::result<Network_Event>::iterator&>(itr);

			//}

			declare_feature_conditional_implementation(Incident_Conditional)
			{
				response.next._iteration =_iteration + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION - 1;

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Event)
			{


			}

			member_data(int,lanes,none,none);
			member_data(int,severity,none,none);
			member_data(int,accident_type,none,none);
			member_data(int,vehicles_involved,none,none);
			member_data(int,vehicle_type,none,none);
			member_data(int,injury_type,none,none);
		};

		implementation struct Congestion_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Congestion_Network_Event)>
		{
			feature_implementation static void Initialize_Type(){}
			
			feature_implementation void Initialize()
			{
				Base_Network_Event::Initialize();
			}
			
			//feature_implementation void Setup(odb::result<Network_Event>::iterator& itr)
			//{
			//	Base_Network_Event::Setup<Base_Network_Event,Weather_Network_Event,odb::result<Network_Event>::iterator&>(itr);

			//}

			declare_feature_conditional_implementation(Incident_Conditional)
			{
				response.next._iteration =_iteration + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION - 1;

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Event)
			{


			}

			member_data(int,travel_time,none,none);
			member_data(int,slowdown_index,none,none);
		};
		
		implementation struct Lane_Closure_Network_Event : public Base_Network_Event<MasterType,NT,APPEND_CHILD(Lane_Closure_Network_Event)>
		{
			feature_implementation static void Initialize_Type(){}
			
			feature_implementation void Initialize()
			{
				Base_Network_Event::Initialize();
			}
			
			//feature_implementation void Setup(odb::result<Network_Event>::iterator& itr)
			//{
			//	Base_Network_Event::Setup<Base_Network_Event,Weather_Network_Event,odb::result<Network_Event>::iterator&>(itr);

			//}

			declare_feature_conditional_implementation(Incident_Conditional)
			{
				response.next._iteration =_iteration + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION - 1;

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Event)
			{


			}

			member_data(int,lanes,none,none);
		};


		implementation struct Network_Event_Manager_Implementation : public Polaris_Component<APPEND_CHILD(Network_Event_Manager_Implementation),MasterType,Data_Object,ParentType>
		{
			feature_implementation void Initialize()
			{
				execute_static_typelist_loop(Initialize_List,MasterType::network_event_types);

				//Load_Event<ComponentType>(&Network_Event_Manager_Conditional<ComponentType,NT,NT>,&Network_Event_Manager_Event<ComponentType,NT,NT>,0,0);

				Read_DB<ComponentType,ComponentType,NT>();
			}

			typedef Network_Event<typename MasterType::type_of(base_network_event),ComponentType> Base_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(weather_network_event),ComponentType> Weather_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(accident_network_event),ComponentType> Accident_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(congestion_network_event),ComponentType> Congestion_Network_Event_Interface;
			typedef Network_Event<typename MasterType::type_of(lane_closure_network_event),ComponentType> Lane_Closure_Network_Event_Interface;

			feature_implementation void Read_DB()
			{
				using namespace odb;
				using namespace polaris;

				//read the initial data from input db
				string name("chicago");
				auto_ptr<database> db = open_sqlite_database(name);
				transaction t (db->begin());
				result<io::Network_Event> r = db->query<io::Network_Event> (query<io::Network_Event>::true_expr);

				cout << "A" << endl;

				for (result<io::Network_Event>::iterator itr=r.begin(); itr!=r.end(); ++itr)
				{
					cout << "B" << endl;

					const vector<weak_ptr<io::Event_Instance> >& instances = itr->getInstances();

					cout << "C" << endl;

					const string& name = itr->getName();

					for(vector<weak_ptr<io::Event_Instance>>::const_iterator vitr=instances.begin();vitr!=instances.end();vitr++)
					{
						weak_ptr<io::Event_Instance> _ptr=*vitr;

						if(name == "Weather")
						{

							Weather_Network_Event_Interface* wii = (Weather_Network_Event_Interface*)Allocate<MasterType::type_of(weather_network_event)>();
							wii->Setup< weak_ptr<io::Event_Instance>& >(_ptr);
							//Create_Network_Event<ComponentType,ComponentType,typename Weather_Network_Event_Interface::ComponentType>(wii);
						}
					}
					//else if()
					//{
					//}
					//else if()
					//{
					//}
					//else if()
					//{
					//}
				}
			}
			
			//declare_feature_conditional_implementation(Network_Event_Manager_Conditional)
			//{
			//	response.next._iteration =_iteration + 1;
			//	response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION;

			//	response.result = true;
			//}

			//declare_feature_event_implementation(Network_Event_Manager_Event)
			//{

			//}

			//feature_implementation void Get_Network_Events(int link_id,vector< Network_Event<TargetType,NT>* >& container)
			//{
			//	list<Network_Event<typename TargetType::ControlType,NT>*>* events_of_type = (list<Network_Event<typename TargetType::ControlType,NT>*>*) & (_network_event_container[TargetType::ControlType::component_index]);

			//	for(list< Network_Event<typename TargetType::ControlType,NT>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
			//	{
			//		Network_Event<typename TargetType::ControlType,NT>* network_event=*itr;

			//		vector<int>* affected_links = network_event->affected_links<vector<int>*>();

			//		for(vector<int>::iterator vitr = affected_links->begin();vitr != affected_links->end();vitr++)
			//		{
			//			if(*vitr == link_id)
			//			{
			//				container.push_back( *itr );
			//				break;
			//			}
			//		}
			//	}
			//}

			//feature_implementation void Create_Network_Event(Network_Event<TargetType,NT>* network_event)
			//{
			//	network_event->Initialize<NT>();

			//	_network_event_container[TargetType::ControlType::component_index]->push_back( (Base_Network_Event_Interface*) network_event );
			//}

			//feature_implementation void Remove_Network_Event(Network_Event<TargetType,NT>* network_event)
			//{
			//	list<Network_Event<typename TargetType::ControlType,NT>*>* events_of_type = (list<Network_Event<typename TargetType::ControlType,NT>*>*) & (_network_event_container[TargetType::ControlType::component_index]);

			//	for(list< Network_Event<typename TargetType::ControlType,NT>* >::iterator itr=events_of_type->begin();itr!=events_of_type->end();itr++)
			//	{
			//		if( (*itr) == network_event )
			//		{
			//			events_of_type->erase(itr);
			//		}
			//	}
			//}

			//member_data( concat(hash_map< int, list<Base_Network_Event_Interface*> >), network_event_container );
		};
	}
}
