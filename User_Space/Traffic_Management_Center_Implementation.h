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
			typedef typename Polaris_Component<APPEND_CHILD(Simple_TMC),MasterType,Execution_Object>::Component_Type ComponentType;
			// added for convinience
			typedef Network_Event<typename MasterType::type_of(base_network_event),ComponentType> Base_Network_Event_Interface;

			typedef Advisory_ITS<typename MasterType::type_of(variable_speed_sign),Simple_TMC> Variable_Speed_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(variable_word_sign),Simple_TMC> Variable_Word_Sign_Interface;
			typedef Advisory_ITS<typename MasterType::type_of(advisory_radio),Simple_TMC> Advisory_Radio_Interface;

			typedef Depot<typename MasterType::type_of(depot),Simple_TMC> Depot_Interface;
			typedef Link_Control<typename MasterType::type_of(link_control),Simple_TMC> Link_Control_Interface;
			typedef Sensor<typename MasterType::type_of(link_sensor),Simple_TMC> Sensor_Interface;

			// that places a pointer to network_event_manager_type
			//			  name of the prototype class		variable name					underlying type
			member_prototype(Network_Event_Manager, network_event_manager, typename MasterType::network_event_manager_type, none, none);

			member_data(vector<Variable_Word_Sign_Interface*>,variable_word_signs,none,none);
			member_data(vector<Variable_Speed_Sign_Interface*>,variable_speed_signs,none,none);
			member_data(vector<Advisory_Radio_Interface*>,advisory_radios,none,none);
			
			member_data(vector<Depot_Interface*>,depots,none,none);
			member_data(vector<Link_Control_Interface*>,link_controls,none,none);

			member_data(vector<Sensor_Interface*>,traffic_sensors,none,none);

			//                                          name of the function
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

				for(typename vector<Advisory_Radio_Interface*>::iterator itr=_advisory_radios.begin();itr!=_advisory_radios.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
					
				}

				for(typename vector<Variable_Word_Sign_Interface*>::iterator itr=_variable_word_signs.begin();itr!=_variable_word_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}

				for(typename vector<Variable_Speed_Sign_Interface*>::iterator itr=_variable_speed_signs.begin();itr!=_variable_speed_signs.end();itr++)
				{
					(*itr)->template Push_Network_Events<typename type_of(MasterType::base_network_event)>((vector<Network_Event<typename type_of(MasterType::base_network_event)>*>&)current_events);
				}
			}

			feature_implementation void Initialize()
			{
				this_component()->template Read_Database<ComponentType,CallerType,TargetType>();

				((ComponentType*)this)->template Load_Event<ComponentType>(&ComponentType::template TMC_Conditional<ComponentType,NT,NT>,&ComponentType::template TMC_Event<ComponentType,NT,NT>, 0, 0);
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

				
				cout << "\tVSS" << endl;

				result<VSS> vss_component_result=db->template query<VSS>(query<VSS>::true_expr);
				
				Variable_Speed_Sign_Interface::template Initialize_Type<NT>();

				for(typename result<VSS>::iterator db_itr = vss_component_result.begin (); db_itr != vss_component_result.end (); ++db_itr)
				{
					Variable_Speed_Sign_Interface* its_component = (Variable_Speed_Sign_Interface*)Allocate<typename Variable_Speed_Sign_Interface::ComponentType>();
					its_component->template Initialize< VSS& >( *db_itr );
					_variable_speed_signs.push_back(its_component);				
				}
				

				cout << "\tVWS" << endl;

				result<VMS> vws_component_result=db->template query<VMS>(query<VMS>::true_expr);

				Variable_Word_Sign_Interface::template Initialize_Type<NT>();

				for(typename result<VMS>::iterator db_itr = vws_component_result.begin (); db_itr != vws_component_result.end (); ++db_itr)
				{
					Variable_Word_Sign_Interface* its_component = (Variable_Word_Sign_Interface*)Allocate<typename Variable_Word_Sign_Interface::ComponentType>();
					its_component->template Initialize< VMS& >( *db_itr );
					_variable_word_signs.push_back(its_component);				
				}


				cout << "HAR" << endl;

				result<HAR> har_component_result=db->template query<HAR>(query<HAR>::true_expr);

				Advisory_Radio_Interface::template Initialize_Type<NT>();

				for(typename result<HAR>::iterator db_itr = har_component_result.begin (); db_itr != har_component_result.end (); ++db_itr)
				{
					Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate<typename Advisory_Radio_Interface::ComponentType>();
					its_component->Initialize< HAR& >( *db_itr );
					_advisory_radios.push_back(its_component);				
				}


				cout << "\tDepot" << endl;

				result<polaris::io::Depot> depot_component_result=db->template query<polaris::io::Depot>(query<polaris::io::Depot>::true_expr);

				Depot_Interface::template Initialize_Type<NT>();

				for(typename result<polaris::io::Depot>::iterator db_itr = depot_component_result.begin (); db_itr != depot_component_result.end (); ++db_itr)
				{
					Depot_Interface* its_component = (Depot_Interface*)Allocate<typename Depot_Interface::ComponentType>();
					its_component->template Initialize< polaris::io::Depot& >( *db_itr );
					_depots.push_back(its_component);				
				}


				cout << "\tLink Control" << endl;

				result<OpenShoulder> link_control_component_result=db->template query<OpenShoulder>(query<OpenShoulder>::true_expr);

				Link_Control_Interface::template Initialize_Type<NT>();

				for(typename result<OpenShoulder>::iterator db_itr = link_control_component_result.begin (); db_itr != link_control_component_result.end (); ++db_itr)
				{
					Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate<typename Link_Control_Interface::ComponentType>();
					its_component->template Initialize< OpenShoulder& >( *db_itr );
					_link_controls.push_back(its_component);				
				}


				cout << "\tSensor" << endl;

				result<Fixed_Sensor> sensor_component_result=db->template query<Fixed_Sensor>(query<Fixed_Sensor>::true_expr);

				Sensor_Interface::template Initialize_Type<NT>();

				for(typename result<Fixed_Sensor>::iterator db_itr = sensor_component_result.begin (); db_itr != sensor_component_result.end (); ++db_itr)
				{
					Sensor_Interface* its_component = (Sensor_Interface*)Allocate<typename Sensor_Interface::ComponentType>();
					its_component->template Initialize< Fixed_Sensor& >( *db_itr );
					_traffic_sensors.push_back(its_component);				
				}

				cout << "Done Reading" << endl;
			}

		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;