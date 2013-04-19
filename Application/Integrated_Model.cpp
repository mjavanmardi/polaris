#include "Polaris_PCH.h"


// SET THE DEBUG VERSION TO SIMULATE ONLY ONE AGENT
#ifdef _DEBUG
//#define DEBUG_1
#endif

#ifdef DBIO
#define WINDOWS
#include "Application_Includes.h"


struct MasterType
{
	typedef MasterType M;

	#ifdef ANTARES
	typedef Conductor_Implementation<M> conductor_type;
	typedef Control_Panel_Implementation<M> control_panel_type;
	typedef Time_Panel_Implementation<M> time_panel_type;
	typedef Information_Panel_Implementation<M> information_panel_type;
	typedef Canvas_Implementation<M> canvas_type;
	typedef Antares_Layer_Implementation<M> antares_layer_type;
	typedef Layer_Options_Implementation<M> layer_options_type;
	typedef Attributes_Panel_Implementation<M> attributes_panel_type;
	typedef Control_Dialog_Implementation<M> control_dialog_type;
	typedef Information_Page_Implementation<MasterType> information_page_type;

	typedef Antares_Network_Implementation<M> network_type;
	typedef Antares_Link_Implementation<M> link_type;
	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> vehicle_type;	
	typedef Zone_Components::Implementations::Graphical_Zone_Implementation<M> zone_type;
	typedef Antares_Intersection_Implementation<M> intersection_type;
	typedef Zone_Components::Implementations::Graphical_Zone_Group_Implementation<M> graphical_zone_group_type;
	#else
	typedef Network_Components::Implementations::Integrated_Polaris_Network_Implementation<M> network_type;
	typedef Link_Components::Implementations::Polaris_Link_Implementation<M> link_type;
	typedef Intersection_Components::Implementations::Polaris_Intersection_Implementation<M> intersection_type;
	typedef Vehicle_Components::Implementations::Polaris_Vehicle_Implementation<M> vehicle_type;
	typedef Zone_Components::Implementations::Polaris_Zone_Implementation<M> zone_type;
	#endif

	//==============================================================================================
	// Network Types
	typedef Scenario_Components::Implementations::Polaris_Scenario_Implementation<M> scenario_type;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<M> network_db_reader_type;
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<M> movement_type;

	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<M> turn_movement_type;
	typedef Routing_Components::Implementations::Routable_Network_Implementation<M> routable_network_type;
	typedef Routing_Components::Implementations::Polaris_Routing_Implementation<M> routing_type;
	typedef Routing_Components::Implementations::Polaris_Skim_Routing_Implementation<M> skim_routing_type;
	typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<M> routable_intersection_type;
	typedef Link_Components::Implementations::Routable_Link_Implementation<M> routable_link_type;
	typedef Activity_Location_Components::Implementations::Polaris_Activity_Location_Implementation<M> activity_location_type;
	typedef Traveler_Components::Implementations::Polaris_Traveler_Implementation<M> traveler_type;
	typedef Vehicle_Components::Implementations::Polaris_Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;
	typedef Intersection_Components::Implementations::Polaris_Inbound_Outbound_Movements_Implementation<M> inbound_outbound_movements_type;
	typedef Intersection_Components::Implementations::Polaris_Outbound_Inbound_Movements_Implementation<M> outbound_inbound_movements_type;
	typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<M> routable_inbound_outbound_movements_type;
	typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<M> routable_outbound_inbound_movements_type;
	typedef Intersection_Components::Implementations::Routable_Movement_Implementation<M> routable_movement_type;
	typedef Operation_Components::Implementations::Polaris_Operation_Implementation<M> operation_type;
	typedef Intersection_Control_Components::Implementations::Polaris_Intersection_Control_Implementation<M> intersection_control_type;
	typedef Intersection_Control_Components::Implementations::Polaris_Control_Plan_Implementation<M> control_plan_type;
	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Implementation<M> phase_type;
	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Movement_Implementation<M> phase_movement_type;
	typedef Intersection_Control_Components::Implementations::Polaris_Approach_Implementation<M> approach_type;
	typedef Plan_Components::Implementations::Polaris_Plan_Implementation<M> plan_type;
	typedef Movement_Plan_Components::Implementations::Polaris_Integrated_Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	
	// DEMAND AGENT Types
	typedef Person_Components::Implementations::Person_Implementation<M> person_type;
	typedef Person_Components::Implementations::CTRAMP_Person_Planner_Implementation<M, person_type> person_planner_type;
	typedef Person_Components::Implementations::CTRAMP_Activity_Generator_Implementation<M, person_type> activity_generator_type;
	typedef Person_Components::Implementations::ADAPTS_Person_Properties_Implementation<M,person_type> person_properties_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<M> person_static_properties_type;
	typedef RNG_Components::Implementations::RngStream_Implementation<M> RNG;

