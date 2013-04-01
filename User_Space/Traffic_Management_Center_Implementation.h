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
			typedef Link_Control<typename MasterType::type_of(link_control),Simple_TMC> Link_Control_Interface;
			typedef Advisory_Radio<typename MasterType::type_of(advisory_radio),Simple_TMC> Advisory_Radio_Interface;
			typedef Depot<typename MasterType::type_of(depot),Simple_TMC> Depot_Interface;
			typedef Variable_Message_Sign<typename MasterType::type_of(variable_message_sign),Simple_TMC> Variable_Message_Sign_Interface;
			
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
				//string name(_scenario_reference->template database_name<string&>());

				string name("chicago");

				auto_ptr<database> db (open_sqlite_database (name));
				transaction t(db->begin());

				result<Component> component_result=db->template query<Component>(query<Component>::true_expr);

				cout << "Reading Component Mappings" << endl;


				for(result<Component>::iterator db_itr = component_result.begin (); db_itr != component_result.end (); ++db_itr)
				{
					if(db_itr->getName() == "Variable Speed Sign")
					{
						_variable_message_sign_identifier = db_itr->getId();
					}
					else if(db_itr->getName() == "Tow Truck Depot")
					{
						_depot_identifier = db_itr->getId();
					}
					else if(db_itr->getName() == "HAR")
					{
						_advisory_radio_identifier = db_itr->getId();
					}
					else if(db_itr->getName() == "Open Shoulder")
					{
						_link_control_identifier = db_itr->getId();
					}
				}

				result<Instance> instance_result=db->template query<Instance>(query<Instance>::true_expr);

				cout << "Reading Instances" << endl;

				for(result<Instance>::iterator db_itr = instance_result.begin (); db_itr != instance_result.end (); ++db_itr)
				{
					int component_id=db_itr->getComponent()->getId();

					if(component_id==_link_control_identifier)
					{
						Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate< Link_Control_Interface::ComponentType >();

						its_component->x_position<float>(db_itr->getLocation_X());
						its_component->y_position<float>(db_itr->getLocation_Y());

						_link_controls.push_back(its_component);

					}
					else if(component_id==_variable_message_sign_identifier)
					{
						Variable_Message_Sign_Interface* its_component = (Variable_Message_Sign_Interface*)Allocate< Variable_Message_Sign_Interface::ComponentType >();

						its_component->x_position<float>(db_itr->getLocation_X());
						its_component->y_position<float>(db_itr->getLocation_Y());

						_variable_message_signs.push_back(its_component);
					}
				}










				result<Component_Key> component_key_result=db->template query<Component_Key>(query<Component_Key>::true_expr);

				cout << "Reading Component Keys" << endl;

				pair<string,string> key_value;

				for(result<Component_Key>::iterator db_itr = component_key_result.begin (); db_itr != component_key_result.end (); ++db_itr)
				{
					string component_key = db_itr->getKey();

					int component_id=db_itr->getComponent()->getId();

					if(component_id==_link_control_identifier)
					{
						_link_control_keys.push_back(component_key);
					}
					else if(component_id==_advisory_radio_identifier)
					{
						_advisory_radio_keys.push_back(component_key);
					}
					else if(component_id==_depot_identifier)
					{
						_depot_keys.push_back(component_key);
					}
					else if(component_id==_variable_message_sign_identifier)
					{
						_variable_message_sign_keys.push_back(component_key);
					}
				}

				
				//cout << "Reading Polygons" << endl;

				//map<int, POLY>& polys=GetDepotPolygons("chicago");

				//for(map<int, POLY>::iterator itr = polys.begin();itr != polys.end();itr++)
				//{
				//	Depot_Interface* its_component = (Depot_Interface*)Allocate< Depot_Interface::ComponentType >();

				//	vector<Point_3D<MasterType>>* area = &its_component->covered_area<vector<Point_3D<MasterType>>&>();

				//	vector<GeoPoint>* coords = &itr->second.exterior.coords;

				//	Point_3D<MasterType> poly_point;

				//	cout << "new polygon:" << endl;
				//	for(vector<GeoPoint>::iterator pitr = coords->begin(); pitr != coords->end(); pitr++)
				//	{
				//		poly_point._x = pitr->x;
				//		poly_point._y = pitr->y;
				//		poly_point._z = 1;

				//		cout << poly_point._x << "," << poly_point._y << endl;
				//		

				//		area->push_back(poly_point);
				//	}
				//}

				//result<Component_Key> component_key_result=db->template query<Component_Key>(query<Component_Key>::true_expr);

				//cout << "Reading Component Keys" << endl;

				//pair<string,string> key_value;

				//for(result<Component_Key>::iterator db_itr = component_key_result.begin (); db_itr != component_key_result.end (); ++db_itr)
				//{
				//	string component_key = db_itr->key();

				//	_component_keys.push_back(component_key);

				//	int component_id=db_itr->getComponent()->getId();

				//	if(component_id==_link_control_identifier)
				//	{
				//		Link_Control_Interface* its_component = (Link_Control_Interface*)Allocate< Link_Control_Interface::ComponentType >();

				//		link_controls_keys.push_back(component_key);
				//		key_value.first = 
				//		its_component->active_properties<vector<pair<string,string>>&>().push_back(component_key);

				//	}
				//	else if(component_id==_advisory_radio_identifier)
				//	{
				//		Advisory_Radio_Interface* its_component = (Advisory_Radio_Interface*)Allocate< Advisory_Radio_Interface::ComponentType >();

				//		its_component->active_properties<vector<pair<string,string>>&>().push_back(component_key);
				//	}
				//	else if(component_id==_depot_identifier)
				//	{
				//		Depot_Interface* its_component = (Depot_Interface*)Allocate< Depot_Interface::ComponentType >();

				//		its_component->active_properties<vector<pair<string,string>>&>().push_back(component_key);
				//	}
				//	else if(component_id==_variable_message_sign_identifier)
				//	{
				//		Variable_Message_Sign_Interface* its_component = (Variable_Message_Sign_Interface*)Allocate< Variable_Message_Sign_Interface::ComponentType >();
				//		
				//		its_component->active_properties<vector<pair<string,string>>&>().push_back(component_key);
				//	};

				//}


				cout << "Done Reading" << endl;
			}

			member_data(int,link_control_identifier,none,none);
			member_data(vector<Link_Control_Interface*>,link_controls,none,none);
			member_data(vector<string>,link_control_keys,none,none);

			member_data(int,advisory_radio_identifier,none,none);
			member_data(vector<Advisory_Radio_Interface*>,advisory_radios,none,none);
			member_data(vector<string>,advisory_radio_keys,none,none);

			member_data(int,depot_identifier,none,none);
			member_data(vector<Depot_Interface*>,depots,none,none);
			member_data(vector<string>,depot_keys,none,none);

			member_data(int,variable_message_sign_identifier,none,none);
			member_data(vector<Variable_Message_Sign_Interface*>,variable_message_signs,none,none);
			member_data(vector<string>,variable_message_sign_keys,none,none);

			member_data(vector<string>,component_keys,none,none);
		};
	}
}

using namespace Traffic_Management_Center_Components::Implementations;