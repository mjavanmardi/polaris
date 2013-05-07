#pragma once
#include "Traffic_Management_Center_Prototype.h"
//#include "Io\Geometry.h"
#include "Geometry_Implementation.h"


namespace Traffic_Management_Center_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Simple_TMC:public Polaris_Component<APPEND_CHILD(Simple_TMC),MasterType,Execution_Object>
		{
			// added for convinience
			typedef Network_Event<typename MasterType::type_of(base_network_event),ComponentType> Base_Network_Event_Interface;

			typedef Advisory_ITS<typename MasterType::type_of(variable_speed_sign),Simple_TMC> Variable_Speed_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(variable_word_sign),Simple_TMC> Variable_Word_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(advisory_radio),Simple_TMC> Advisory_Radio_Interface;

			typedef Depot<typename MasterType::type_of(depot),Simple_TMC> Depot_Interface;
			typedef Link_Control<typename MasterType::type_of(link_control),Simple_TMC> Link_Control_Interface;

			// that places a pointer to network_event_manager_type
			//			  name of the prototype class		variable name					underlying type
			member_prototype(Network_Event_Manager, network_event_manager, typename MasterType::network_event_manager_type, none, none);

			//                                          name of the dunction
			declare_feature_conditional_implementation(TMC_Conditional)
			{
				response.next._iteration = _iteration + 10;
				response.next._sub_iteration = 0;


				//this variable specifies wether the the Event fucntion to be called
				response.result = true;
			}

			declare_feature_event_implementation(TMC_Event)
			{
				ComponentType* pthis = (ComponentType*)_this;

				pthis->template Load_New_Events<ComponentType,ComponentType,NT>();
				pthis->DecideOnEventsToBeDisplayed<ComponentType,ComponentType,NT>();
			}

			feature_implementation void DecideOnEventsToBeDisplayed()
			{
				vector<Base_Network_Event_Interface*> current_events;
				_network_event_manager->Get_Network_Events<typename type_of(MasterType::base_network_event)>(current_events);
				for(vector<Advisory_Radio_Interface*>::iterator itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some claculations here
					(*itr)->Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}

				for(vector<Variable_Word_Sign_Interface*>::iterator itr=_variable_word_signs.begin();itr!=_variable_word_signs.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some claculations here
					(*itr)->Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}

				for(vector<Variable_Speed_Sign_Interface*>::iterator itr=_variable_speed_signs.begin();itr!=_variable_speed_signs.end();itr++)
				{
					vector<Base_Network_Event_Interface*> events_to_display;
					//some claculations here
					(*itr)->Push_Displayed_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)events_to_display);
				}


			}

			feature_implementation void Load_New_Events()
			{
				vector<Base_Network_Event_Interface*> current_events;
				_network_event_manager->template Get_Network_Events<typename type_of(MasterType::base_network_event)>(current_events);

				for(vector<Advisory_Radio_Interface*>::iterator itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
					
				}

				for(vector<Variable_Word_Sign_Interface*>::iterator itr=_variable_word_signs.begin();itr!=_variable_word_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}

				for(vector<Variable_Speed_Sign_Interface*>::iterator itr=_variable_speed_signs.begin();itr!=_variable_speed_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}
			}

			feature_implementation void Initialize()
			{
				this_component()->template Read_Database<ComponentType,CallerType,TargetType>();

				Load_Event<ComponentType>(&ComponentType::template TMC_Conditional<ComponentType,NT,NT>,&ComponentType::template TMC_Event<ComponentType,NT,NT>, 0, 0);
			}

			feature_implementation void Read_Database()
			{
				using namespace odb;
				using namespace polaris::io;
				
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				string db_name(((_Scenario_Interface*)_global_scenario)->template database_name<string&>());

				unique_ptr<database> db (open_sqlite_database (db_name));
				
				session s;

				transaction t(db->begin());

				cout << "Reading Components" << endl;

				
				cout << "VSS" << endl;

				result<VSS> vss_component_result=db->template query<VSS>(query<VSS>::true_expr);
				
				Variable_Speed_Sign_Interface::template Initialize_Type<NT>();

				for(result<VSS>::iterator db_itr = vss_component_result.begin (); db_itr != vss_component_result.end (); ++db_itr)
				{
					Variable_Speed_Sign_Interface* its_component = (Variable_Speed_Sign_Interface*)Allocate<Variable_Speed_Sign_Interface::ComponentType>();
					its_component->template Initialize< VSS& >( *db_itr );
					_variable_speed_signs.push_back(its_component);				
				}
				

				cout << "VWS" << endl;

				result<VMS> vws_component_result=db->template query<VMS>(query<VMS>::true_expr);

				Variable_Word_Sign_Interface::template Initialize_Type<NT>();

				for(result<VMS>::iterator db_itr = vws_component_result.begin (); db_itr != vws_component_result.end (); ++db_itr)
				{
					Variable_Word_Sign_Interface* its_component = (Variable_Word_Sign_Interface*)Allocate<Variable_Word_Sign_Interface::ComponentType>();
					its_component->template Initialize< VMS& >( *db_itr );
					_variable_word_signs.push_back(its_component);				
				}


				cout << "HAR" << endl;

				result<HAR> har_component_result=db->template query<HAR>(query<HAR>::true_expr);

				Advisory_Radio_Interface::Initialize_Type<NT>();

				for(result<HAR>::iterator db_itr = har_component_result.begin (); db_itr != har_component_result.end (); ++db_itr)
				{
					Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate<Advisory_Radio_Interface::ComponentType>();
					its_component->Initialize< HAR& >( *db_itr );
					_advisory_radios.push_back(its_component);				
				}


				cout << "Depot" << endl;

				result<polaris::io::Depot> depot_component_result=db->template query<polaris::io::Depot>(query<polaris::io::Depot>::true_expr);

				Depot_Interface::template Initialize_Type<NT>();

				for(result<polaris::io::Depot>::iterator db_itr = depot_component_result.begin (); db_itr != depot_component_result.end (); ++db_itr)
				{
					Depot_Interface* its_component = (Depot_Interface*)Allocate<Depot_Interface::ComponentType>();
					its_component->template Initialize< polaris::io::Depot& >( *db_itr );
					_depots.push_back(its_component);				
				}


				cout << "Link Control" << endl;

				result<OpenShoulder> link_control_component_result=db->template query<OpenShoulder>(query<OpenShoulder>::true_expr);

				Link_Control_Interface::Initialize_Type<NT>();

				for(result<OpenShoulder>::iterator db_itr = link_control_component_result.begin (); db_itr != link_control_component_result.end (); ++db_itr)
				{
					Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate<Link_Control_Interface::ComponentType>();
					its_component->template Initialize< OpenShoulder& >( *db_itr );
					_link_controls.push_back(its_component);				
				}

				cout << "Done Reading" << endl;

				//result<Component> component_result=db->template query<Component>(query<Component>::true_expr);

				//cout << "Reading Components: " << db_name << endl;

				//for(result<Component>::iterator db_itr = component_result.begin (); db_itr != component_result.end (); ++db_itr)
				//{
				//	const string& name = db_itr->getName();

				//	cout << name << endl;

				//	if(name == "Variable Speed Sign")
				//	{
				//		Variable_Speed_Sign_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
				//	}
				//	else if(name == "Variable Message Sign")
				//	{
				//		Variable_Word_Sign_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
				//	}
				//	else if(name == "Tow Truck Depot")
				//	{
				//		Depot_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());

				//		Depot_Interface* its_component = (Depot_Interface*)Allocate<Depot_Interface::ComponentType>();											
				//		vector<int>& covered_links = GetLinksInsideDepotPolygon(db_name);
				//		its_component->Initialize< vector<int>& >(covered_links);
				//		_depots.push_back(its_component);
				//	}
				//	else if(name == "HAR")
				//	{
				//		Advisory_Radio_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());

				//		map<string, POLY>& har_locations = GetCountyPolygons(db_name);

				//		for(map<string, POLY>::iterator itr = har_locations.begin(); itr!= har_locations.end(); itr++)
				//		{
				//			vector<int>& covered_links = GetLinksInsideCounty(db_name,itr->first);

				//			if(covered_links.size() > 0)
				//			{
				//				Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate<Advisory_Radio_Interface::ComponentType>();
				//				its_component->Initialize< vector<int>& >(covered_links);
				//				_advisory_radios.push_back(its_component);
				//			}
				//		}
				//	}
				//	else if(name == "Open Shoulder")
				//	{
				//		Link_Control_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
				//	}
				//	
				//	const vector<weak_ptr<Instance> >& instances = db_itr->getInstances();

				//	for(vector<weak_ptr<Instance>>::const_iterator vitr=instances.begin();vitr!=instances.end();vitr++)
				//	{
				//		weak_ptr<Instance> _ptr=*vitr;

				//		if(name == "Variable Speed Sign")
				//		{
				//			Variable_Speed_Sign_Interface* its_component = (Variable_Speed_Sign_Interface*)Allocate<Variable_Speed_Sign_Interface::ComponentType>();
				//			its_component->Initialize< weak_ptr<Instance>& >(_ptr);
				//			_variable_speed_signs.push_back(its_component);
				//		}
				//		else if(name == "Variable Message Sign")
				//		{
				//			Variable_Word_Sign_Interface* its_component = (Variable_Word_Sign_Interface*)Allocate<Variable_Word_Sign_Interface::ComponentType>();
				//			its_component->Initialize< weak_ptr<Instance>& >(_ptr);
				//			_variable_word_signs.push_back(its_component);
				//		}
				//		else if(name == "Open Shoulder")
				//		{
				//			Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate<Link_Control_Interface::ComponentType>();
				//			its_component->Initialize< weak_ptr<Instance>& >(_ptr);
				//			_link_controls.push_back(its_component);
				//		}
				//	}
				//}

				
			}

			member_data(vector<Variable_Word_Sign_Interface*>,variable_word_signs,none,none);
			member_data(vector<Variable_Speed_Sign_Interface*>,variable_speed_signs,none,none);
			member_data(vector<Advisory_Radio_Interface*>,advisory_radios,none,none);
			
			member_data(vector<Depot_Interface*>,depots,none,none);
			member_data(vector<Link_Control_Interface*>,link_controls,none,none);
		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;