	typedef Activity_Components::Implementations::Basic_Activity_Plan_Implementation<M,person_type> activity_type;
	typedef Activity_Components::Implementations::ADAPTS_Activity_Plan_Implementation<M,person_type> activity_plan_type;
	typedef Activity_Components::Implementations::ADAPTS_Routine_Activity_Plan_Implementation<M,person_type> routine_activity_plan_type;
	typedef Activity_Components::Implementations::Activity_Record<M,person_type> activity_record_type;

	// destination choice types
	typedef Person_Components::Implementations::CTRAMP_Destination_Chooser_Implementation<M,person_type> person_destination_chooser_type;
	typedef Person_Components::Implementations::CTRAMP_Destination_Choice_Option<M,person_type> person_destination_choice_option_type;
	
	
	// POPULATION SYNTHESIS CLASSES
	typedef PopSyn::Implementations::Synthesis_Zone_Implementation<M> zone;
	typedef PopSyn::Implementations::Synthesis_Region_Implementation<M> region;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<M> IPF_Solver_Settings;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<M> popsyn_solver;



	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
#ifdef ANTARES
	typedef Network_Event_Components::Implementations::Antares_Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Antares_Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck_Depot<MasterType> depot_type;
	typedef Advisory_Radio_Components::Implementations::Antares_Highway_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Speed_Sign<MasterType> variable_speed_sign_type;
#else
	typedef Network_Event_Components::Implementations::Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Tow_Truck_Depot<MasterType> depot_type;
	typedef Advisory_Radio_Components::Implementations::Highway_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Speed_Sign<MasterType> variable_speed_sign_type;
#endif

	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;
};
ostream* stream_ptr;

int main(int argc,char** argv)
{
	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region Copy from NetworkModel.cpp
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	// OUTPUT OPTIONS
	ofstream log_file("signal_log3.txt");
	ostream output_stream(log_file.rdbuf());
	stream_ptr = &output_stream;	
	//----------------
	//stream_ptr = &cout;

	string output_dir_name = "";

	network_models::network_information::scenario_data_information::ScenarioData scenario_data_for_output;
	network_models::network_information::network_data_information::NetworkData network_data_for_output;
	network_models::network_information::demand_data_information::DemandData demand_data_for_output;
	network_models::network_information::operation_data_information::OperationData operation_data_for_output;
	network_models::initialization(scenario_data_for_output,network_data_for_output,demand_data_for_output,operation_data_for_output);


	//data
	cout << "allocating data structures..." <<endl;	
	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
	
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
	
	_Network_Interface* network=(_Network_Interface*)Allocate<typename MasterType::network_type>();
	_global_network = network;
	network->template scenario_reference<_Scenario_Interface*>(scenario);
	
	////data input
	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>();
	cout << "converting scenario data..." << endl;
	scenario->write_scenario_data<NULLTYPE>(scenario_data_for_output);
	network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);
	cout<<"writing scenario data..."<<endl;
	network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);	

	cout << "reading network data..." <<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);

	cout << "converting network data..." << endl;
	network->write_network_data<Target_Type<NULLTYPE,void,network_models::network_information::network_data_information::NetworkData&>>(network_data_for_output);
	network_models::network_information::network_data_information::write_network_data("", network_data_for_output);
	cout<<"writing network data..."<<endl;
	//network_models::network_information::network_data_information::write_network_data(output_dir_name,network_data_for_output);

	//==================================================================================================================================
	// Start Antares UI
	//----------------------------------------------------------------------------------------------------------------------------------
