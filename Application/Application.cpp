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

	////data input
	cout << "reading input data..." <<endl;	
	scenario->read_scenario_data<NULLTYPE>();
	network->read_network_data<NULLTYPE>();
	//read_demand_data();

	////network cost
	cout << "constructing network cost..." <<endl;
	network->construct_network_cost<NULLTYPE>();
	

	world->Start_Turning();

	//if (scenario_data.assignment_mode == ONE_SHOT_ASSIGNMENT_SIMULATION_MODE)
	//{
	//	one_shot_assignment_simulation_model(scenario_data,network_data,demand_data);
	//}
}