#pragma once

#include "Models.h"


ostream* stream_ptr;



void main()
{
	//===============
	// OUTPUT OPTIONS
	//----------------
	ofstream log_file("signal_log3.txt");
	ostream output_stream(log_file.rdbuf());
	stream_ptr = &output_stream;	
	//----------------
	//stream_ptr = &cout;

	generator.seed(1);

	typedef Auto_Data<Master_Type>::type auto_data_type;
	typedef Bus_Data<Master_Type>::type bus_data_type;
	typedef Train_Data<Master_Type>::type train_data_type;
	typedef Master_Type::UTILITY_CHOICE_MODEL_TYPE choice_model_type;
	typedef Mode_Choice_Option<Master_Type>::type choice_option_type;
	typedef Mode_Choice_Data_Interface<auto_data_type,NULLTYPE> auto_data_interface;
	typedef Mode_Choice_Data_Interface<bus_data_type,NULLTYPE> bus_data_interface;
	typedef Mode_Choice_Data_Interface<train_data_type,NULLTYPE> train_data_interface;
	typedef Choice_Model_Components::Interfaces::Choice_Model_Interface<choice_model_type,NULLTYPE> choice_model_interface;	
	typedef choice_option_type::Interface_Type<>::type choice_option_interface;	

	//=====================================================
	// create general choice model
	choice_model_interface* mode_choice_model=(choice_model_interface*)Allocate<choice_model_type>();
	mode_choice_model->Initialize<NULLTYPE>();
	mode_choice_model->generator<std::tr1::mt19937_64*>(&generator);


	//=====================================================
	// Add three choice options to the mode choce model
	for (int i=0; i<3; i++)
	{
		choice_option_interface* choice_option = (choice_option_interface*)Allocate<choice_option_type>();
		mode_choice_model->choice_options<vector<choice_option_interface*>*>()->push_back((choice_option_interface*)choice_option);
	}

	//=====================================================
	// Create model input data for user 1 and 2
	auto_data_interface* auto_data = (auto_data_interface*)Allocate<auto_data_type>();
	bus_data_interface* bus_data = (bus_data_interface*)Allocate<bus_data_type>();
	train_data_interface* train_data = (train_data_interface*)Allocate<train_data_type>();
	auto_data->cost<float>(1.0f);
	auto_data->time<float>(1.0f);
	bus_data->cost<float>(1.0f);
	bus_data->time<float>(1.0f);
	bus_data->wait<float>(1.0f);
	train_data->cost<float>(1.0f);
	train_data->time<float>(1.0f);
	train_data->wait<float>(1.0f);
	auto_data_interface* auto_data2 = (auto_data_interface*)Allocate<auto_data_type>();
	bus_data_interface* bus_data2 = (bus_data_interface*)Allocate<bus_data_type>();
	train_data_interface* train_data2 = (train_data_interface*)Allocate<train_data_type>();
	auto_data2->cost<float>(3.0f);
	auto_data2->time<float>(3.0f);
	bus_data2->cost<float>(4.0f);
	bus_data2->time<float>(4.0f);
	bus_data2->wait<float>(4.0f);
	train_data2->cost<float>(5.0f);
	train_data2->time<float>(5.0f);
	train_data2->wait<float>(5.0f);

	//============================================
	// SOLVE CHOICE MODEL FOR EACH USER
	// user 1
	vector<choice_option_interface*>* choices = mode_choice_model->choice_options<vector<choice_option_interface*>*>();
	(*choices)[0]->choice_data_interface<auto_data_type>(auto_data);
	(*choices)[1]->choice_data_interface<bus_data_type>(bus_data);
	(*choices)[2]->choice_data_interface<train_data_type>(train_data);
	mode_choice_model->Evaluate_Choices<choice_option_type>();
	// user 2
	choices = mode_choice_model->choice_options<vector<choice_option_interface*>*>();
	(*choices)[0]->choice_data_interface<auto_data_type>(auto_data2);
	(*choices)[1]->choice_data_interface<bus_data_type>(bus_data2);
	(*choices)[2]->choice_data_interface<train_data_type>(train_data2);
	mode_choice_model->Evaluate_Choices<choice_option_type>();

	//data
	cout << "allocating data structures..." <<endl;	
	typedef Master_Type::scenario_type scenario_type;
	typedef Scenario_Components::Interfaces::Scenario_Interface<scenario_type,NULLTYPE> Scenario_Interface;
	
	Scenario_Interface* scenario=(Scenario_Interface*)Allocate<scenario_type>();
	//scenario->current_simulation_interval_index<int>(0);

	typedef Master_Type::network_type network_type;
	typedef Network_Components::Interfaces::Network_Interface<network_type,NULLTYPE> Network_Interface;

	
	Network_Interface* network=(Network_Interface*)Allocate<network_type>();
	
	////data input
	cout << "reading input data..." <<endl;	
	scenario->read_scenario_data<NULLTYPE>();
	network->read_network_data<NULLTYPE>();
	network->simulation_initialize<NULLTYPE>(scenario);
	

	////network cost
	cout << "constructing network cost..." <<endl;
	network->construct_network_cost<NULLTYPE>();
	//network->printNetwork<NULLTYPE>();
	
	////routable network
	network->construct_routable_network<NULLTYPE>();

	//DemandData demand_data;
	typedef Master_Type::demand_type demand_type;
	typedef Demand_Components::Interfaces::Demand_Interface<demand_type, NULLTYPE> Demand_Interface;
	Demand_Interface* demand = (Demand_Interface*)Allocate<demand_type>();
	demand->scenario_reference<Scenario_Interface*>(scenario);
	demand->network_reference<Network_Interface*>(network);

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
		int id=((Intersection_Interface*)(*intersections_itr))->uuid<int>();
		if(id!=0 && id!=4 && id!=6) ((Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();		
		else ((Intersection_Interface*)(*intersections_itr))->intersection_simulation_status<Intersection_Components::Types::Intersection_Simulation_Status>(Intersection_Components::Types::Intersection_Simulation_Status::COMPUTE_STEP_FLOW_COMPLETE);
	}

	world->Start_Turning();

	cout << "Finished!" << endl;
	//if (scenario_data.assignment_mode == ONE_SHOT_ASSIGNMENT_SIMULATION_MODE)
	//{
	//	one_shot_assignment_simulation_model(scenario_data,network_data,demand_data);
	//}
}