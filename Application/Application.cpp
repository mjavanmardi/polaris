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
};


void main()
{
	//data
	cout << "allocating data structures..." <<endl;	
	typedef Master_Type::scenario_type scenario_type;
	typedef Scenario_Components::Interfaces::Scenario_Interface<scenario_type,NULLTYPE> Scenario_Interface;
	
	Scenario_Interface* scenario=(Scenario_Interface*)Allocate<scenario_type>();

	typedef Master_Type::network_type network_type;
	typedef Network_Components::Interfaces::Network_Interface<network_type,NULLTYPE> Network_Interface;
	
	Network_Interface* network=(Network_Interface*)Allocate<network_type>();
	
	//DemandData demand_data;

	//data input
	cout << "reading input data..." <<endl;	
	scenario->read_scenario_data<NULLTYPE>();
	network->read_network_data<NULLTYPE>();
	//read_demand_data();

	//network cost
	cout << "constructing network cost..." <<endl;
	network->construct_network_cost<NULLTYPE>();
	
	//initialize network agents

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
}