#pragma once

#include "Person_Platooning_Prototype.h"


namespace Platoon_Components
{
	namespace Implementations
	{
		implementation struct Person_Platooning_Implementation : public Polaris_Component<MasterType, INHERIT(Person_Platooning_Implementation), Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(Person_Platooning_Implementation), Data_Object>::Component_Type ComponentType;

			
			//typedef typename Polaris_Component<MasterType, INHERIT(Coordinated_Platooning_Implementation), Execution_Object>::Component_Type ComponentType;
			template<typename TargetType> void Initialize(int start_time);
			//static void Person_Platooning_Event_Notification(void* link, Network_Event<typename MasterType::platooning_type>* net_event);

			static void Person_Platooning_Event_Controller(ComponentType* _this, Event_Response& response);
		};


		template<typename MasterType, typename InheritanceList>
		void Person_Platooning_Implementation<MasterType, InheritanceList>::Person_Platooning_Event_Controller(ComponentType* _this, Event_Response& response)
		{
			int cur_iter = iteration();
			int cur_sub = sub_iteration();
			cout << "iteration()\t" << cur_iter <<  "\tsub_iteration()\t" << cur_sub << endl;

			//cout << "test";
		}


		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Person_Platooning_Implementation<MasterType, InheritanceList>::Initialize(int start_time)
		{
			this_component()->template Load_Event<ComponentType>(&Person_Platooning_Event_Controller, start_time, 1);

			//typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;

			//_Network_Event_Manager_Interface* network_event_manager = ((_Network_Interface*)_global_network)->template network_event_manager<_Network_Event_Manager_Interface*>();
			//network_event_manager->template Push_Subscriber<typename MasterType::platooning_type>(&Platoon_Event_Notification, (int)ComponentType::component_id);
			////network_event_manager->template Push_Subscriber<typename MasterType::accident_network_event_type>(&Accident_Event_Notification, (int)ComponentType::component_id);


		}

	}

}