#ifdef ANTARES
	network->set_network_bounds<NULLTYPE>();
	Rectangle_XY<MasterType>* local_bounds=network->network_bounds<Rectangle_XY<MasterType>*>();
	START_UI(MasterType,local_bounds->_xmin,local_bounds->_ymin,local_bounds->_xmax,local_bounds->_ymax);
	MasterType::vehicle_type::Initialize_Layer();
	network->initialize_antares_layers<NULLTYPE>();
	MasterType::link_type::configure_link_moes_layer();
#endif
	
	//Network_Event<MasterType::network_event_manager_type>* net_event_manager=(Network_Event<MasterType::network_event_manager_type>*)Allocate<MasterType::network_event_manager_type>();

	//network->network_event_manager<MasterType::network_event_manager_type*>( (MasterType::network_event_manager_type*)net_event_manager );

	//net_event_manager->Initialize<NT>();
	
	//typedef Traffic_Management_Center<MasterType::traffic_management_center_type,NT> TMC_Interface;
	//TMC_Interface* tmc = (TMC_Interface*) Allocate< MasterType::traffic_management_center_type >();

	//network->traffic_management_center<MasterType::traffic_management_center_type*>(tmc);

	//tmc->scenario_reference<_Scenario_Interface*>(scenario);
	//tmc->Initialize<NT>();

	cout << "initializing simulation..." <<endl;	
	network->simulation_initialize<NULLTYPE>();

	define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	if (scenario->intersection_control_flag<int>() == 1) {
		cout <<"reading operation data..." << endl;
		operation->read_operation_data<Net_IO_Type>(network_io_maps);
	}
	cout <<"converting operation data..." << endl;
	operation->write_operation_data<NULLTYPE>(network_data_for_output, operation_data_for_output);
	cout<<"writing operation data..."<<endl;
	network_models::network_information::operation_data_information::write_operation_data(output_dir_name,scenario_data_for_output,operation_data_for_output,network_data_for_output);

	//network_models::write_data("",scenario_data_for_output,demand_data_for_output,network_data_for_output, operation_data_for_output);

	////initialize network agents	
	cout << "initializing link agents..." <<endl;
	define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
	_Links_Container_Interface::iterator links_itr;

	for(links_itr=network->links_container<_Links_Container_Interface&>().begin();
		links_itr!=network->links_container<_Links_Container_Interface&>().end();
		links_itr++)
	{
		((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
	}

	cout << "initializing intersection agents..." <<endl;
	define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, _Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Prototype, NULLTYPE);
	_Intersections_Container_Interface::iterator intersections_itr;

	for(intersections_itr=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().begin();
		intersections_itr!=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().end();
		intersections_itr++)
	{
		((_Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();
	}
	cout << "starting sim..." <<endl;
	#pragma endregion

	
	//==================================================================================================================================
	// Initialize global randon number generators
	//---------------------------------------------------------------------------------------------------------------------------------- 
	GLOBALS::Normal_RNG.Set_Seed<int>(100);
	GLOBALS::Uniform_RNG.Set_Seed<int>(200);


	//==================================================================================================================================
	// Set up graphical display
	//----------------------------------------------------------------------------------------------------------------------------------
	#ifdef ANTARES
	define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, NULLTYPE);
	_Zones_Container_Interface::iterator zone_itr;
	_Zones_Container_Interface* zone_list = network->zones_container<_Zones_Container_Interface*>();

	//--------------------------------------------------------------------------------------------
	// Graphical zone group display - integrate to graphical network when database is fixed
	typedef Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,NULLTYPE> zone_group_interface;
	zone_group_interface* _graphical_zone_group = (zone_group_interface*) Allocate<MasterType::graphical_zone_group_type>();	
	// initialize zone static reference to the graphical zone group
	MasterType::zone_type::_graphical_zone_group=(Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,MasterType::zone_type>*)_graphical_zone_group;
	_graphical_zone_group->configure_zones_layer<NULLTYPE>();
	#endif


	//==================================================================================================================================
	// Network Skimming stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	define_component_interface(_network_skim_itf, _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,NULLTYPE);
	_network_skim_itf* skimmer = (_network_skim_itf*)Allocate<_Network_Interface::get_type_of(skimming_faculty)>();
	skimmer->Initialize<_Network_Interface*>(network);
	network->skimming_faculty<_network_skim_itf*>(skimmer);


	//==================================================================================================================================
	// Destination choice model - set parameters
	//----------------------------------------------------------------------------------------------------------------------------------
	MasterType::person_destination_chooser_type::_choice_set_size = 50;
	MasterType::person_destination_choice_option_type::_B_TTIME = -0.1;
	MasterType::person_destination_choice_option_type::_B_EMPLOYMENT = 0.0002;
	MasterType::person_destination_choice_option_type::_B_POPULATION = 0.00005;
		

	//==================================================================================================================================
	// POPSYN stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	#ifdef DEBUG_1
	typedef Person_Components::Prototypes::Person<MasterType::person_type> person_itf;
	typedef Person_Components::Prototypes::Person_Properties<MasterType::person_properties_type> properties_itf;
	typedef Person_Components::Prototypes::Activity_Generator<MasterType::activity_generator_type> generator_itf;
	typedef Person_Components::Prototypes::Person_Planner<MasterType::person_planner_type> planner_itf;
	typedef Person_Components::Prototypes::Person_Properties<MasterType::person_static_properties_type> pop_unit_itf;
	pop_unit_itf* data = (pop_unit_itf*)Allocate<MasterType::person_static_properties_type>();
	data->Age<int>(26);
	data->Class_of_worker<Person_Components::Types::CLASS_OF_WORKER>(Person_Components::Types::CLASS_OF_WORKER::COW_EMPLOYEE);
	data->Employment_Status<Person_Components::Types::EMPLOYMENT_STATUS>(Person_Components::Types::EMPLOYMENT_STATUS::EMPLOYMENT_STATUS_CIVILIAN_AT_WORK);
	data->Journey_To_Work_Travel_Time<Time_Minutes>(3.0);
	data->Journey_To_Work_Arrival_Time<int>(3);
	data->Work_Hours<Time_Hours>(40.0);
	person_itf* p = (person_itf*)Allocate<MasterType::person_type>();
	p->network_reference<_Network_Interface*>(network);
	p->scenario_reference<_Scenario_Interface*>(scenario);	
	p->Initialize<int>(1);
	p->Static_Properties<pop_unit_itf*>(data);
	p->Home_Location<int>(65);
	p->Work_Location<int>(256);

	#else
	//ofstream out, marg_out, popsyn_log;
	//out.open("full_population.csv",ios_base::out);
	//popsyn_log.open("popsyn_log.csv",ios_base::out);
	//marg_out.open("marginals_and_distributions.csv",ios_base::out);

	//// IPF Solver Settings
	//define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
	//solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
	//solver->Initialize<Target_Type<NULLTYPE,void,double,int>>(scenario->ipf_tolerance<float>(),scenario->percent_to_synthesize<float>(),scenario->maximum_iterations<int>());

	define_component_interface(popsyn_itf,MasterType::popsyn_solver,PopSyn::Prototypes::Population_Synthesizer_Prototype,NULLTYPE);
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::popsyn_solver>();
	//popsyn->write_marginal_output_flag<bool>(scenario->write_marginal_output<bool>());
	//popsyn->write_full_output_flag<bool>(scenario->write_full_output<bool>());
	//popsyn->linker_file_path<string>(string("linker_file.txt"));
	//popsyn->Solution_Settings<solver_itf*>(solver);
	////popsyn->Output_Stream<ostream&>(out);
	////popsyn->Log_File<ostream&>(popsyn_log);
	////popsyn->Marginal_Output_Stream<ostream&>(marg_out);
	//popsyn->network_reference<_Network_Interface*>(network);
	//popsyn->scenario_reference<_Scenario_Interface*>(scenario);
	popsyn->Initialize<Target_Type<NT,NT,_Network_Interface*, _Scenario_Interface*> >(network,scenario);
	#endif
	//----------------------------------------------------------------------------------------------------------------------------------

	//==================================================================================================================================
	// Logging of activity generation / scheduling outputs
	//----------------------------------------------------------------------------------------------------------------------------------
	stringstream logfilename;
	MasterType::person_planner_type::_write_activity_files = true;
	for (int i = 0; i < _num_threads; ++i)
	{
		logfilename.str("");
		logfilename << "generated_acts_" << i << ".csv";
		MasterType::person_planner_type::logs[i].open(logfilename.str());
		MasterType::person_planner_type::logs[i] << "PERID,DEPART,ORIG,DEST,EST_TTIME"<<endl;
	}
	//----------------------------------------------------------------------------------------------------------------------------------
	


	//==================================================================================================================================
	// Start Simulation
	//----------------------------------------------------------------------------------------------------------------------------------
	try
	{
	START();
	}
	catch (std::exception ex)
	{
		cout << ex.what();
	}

	cout << "Finished!" << endl;
	system("PAUSE");
}
#endif























//============================================
// USE THIS FOR RUNNING ON LINUX
//--------------------------------------------
//#ifndef DBIO
//#define WINDOWS
//#include "Application_Includes.h"
//#include "../File_IO/network_models.h"
//struct MasterType
//{
//	typedef MasterType M;
//
//#ifdef ANTARES
//	typedef Conductor_Implementation<M> conductor_type;
//	typedef Control_Panel_Implementation<M> control_panel_type;
//	typedef Time_Panel_Implementation<M> time_panel_type;
//	typedef Information_Panel_Implementation<M> information_panel_type;
//	typedef Canvas_Implementation<M> canvas_type;
//	typedef Antares_Layer_Implementation<M> antares_layer_type;
//	typedef Layer_Options_Implementation<M> layer_options_type;
//	typedef Attributes_Panel_Implementation<M> attributes_panel_type;
//	typedef Control_Dialog_Implementation<M> control_dialog_type;
//	typedef Information_Page_Implementation<MasterType> information_page_type;
//
//	typedef Graphical_Network_Implementation<M> graphical_network_type;
//	typedef Graphical_Link_Implementation<M> graphical_link_type;
//	typedef Graphical_Intersection_Implementation<M> graphical_intersection_type;
//	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> vehicle_type;	
//	typedef Zone_Components::Implementations::Graphical_Zone_Implementation<M> zone_type;
//	//typedef Zone_Components::Implementations::Polaris_Zone_Implementation<M> zone_type;
//	typedef Zone_Components::Implementations::Graphical_Zone_Group_Implementation<M> graphical_zone_group_type;
//#else
//	typedef Vehicle_Components::Implementations::Polaris_Vehicle_Implementation<M> vehicle_type;
//	typedef Zone_Components::Implementations::Polaris_Zone_Implementation<M> zone_type;
//#endif
//
//	//typedef Network_Components::Implementations::Network_DB_Reader_Implementation<M> network_db_reader_type;
//	typedef true_type network_db_reader_type;
//
//	//==============================================================================================
//	// Network Types
//	typedef Scenario_Components::Implementations::Polaris_Scenario_Implementation<M> scenario_type;
//	typedef Network_Components::Implementations::Integrated_Polaris_Network_Implementation<M> network_type;
//	typedef Intersection_Components::Implementations::Polaris_Intersection_Implementation<M> intersection_type;
//	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<M> movement_type;
//	typedef Link_Components::Implementations::Polaris_Link_Implementation<M> link_type;
//	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<M> turn_movement_type;
//	typedef Routing_Components::Implementations::Routable_Network_Implementation<M> routable_network_type;
//	typedef Routing_Components::Implementations::Polaris_Routing_Implementation<M> routing_type;
//	typedef Routing_Components::Implementations::Polaris_Skim_Routing_Implementation<M> skim_routing_type;
//	typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<M> routable_intersection_type;
//	typedef Link_Components::Implementations::Routable_Link_Implementation<M> routable_link_type;
//	typedef Activity_Location_Components::Implementations::Polaris_Activity_Location_Implementation<M> activity_location_type;
//	typedef Traveler_Components::Implementations::Polaris_Traveler_Implementation<M> traveler_type;
//	typedef Intersection_Components::Implementations::Polaris_Inbound_Outbound_Movements_Implementation<M> inbound_outbound_movements_type;
//	typedef Intersection_Components::Implementations::Polaris_Outbound_Inbound_Movements_Implementation<M> outbound_inbound_movements_type;
//	typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<M> routable_inbound_outbound_movements_type;
//	typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<M> routable_outbound_inbound_movements_type;
//	typedef Intersection_Components::Implementations::Routable_Movement_Implementation<M> routable_movement_type;
//	//typedef Operation_Components::Implementations::Polaris_Operation_Implementation<M> operation_type;
//	typedef Intersection_Control_Components::Implementations::Polaris_Intersection_Control_Implementation<M> intersection_control_type;
//	typedef Intersection_Control_Components::Implementations::Polaris_Control_Plan_Implementation<M> control_plan_type;
//	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Implementation<M> phase_type;
//	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Movement_Implementation<M> phase_movement_type;
//	typedef Intersection_Control_Components::Implementations::Polaris_Approach_Implementation<M> approach_type;
//	typedef Plan_Components::Implementations::Polaris_Plan_Implementation<M> plan_type;
//	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Implementation<M> movement_plan_type;
//	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<M> trajectory_unit_type;
//	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
//	
//	// DEMAND AGENT Types
//	typedef Person_Components::Implementations::Person_Implementation<M> person_type;
//	typedef Person_Components::Implementations::CTRAMP_Person_Planner_Implementation<M, person_type> person_planner_type;
//	typedef Person_Components::Implementations::CTRAMP_Activity_Generator_Implementation<M, person_type> activity_generator_type;
//	typedef Person_Components::Implementations::ADAPTS_Person_Properties_Implementation<M,person_type> person_properties_type;
//	typedef RNG_Components::Implementations::RngStream_Implementation<M> RNG;
//	typedef Activity_Components::Implementations::ADAPTS_Activity_Plan_Implementation<M,person_type> activity_plan_type;
//	// destination choice types
//	typedef Person_Components::Implementations::CTRAMP_Destination_Chooser_Implementation<M,person_type> person_destination_chooser_type;
//	typedef Person_Components::Implementations::CTRAMP_Destination_Choice_Option<M,person_type> person_destination_choice_option_type;
//
//	
//	// POPULATION SYNTHESIS CLASSES
//	typedef PopSyn::Implementations::Synthesis_Zone_Implementation<M> zone;
//	typedef PopSyn::Implementations::Synthesis_Region_Implementation<M> region;
//	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<M> IPF_Solver_Settings;
//	typedef PopSyn::Implementations::ADAPTS_Population_Unit_Implementation<M> pop_unit;
//	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<M> popsyn_solver;
//};
//
//ostream* stream_ptr;
//
//
//
//
//int main(int argc,char** argv)
//{
//	cout << endl << "Time unit: " << sizeof(Basic_Units::Implementations::Time_Implementation<MasterType>);
//	cout << endl << "RNG: " << sizeof(MasterType::RNG);
//	cout << endl << "Pop unit: " << sizeof(MasterType::pop_unit);
//	cout << endl << "Pop region: " << sizeof(MasterType::region);
//	cout << endl << "Pop zone: " << sizeof(MasterType::zone);
//	cout << endl << "Person: " << sizeof(MasterType::person_type);
//	cout << endl << "Person - planner: " << sizeof(MasterType::person_planner_type);
//	cout << endl << "Person - properties: " << sizeof(MasterType::person_properties_type);
//	cout << endl << "Person - router: " << sizeof(MasterType::routing_type);
//	cout << endl << "Person - vehicle: " << sizeof(MasterType::vehicle_type);
//	cout << endl << "Movement plans: " << sizeof(MasterType::movement_plan_type);
//	cout << endl << "plan hashmaps: " << sizeof(hash_multimap<long,MasterType::activity_plan_type*>);
//	cout << endl << "plan vectors: " << sizeof(vector<MasterType::activity_plan_type*>);
//
//#ifdef ANTARES
//	START_UI(argc,argv, MasterType);
//#endif
//
//#pragma region COPY FROM NETWORKMODEL.CPP
//	Network_Components::Types::Network_IO_Maps network_io_maps;
//	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::File_Network,network_models::network_information::network_data_information::NetworkData&> Net_IO_Type;
//
//	//reading from files
//	network_models::network_information::scenario_data_information::ScenarioData scenario_data;
//	network_models::network_information::network_data_information::NetworkData network_data;
//	network_models::network_information::demand_data_information::DemandData demand_data;
//	network_models::network_information::operation_data_information::OperationData operation_data;
//	network_models::initialization(scenario_data,network_data,demand_data,operation_data);
//	scenario_data.input_dir_name = "";
//	scenario_data.output_dir_name = "";
//	network_models::read_data(scenario_data,network_data,demand_data,operation_data);
//
//	//converting between k and p
//	network_models::network_information::scenario_data_information::ScenarioData scenario_data_for_output;
//	network_models::network_information::network_data_information::NetworkData network_data_for_output;
//	network_models::network_information::demand_data_information::DemandData demand_data_for_output;
//	network_models::network_information::operation_data_information::OperationData operation_data_for_output;
//	network_models::initialization(scenario_data_for_output,network_data_for_output,demand_data_for_output,operation_data_for_output);
//
//	cout << "reading input data..." <<endl;	
//	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
//	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
//	_global_scenario = scenario;
//	scenario->read_scenario_data<Scenario_Components::Types::File_Scenario>(scenario_data);
//	//scenario->write_scenario_data<NULLTYPE>(scenario_data_for_output);
//
//	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
//	_Network_Interface* network = (_Network_Interface*)Allocate<typename MasterType::network_type>();
//	_global_network = network;
//	network->scenario_reference<_Scenario_Interface*>(scenario);
//	network->read_network_data<Net_IO_Type>(network_data);
//	//network->write_network_data<NULLTYPE>(network_data_for_output);
//
//
//	//define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
//	//_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
//	//operation->network_reference<_Network_Interface*>(network);
//	//operation->read_operation_data<Operation_Components::Types::File_Operation>(scenario_data, network_data, operation_data);
//	//operation->write_operation_data<NULLTYPE>(network_data_for_output, operation_data_for_output);
//	
//	//output data
//	//network_models::write_data("",scenario_data_for_output,demand_data_for_output,network_data_for_output, operation_data);
//
//
//	network->simulation_initialize<NULLTYPE>();
//	cout << "world started..." << endl;
//	////initialize network agents
//	
//	define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
//	_Links_Container_Interface::iterator links_itr;
//
//	for(links_itr=network->links_container<_Links_Container_Interface&>().begin();
//		links_itr!=network->links_container<_Links_Container_Interface&>().end();
//		links_itr++)
//	{
//		((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
//	}
//
//	define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, _Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Prototype, NULLTYPE);
//	_Intersections_Container_Interface::iterator intersections_itr;
//
//	for(intersections_itr=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().begin();
//		intersections_itr!=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().end();
//		intersections_itr++)
//	{
//
//		((_Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();		
//	}
//
//#pragma endregion
//
//	//==================================================================================================================================
//	// Set up graphical display
//	//----------------------------------------------------------------------------------------------------------------------------------
//	#ifdef ANTARES
//	define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, NULLTYPE);
//	_Zones_Container_Interface::iterator zone_itr;
//	_Zones_Container_Interface* zone_list = network->zones_container<_Zones_Container_Interface*>();
//	typedef Canvas<MasterType::canvas_type,MasterType::graphical_zone_group_type> canvas_itf;
//	canvas_itf* canvas_ptr = (canvas_itf*) canvas;
//	define_component_interface(graphical_network_interface,typename canvas_itf::get_type_of(graphical_network),Network_Components::Prototypes::Network_Prototype,NULLTYPE);
//	//--------------------------------------------------------------------------------------------
//	// Graphical zone group display - integrate to graphical network when database is fixed
//	typedef Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,NULLTYPE> zone_group_interface;
//	zone_group_interface* _graphical_zone_group = (zone_group_interface*) Allocate<MasterType::graphical_zone_group_type>();	
//	_graphical_zone_group->canvas<canvas_itf*>( (canvas_itf*) canvas );
//	// initialize zone static reference to the graphical zone group
//	MasterType::zone_type::_graphical_zone_group=(Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,MasterType::zone_type>*)_graphical_zone_group;
//	_graphical_zone_group->configure_zones_layer<NULLTYPE>();
//	// get offsets from graphical network
//	_graphical_zone_group->input_offset<Point_2D<MasterType>*>(canvas_ptr->graphical_network<graphical_network_interface*>()->input_offset<Point_2D<MasterType>*>());
//	#endif
//
//
//	//==================================================================================================================================
//	// Network Skimming stuff
//	//----------------------------------------------------------------------------------------------------------------------------------
//	define_component_interface(_network_skim_itf, _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,NULLTYPE);
//	_network_skim_itf* skimmer = (_network_skim_itf*)Allocate<_Network_Interface::get_type_of(skimming_faculty)>();
//
//	skimmer->Initialize<_Network_Interface*>(network);
//	network->skimming_faculty<_network_skim_itf*>(skimmer);
//	
//	//==================================================================================================================================
//	// POPSYN stuff
//	//----------------------------------------------------------------------------------------------------------------------------------
//	ofstream out;
//	out.open("full_population_chicag.csv",ios_base::out);
//	ofstream marg_out;
//	marg_out.open("marginals_and_distributions_chicago.csv",ios_base::out);
//
//	// IPF Solver Settings
//	define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
//	solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
//	// Solver settings - IPF tolerance, Percentage of population to synthesis, maximum ipf and selection iterations
//	solver->Initialize<Target_Type<NULLTYPE,void,double,int>>(0.05,0.5,100);
//
//	define_component_interface(popsyn_itf,MasterType::popsyn_solver,PopSyn::Prototypes::Population_Synthesizer_Prototype,NULLTYPE);
//	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::popsyn_solver>();
//	popsyn->write_output_flag<bool>(true);
//	popsyn->linker_file_path<string>(string("linker_file.txt"));
//	popsyn->Solution_Settings<solver_itf*>(solver);
//	popsyn->Output_Stream<ostream&>(out);
//	popsyn->Marginal_Output_Stream<ostream&>(marg_out);
//	popsyn->network_reference<_Network_Interface*>(network);
//	popsyn->scenario_reference<_Scenario_Interface*>(scenario);
//	popsyn->Initialize<NULLTYPE>();
//	//----------------------------------------------------------------------------------------------------------------------------------
//
//	//==================================================================================================================================
//	// Logging of activity generation / scheduling outputs
//	//----------------------------------------------------------------------------------------------------------------------------------
//	stringstream logfilename;
//	MasterType::person_planner_type::_write_activity_files = false;
//	for (int i = 0; i < _num_threads; ++i)
//	{
//		logfilename.str("");
//		logfilename << "generated_acts_" << i << ".csv";
//		MasterType::person_planner_type::logs[i].open(logfilename.str());
//		MasterType::person_planner_type::logs[i] << "PERID,DEPART,ORIG,DEST,EST_TTIME"<<endl;
//	}
//	//----------------------------------------------------------------------------------------------------------------------------------
//	
//	try
//	{
//	START();
//	}
//	catch (std::exception ex)
//	{
//		cout << ex.what();
//	}
//
//	cout << "Finished!" << endl;
//	system("PAUSE");
//}
//#endif

