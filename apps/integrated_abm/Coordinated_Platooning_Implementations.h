#pragma once

#include "Coordinated_Platooning_Prototype.h"


namespace Platoon_Components
{
	namespace Implementations
	{
		implementation struct Coordinated_Platooning_Implementation : public Polaris_Component<MasterType, INHERIT(Coordinated_Platooning_Implementation), Execution_Object>
		{
			//m_prototype(Platoon_Components::Prototypes::Person_Platooning, typename MasterType::person_platooning_type, Person_Platooning_Faculty, NONE, NONE);
			
			//typedef typename Polaris_Component<MasterType, INHERIT(Coordinated_Platooning_Implementation), Execution_Object>::Component_Type ComponentType;
			template<typename TargetType> void Initialize(int start_time);
			static void Coordinated_Platooning_Event_Notification(void* link, Network_Event<typename MasterType::platooning_type>* net_event);

			static void Coordinated_Platooning_Event_Controller(ComponentType* _this, Event_Response& response);
		};
		
		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Coordinated_Platooning_Implementation<MasterType, InheritanceList>::Initialize(int start_time)
		{
			cout << "1- called the Coordinated_Platooning_Event_Controller" << endl;
			this_component()->template Load_Event<ComponentType>(&Coordinated_Platooning_Event_Controller, start_time, 1);

			
			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			//_Network_Event_Manager_Interface* network_event_manager = ((_Network_Interface*)_global_network)->template network_event_manager<_Network_Event_Manager_Interface*>();
			//network_event_manager->template Push_Subscriber<typename MasterType::platooning_type>(&Platoon_Event_Notification, (int)ComponentType::component_id);
			////network_event_manager->template Push_Subscriber<typename MasterType::accident_network_event_type>(&Accident_Event_Notification, (int)ComponentType::component_id);


		}


		template<typename MasterType, typename InheritanceList>
		void Coordinated_Platooning_Implementation<MasterType, InheritanceList>::Coordinated_Platooning_Event_Notification(void* link, Network_Event<typename MasterType::platooning_type>* net_event)
		{
			//typedef typename MasterType::link_type _Link_Component;
			//((_Link_Component*)link)->record_weather_event(net_event);
		}


		template<typename MasterType, typename InheritanceList>
		void Coordinated_Platooning_Implementation<MasterType, InheritanceList>::Coordinated_Platooning_Event_Controller(ComponentType* _this, Event_Response& response)
		{
			int cur_iter = iteration();
			int cur_sub = sub_iteration();
		}

		//implementation struct Platoon_Data_Implementation : public Polaris_Component<MasterType, INHERIT(Platoon_Data_Implementation), Data_Object>
		//{
		//	m_data(int, lead_vehicle_id, NONE, NONE);
		//	m_data(int, link_arrival_time, NONE, NONE);
		//	m_data(int, node_wait_time, NONE, NONE);
		//	m_prototype(Link_Components::Prototypes::Link, typename MasterType::link_type, link, NONE, NONE);
		//};

	}
}