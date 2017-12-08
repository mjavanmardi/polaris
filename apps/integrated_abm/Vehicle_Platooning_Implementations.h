#pragma once

#include "Vehicle_Platooning_Prototype.h"


namespace Platoon_Components
{
	namespace Implementations
	{
		implementation struct Vehicle_Platooning_Implementation : public Polaris_Component<MasterType, INHERIT(Vehicle_Platooning_Implementation), Execution_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType, INHERIT(Vehicle_Platooning_Implementation), Data_Object>::Component_Type ComponentType;

			
			//typedef typename Polaris_Component<MasterType, INHERIT(Coordinated_Platooning_Implementation), Execution_Object>::Component_Type ComponentType;
			template<typename TargetType> void Initialize(int start_time);
			//static void Vehicle_Platooning_Event_Notification(void* link, Network_Event<typename MasterType::platooning_type>* net_event);

			static void Vehicle_Platooning_Event_Controller(ComponentType* _this, Event_Response& response);
		};


		template<typename MasterType, typename InheritanceList>
		void Vehicle_Platooning_Implementation<MasterType, InheritanceList>::Vehicle_Platooning_Event_Controller(ComponentType* _this, Event_Response& response)
		{
			int cur_iter = iteration();
			int cur_sub = sub_iteration();
			cout << "iteration()\t" << cur_iter <<  "\tsub_iteration()\t" << cur_sub;

			typedef Vehicle_Platooning<ComponentType> _Vehicle_Platooning_Interface;
			ComponentType* _pthis = (ComponentType*)_this;
			_Vehicle_Platooning_Interface* pthis = (_Vehicle_Platooning_Interface*)_pthis;
			
			if (1 < 2)
			{
				response.next._iteration = 12;
				//_this->
			}
			//cout << "test";
		}


		template<typename MasterType, typename InheritanceList>
		template<typename TargetType>
		void Vehicle_Platooning_Implementation<MasterType, InheritanceList>::Initialize(int start_time)
		{
			this_component()->template Load_Event<ComponentType>(&Vehicle_Platooning_Event_Controller, start_time, 1);

			//typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;

			//_Network_Event_Manager_Interface* network_event_manager = ((_Network_Interface*)_global_network)->template network_event_manager<_Network_Event_Manager_Interface*>();
			//network_event_manager->template Push_Subscriber<typename MasterType::platooning_type>(&Platoon_Event_Notification, (int)ComponentType::component_id);
			////network_event_manager->template Push_Subscriber<typename MasterType::accident_network_event_type>(&Accident_Event_Notification, (int)ComponentType::component_id);


		}

	}

}
