#pragma once
#include "Application_Includes.h"

struct Master_Type
{
	typedef Master_Type T;

	typedef Scenario_Components::Components::Polaris_Scenario_Component<T>::type scenario_type;

	typedef Network_Components::Components::Polaris_Network_Component<T>::type network_type;

	typedef Intersection_Components::Components::Polaris_Intersection_Component<T>::type intersection_type;
	typedef Intersection_Components::Components::Polaris_Movement_Component<T>::type movement_type;

	typedef Link_Components::Components::Polaris_Link_Component<T>::type link_type;
	typedef Link_Components::Components::Polaris_Turn_Movement_Component<T>::type turn_movement_type;

	typedef Vehicle_Components::Components::Polaris_Vehicle_Component<T>::type vehicle_type;

	typedef Routing_Components::Components::Routable_Network_Component<T>::type routable_network_type;

	typedef Routing_Components::Components::Polaris_Routing_Component<T>::type routing_type;

	typedef Intersection_Components::Components::Routable_Intersection_Component<T>::type routable_intersection_type;

	typedef Link_Components::Components::Routable_Link_Component<T>::type routable_link_type;

	typedef Intersection_Components::Components::Polaris_Movement_Component<T>::type movement_type;

	typedef Demand_Components::Components::Polaris_Demand_Component<T>::type demand_type;

	typedef Activity_Location_Components::Components::Polaris_Activity_Location_Component<T>::type activity_location_type;

	typedef Traveler_Components::Components::Polaris_Traveler_Component<T>::type traveler_type;

	typedef Intersection_Components::Components::Polaris_Inbound_Outbound_Movements_Component<T>::type inbound_outbound_movements_type;
	typedef Intersection_Components::Components::Polaris_Outbound_Inbound_Movements_Component<T>::type outbound_inbound_movements_type;

	typedef Intersection_Components::Components::Routable_Inbound_Outbound_Movements_Component<T>::type routable_inbound_outbound_movements_type;

	typedef Intersection_Components::Components::Routable_Movement_Component<T>::type routable_movement_type;

};


void main()
{
	Routing_Components::Interfaces::Routing_Interface<Master_Type::routing_type,NULLTYPE>* itf;

	itf=(Routing_Components::Interfaces::Routing_Interface<Master_Type::routing_type,NULLTYPE>*)Allocate<Master_Type::routing_type>();

	itf->Initialize<NULLTYPE>();


	//data
	cout << "allocating data structures..." <<endl;	
	typedef Master_Type::scenario_type scenario_type;
	typedef Scenario_Components::Interfaces::Scenario_Interface<scenario_type,NULLTYPE> Scenario_Interface;
	
	Scenario_Interface* scenario=(Scenario_Interface*)Allocate<scenario_type>();

	typedef Master_Type::network_type network_type;
	typedef Network_Components::Interfaces::Network_Interface<network_type,NULLTYPE> Network_Interface;
	typedef Master_Type::routable_network_type routable_network_type;
	typedef Routing_Components::Interfaces::Routable_Network_Interface<routable_network_type, NULLTYPE> Routable_Network_Interface;
	
	Network_Interface* network=(Network_Interface*)Allocate<network_type>();
	
	Routable_Network_Interface* routable_network = (Routable_Network_Interface*)Allocate<routable_network_type>();



	////data input
	cout << "reading input data..." <<endl;	
	scenario->read_scenario_data<NULLTYPE>();
	network->read_network_data<NULLTYPE>();
	routable_network->read_routable_network_data<NULLTYPE>(nullptr);
	//read_demand_data();

	////network cost
	cout << "constructing network cost..." <<endl;
	network->construct_network_cost<NULLTYPE>();
	
	//DemandData demand_data;
	typedef Master_Type::demand_type demand_type;
	typedef Demand_Components::Interfaces::Demand_Interface<demand_type, NULLTYPE> Demand_Interface;
	Demand_Interface* demand = (Demand_Interface*)Allocate<demand_type>();
	demand->read_demand_data<NULLTYPE>();

	////initialize network agents

	Master_Type::network_type::links_container_type::iterator links_itr;
	typedef Master_Type::link_type link_type;
	typedef Link_Components::Interfaces::Link_Interface<link_type,NULLTYPE> Link_Interface;

	for(links_itr=network->links_container<Master_Type::network_type::links_container_type&>().begin();
		links_itr!=network->links_container<Master_Type::network_type::links_container_type&>().end();
		links_itr++)
	{
		((Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
	}

	Master_Type::network_type::intersections_container_type::iterator intersections_itr;
	typedef Master_Type::intersection_type intersection_type;
	typedef Intersection_Components::Interfaces::Intersection_Interface<intersection_type,NULLTYPE> Intersection_Interface;

	for(intersections_itr=network->intersections_container<Master_Type::network_type::intersections_container_type&>().begin();
		intersections_itr!=network->intersections_container<Master_Type::network_type::intersections_container_type&>().end();
		intersections_itr++)
	{
		((Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();
	}

	world->Start_Turning();

	cout << "Finished!" << endl;
	//if (scenario_data.assignment_mode == ONE_SHOT_ASSIGNMENT_SIMULATION_MODE)
	//{
	//	one_shot_assignment_simulation_model(scenario_data,network_data,demand_data);
	//}
}