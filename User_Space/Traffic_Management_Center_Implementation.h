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
			typedef Variable_Message_Sign<typename MasterType::type_of(variable_speed_sign),Simple_TMC> Variable_Speed_Sign_Interface;
			typedef Variable_Message_Sign<typename MasterType::type_of(variable_word_sign),Simple_TMC> Variable_Word_Sign_Interface;
			typedef Depot<typename MasterType::type_of(depot),Simple_TMC> Depot_Interface;
			typedef Advisory_Radio<typename MasterType::type_of(advisory_radio),Simple_TMC> Advisory_Radio_Interface;
			typedef Link_Control<typename MasterType::type_of(link_control),Simple_TMC> Link_Control_Interface;

			member_prototype(Scenario_Components::Prototypes::Scenario_Prototype, scenario_reference, typename MasterType::scenario_type, none, none);

			feature_implementation void Initialize()
			{
				this_component()->Read_Database<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void Read_Database()
			{
				using namespace odb;
				using namespace polaris::io;
				
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				string db_name(_scenario_reference->template database_name<string&>());

				auto_ptr<database> db (open_sqlite_database (db_name));
				
				session s;

				transaction t(db->begin());

				result<Component> component_result=db->template query<Component>(query<Component>::true_expr);

				cout << "Reading Components" << endl;

				for(result<Component>::iterator db_itr = component_result.begin (); db_itr != component_result.end (); ++db_itr)
				{
					const string& name = db_itr->getName();

					cout << name << endl;

					if(name == "Variable Speed Sign")
					{
						Variable_Speed_Sign_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
					}
					else if(name == "Variable Message Sign")
					{
						Variable_Word_Sign_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
					}
					else if(name == "Tow Truck Depot")
					{
						Depot_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());

						Depot_Interface* its_component = (Depot_Interface*)Allocate<Depot_Interface::ComponentType>();											
						vector<int>& covered_links = GetLinksInsideDepotPolygon(db_name);
						its_component->Initialize< vector<int>& >(covered_links);
						_depots.push_back(its_component);
					}
					else if(name == "HAR")
					{
						Advisory_Radio_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());

						map<string, POLY>& har_locations = GetCountyPolygons(db_name);

						for(map<string, POLY>::iterator itr = har_locations.begin(); itr!= har_locations.end(); itr++)
						{
							vector<int>& covered_links = GetLinksInsideCounty(db_name,itr->first);

							if(covered_links.size() > 0)
							{
								Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate<Advisory_Radio_Interface::ComponentType>();
								its_component->Initialize< vector<int>& >(covered_links);
								_advisory_radios.push_back(its_component);
							}
						}
					}
					else if(name == "Open Shoulder")
					{
						Link_Control_Interface::Initialize_Type<const vector<shared_ptr<Component_Key>>&>(db_itr->getKeys());
					}
					
					const vector<weak_ptr<Instance> >& instances = db_itr->getInstances();

					for(vector<weak_ptr<Instance>>::const_iterator vitr=instances.begin();vitr!=instances.end();vitr++)
					{
						weak_ptr<Instance> _ptr=*vitr;

						if(name == "Variable Speed Sign")
						{
							Variable_Speed_Sign_Interface* its_component = (Variable_Speed_Sign_Interface*)Allocate<Variable_Speed_Sign_Interface::ComponentType>();
							its_component->Initialize< weak_ptr<Instance>& >(_ptr);
							_variable_speed_signs.push_back(its_component);
						}
						else if(name == "Variable Message Sign")
						{
							Variable_Word_Sign_Interface* its_component = (Variable_Word_Sign_Interface*)Allocate<Variable_Word_Sign_Interface::ComponentType>();
							its_component->Initialize< weak_ptr<Instance>& >(_ptr);
							_variable_word_signs.push_back(its_component);
						}
						else if(name == "Open Shoulder")
						{
							Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate<Link_Control_Interface::ComponentType>();
							its_component->Initialize< weak_ptr<Instance>& >(_ptr);
							_link_controls.push_back(its_component);
						}
					}
				}

				cout << "Done Reading" << endl;
			}

			member_data(vector<Link_Control_Interface*>,link_controls,none,none);
			member_data(vector<Advisory_Radio_Interface*>,advisory_radios,none,none);
			member_data(vector<Depot_Interface*>,depots,none,none);
			member_data(vector<Variable_Word_Sign_Interface*>,variable_word_signs,none,none);
			member_data(vector<Variable_Speed_Sign_Interface*>,variable_speed_signs,none,none);
		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;