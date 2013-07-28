//#define SHOW_WARNINGS

#include "Polaris_PCH.h"


// SET THE DEBUG VERSION TO SIMULATE ONLY ONE AGENT
#ifdef _DEBUG
//#define DEBUG_1
#endif

#ifdef DBIO
#include "Application_Includes.h"


struct MasterType
{
	typedef MasterType M;

	//==============================================================================================
	#pragma region Network Types
	//----------------------------------------------------------------------------------------------
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
	typedef Splash_Panel_Implementation<M> splash_panel_type;

	typedef Antares_Network_Implementation<M> network_type;
	typedef Antares_Link_Implementation<M> link_type;
	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> vehicle_type;
	//typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<M> basic_vehicle_type;
	//typedef Vehicle_Components::Implementations::Polaris_Base_Vehicle_Implementation<M> vehicle_type;
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

	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Implementation<M> basic_movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Polaris_Integrated_Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Record_Implementation<M> movement_plan_record_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	typedef Network_Skimming_Components::Implementations::Mode_Skim_Table_Implementation<M> network_mode_skim_type;
	#pragma endregion
	//----------------------------------------------------------------------------------------------


	//==============================================================================================
	#pragma region DEMAND Types
	//----------------------------------------------------------------------------------------------
	typedef Demand_Components::Implementations::Polaris_Demand_Implementation<MasterType> demand_type;

	typedef Person_Components::Implementations::Person_Implementation<M> person_type;

	typedef Person_Components::Implementations::POLARIS_Person_Planner_Implementation<M, person_type> person_planner_type;
	typedef Person_Components::Implementations::Person_Mover_Implementation<M, person_type> person_mover_type;
	typedef Person_Components::Implementations::General_Person_Scheduler_Implementation<M, person_type> person_scheduler_type;
	typedef Person_Components::Implementations::General_Person_Perception_Implementation<M, person_type> person_perception_type;
	typedef Person_Components::Implementations::CTRAMP_Activity_Generator_Implementation<M, person_type> activity_generator_type;
	typedef Person_Components::Implementations::ADAPTS_Person_Properties_Implementation<M,person_type> person_properties_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<M> person_static_properties_type;
	
	
	typedef RNG_Components::Implementations::RngStream_Implementation<M> RNG;

	typedef Activity_Components::Implementations::Basic_Activity_Plan_Implementation<M,person_type> activity_type;
	typedef Activity_Components::Implementations::ADAPTS_Activity_Plan_Implementation<M,person_type> activity_plan_type;
	typedef Activity_Components::Implementations::ADAPTS_Routine_Activity_Plan_Implementation<M,person_type> routine_activity_plan_type;
	typedef Activity_Components::Implementations::ADAPTS_At_Home_Activity_Plan_Implementation<M,person_type> at_home_activity_plan_type;
	typedef Activity_Components::Implementations::Activity_Record<M,person_type> activity_record_type;

	// Choice model types
	typedef Person_Components::Implementations::Activity_Timing_Chooser_Implementation<M,person_type> activity_timing_chooser_type;
	typedef Person_Components::Implementations::CTRAMP_Destination_Chooser_Implementation<M,person_type> person_destination_chooser_type;
	typedef Person_Components::Implementations::CTRAMP_Destination_Choice_Option<M,person_type> person_destination_choice_option_type;
	
	#ifdef ANTARES
		typedef Person_Components::Implementations::Antares_Person_Data_Logger_Implementation<M> person_data_logger_type;
	#else
		typedef Person_Components::Implementations::Person_Data_Logger_Implementation<M> person_data_logger_type;
	#endif
	
	// POPULATION SYNTHESIS CLASSES
	typedef PopSyn::Implementations::Synthesis_Zone_Implementation<M> zone;
	typedef PopSyn::Implementations::Synthesis_Region_Implementation<M> region;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<M> IPF_Solver_Settings;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<M> popsyn_solver;
	#pragma endregion
	//----------------------------------------------------------------------------------------------


	//==============================================================================================
	#pragma region TMC Types
	//----------------------------------------------------------------------------------------------
	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
	#ifdef ANTARES
	typedef Network_Event_Components::Implementations::Antares_Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Antares_Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Antares_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Speed_Sign<MasterType> variable_speed_sign_type;

	typedef Sensor_Components::Implementations::Antares_Link_Sensor<MasterType> link_sensor_type;

	typedef Ramp_Metering_Components::Implementations::Antares_Ramp_Metering_Implementation<M> ramp_metering_type;
	#else
	typedef Network_Event_Components::Implementations::Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Link_Control_Components::Implementations::Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Tow_Truck_Implementation<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Highway_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Speed_Sign<MasterType> variable_speed_sign_type;

	typedef Sensor_Components::Implementations::Link_Sensor<MasterType> link_sensor_type;

	typedef Ramp_Metering_Components::Implementations::Polaris_Ramp_Metering_Implementation<M> ramp_metering_type;
	#endif

	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;
	#pragma endregion
	//----------------------------------------------------------------------------------------------
};


int main(int argc,char** argv)
{
	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	char* scenario_filename = "scenario.json";
	if (argc >= 2) scenario_filename = argv[1];

	
	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region Copy from NetworkModel.cpp
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;


	network_models::network_information::scenario_data_information::ScenarioData scenario_data_for_output;
	network_models::network_information::network_data_information::NetworkData network_data_for_output;
	network_models::network_information::demand_data_information::DemandData demand_data_for_output;
	network_models::network_information::operation_data_information::OperationData operation_data_for_output;
	network_models::initialization(scenario_data_for_output,network_data_for_output,demand_data_for_output,operation_data_for_output);


	//data
	cout << "allocating data structures..." <<endl;	
	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
	
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<MasterType::scenario_type>();
	_global_scenario = scenario;

	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
	
	_Network_Interface* network=(_Network_Interface*)Allocate<MasterType::network_type>();
	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);
	
	////data input
	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename);
	cout << "converting scenario data..." << endl;
	scenario->write_scenario_data<NULLTYPE>(scenario_data_for_output);
	//network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);
	//cout<<"writing scenario data..."<<endl;
	//network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);	

	cout << "reading network data..." <<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);

	cout << "converting network data..." << endl;
	network->write_network_data<Target_Type<NULLTYPE,void,network_models::network_information::network_data_information::NetworkData&>>(network_data_for_output);
	//network_models::network_information::network_data_information::write_network_data("", network_data_for_output);
	//cout<<"writing network data..."<<endl;
	//network_models::network_information::network_data_information::write_network_data(output_dir_name,network_data_for_output);

	
	cout << "initializing simulation..." <<endl;	
	network->simulation_initialize<NULLTYPE>();



	define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	if (scenario->intersection_control_flag<int>() == 1) {
		cout <<"reading intersection control data..." << endl;
		operation->read_intersection_control_data<Net_IO_Type>(network_io_maps);
	}
	if (scenario->ramp_metering_flag<bool>() == true) {
		cout <<"reading ramp metering data..." << endl;
		operation->read_ramp_metering_data<Net_IO_Type>(network_io_maps);
	}
	cout <<"converting operation data..." << endl;
	operation->write_operation_data<NULLTYPE>(network_data_for_output, operation_data_for_output);
	cout<<"writing operation data..."<<endl;
	network_models::network_information::operation_data_information::write_operation_data(scenario->output_dir_name<string>(),scenario_data_for_output,operation_data_for_output,network_data_for_output);
	
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

	if(scenario->use_network_events<bool>())
	{
		cout << "setting up network events" << endl;
		define_component_interface(_Network_Event_Manager_Interface, MasterType::network_event_manager_type, Network_Event_Manager, NULLTYPE);
		_Network_Event_Manager_Interface* net_event_manager=(_Network_Event_Manager_Interface*)Allocate<MasterType::network_event_manager_type>();
		network->network_event_manager<_Network_Event_Manager_Interface*>(net_event_manager);
		net_event_manager->Initialize<NT>();

		if (scenario->use_tmc<bool>())
		{
			cout << "setting up tmc" << endl;

			typedef Traffic_Management_Center<MasterType::traffic_management_center_type> TMC_Interface;

			TMC_Interface* tmc = (TMC_Interface*) Allocate< MasterType::traffic_management_center_type >();
			tmc->network_event_manager<_Network_Event_Manager_Interface*>(net_event_manager);
			tmc->Initialize<NT>();
		}
	}



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

	for(intersections_itr=network->intersections_container<MasterType::network_type::intersections_container_type&>().begin();
		intersections_itr!=network->intersections_container<MasterType::network_type::intersections_container_type&>().end();
		intersections_itr++)
	{
		((_Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();
	}

	cout << "initializing ramp metering agents..." <<endl;
	define_container_and_value_interface(_Ramp_Metering_Container_Interface, _Ramp_Metering_Interface, _Network_Interface::get_type_of(ramp_metering_container), Random_Access_Sequence_Prototype, Ramp_Metering_Prototype, NULLTYPE);
	_Ramp_Metering_Container_Interface::iterator ramp_metering_itr;

	Ramp_Metering_Prototype<MasterType::ramp_metering_type>::Initialize_Type<NT>();
	for(ramp_metering_itr=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().begin();
		ramp_metering_itr!=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().end();
		ramp_metering_itr++)
	{
		((_Ramp_Metering_Interface*)(*ramp_metering_itr))->Initialize<NULLTYPE>();
	}

	cout << "starting sim..." <<endl;
	#pragma endregion

	
	//==================================================================================================================================
	// EXTERNAL Demand
	//----------------------------------------------------------------------------------------------------------------------------------
	if (scenario->read_demand_from_database<bool>())
	{
		define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
		_Demand_Interface* demand = (_Demand_Interface*)Allocate<MasterType::demand_type>();
		demand->scenario_reference<_Scenario_Interface*>(scenario);
		demand->network_reference<_Network_Interface*>(network);
		cout << "reading external demand data..." <<endl;
		demand->read_demand_data<Net_IO_Type>(network_io_maps);
	}

	
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
	skimmer->read_input<bool>(scenario->read_skim_tables<bool>());
	if (skimmer->read_input<bool>())
	{
		if (!skimmer->input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: input binary skim file '" << scenario->input_skim_file_path_name<string>() << "' could not be opened.");
	}
	skimmer->write_output<bool>(scenario->write_skim_tables<bool>());	
	if (skimmer->write_output<bool>())
	{
		if (!skimmer->output_file<File_IO::Binary_File_Writer&>().Open(scenario->output_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: output binary skim file '" << scenario->output_skim_file_path_name<string>() << "' could not be opened.");
	}
	skimmer->Initialize<_Network_Interface*>(network);
	network->skimming_faculty<_network_skim_itf*>(skimmer);


	//==================================================================================================================================
	// Destination choice model - set parameters
	//----------------------------------------------------------------------------------------------------------------------------------
	MasterType::person_destination_chooser_type::_choice_set_size = 50;
	MasterType::person_destination_choice_option_type::_B_TTIME = -0.12;
	MasterType::person_destination_choice_option_type::_B_EMPLOYMENT = 0.1;
	MasterType::person_destination_choice_option_type::_B_POPULATION = 0.025;
	// Initialize start time model
	MasterType::activity_timing_chooser_type::static_initializer("start_time_duration_data.txt");	
	// Initialize person properties with average activity frequency and duration
	MasterType::person_properties_type::Static_Initializer();

	//==================================================================================================================================
	// POPSYN stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	define_component_interface(popsyn_itf,MasterType::popsyn_solver,PopSyn::Prototypes::Population_Synthesizer_Prototype,NULLTYPE);
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::popsyn_solver>();
	popsyn->Initialize<Target_Type<NT,NT,_Network_Interface*, _Scenario_Interface*> >(network,scenario);
	//----------------------------------------------------------------------------------------------------------------------------------

	//==================================================================================================================================
	// Logging of activity generation / scheduling outputs
	//----------------------------------------------------------------------------------------------------------------------------------
	define_component_interface(_Logger_Interface, MasterType::person_data_logger_type, Person_Components::Prototypes::Person_Data_Logger, NULLTYPE);	
	_Logger_Interface* logger=(_Logger_Interface*)Allocate<MasterType::person_data_logger_type>();
	logger->Initialize<NT>();
	_global_person_logger = logger;

	
	if (scenario->use_network_events<bool>()) MasterType::link_type::subscribe_events();


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


void output_object_sizes()
{
	cout << endl <<"network_type size = "<<sizeof(MasterType::network_type);
	cout << endl <<"link_type size = "<<sizeof(MasterType::link_type);
	cout << endl <<"intersection_type size = "<<sizeof(MasterType::intersection_type);
	cout << endl <<"vehicle_type size = "<<sizeof(MasterType::vehicle_type);
	cout << endl <<"zone_type size = "<<sizeof(MasterType::zone_type);
	cout << endl <<"scenario_type size = "<<sizeof(MasterType::scenario_type);
	cout << endl <<"network_db_reader_type size = "<<sizeof(MasterType::network_db_reader_type);
	cout << endl <<"movement_type size = "<<sizeof(MasterType::movement_type);
	cout << endl <<"turn_movement_type size = "<<sizeof(MasterType::turn_movement_type);
	cout << endl <<"routable_network_type size = "<<sizeof(MasterType::routable_network_type);
	cout << endl <<"routing_type size = "<<sizeof(MasterType::routing_type);
	cout << endl <<"skim_routing_type size = "<<sizeof(MasterType::skim_routing_type);
	cout << endl <<"routable_intersection_type size = "<<sizeof(MasterType::routable_intersection_type);
	cout << endl <<"routable_link_type size = "<<sizeof(MasterType::routable_link_type);
	cout << endl <<"activity_location_type size = "<<sizeof(MasterType::activity_location_type);
	cout << endl <<"traveler_type size = "<<sizeof(MasterType::traveler_type);
	cout << endl <<"switch_decision_data_type size = "<<sizeof(MasterType::switch_decision_data_type);
	cout << endl <<"inbound_outbound_movements_type size = "<<sizeof(MasterType::inbound_outbound_movements_type);
	cout << endl <<"outbound_inbound_movements_type size = "<<sizeof(MasterType::outbound_inbound_movements_type);
	cout << endl <<"routable_inbound_outbound_movements_type size = "<<sizeof(MasterType::routable_inbound_outbound_movements_type);
	cout << endl <<"routable_outbound_inbound_movements_type size = "<<sizeof(MasterType::routable_outbound_inbound_movements_type);
	cout << endl <<"routable_movement_type size = "<<sizeof(MasterType::routable_movement_type);
	cout << endl <<"operation_type size = "<<sizeof(MasterType::operation_type);
	cout << endl <<"intersection_control_type size = "<<sizeof(MasterType::intersection_control_type);
	cout << endl <<"control_plan_type size = "<<sizeof(MasterType::control_plan_type);
	cout << endl <<"phase_type size = "<<sizeof(MasterType::phase_type);
	cout << endl <<"phase_movement_type size = "<<sizeof(MasterType::phase_movement_type);
	cout << endl <<"approach_type size = "<<sizeof(MasterType::approach_type);
	cout << endl <<"plan_type size = "<<sizeof(MasterType::plan_type);
	cout << endl <<"movement_plan_type size = "<<sizeof(MasterType::movement_plan_type);
	cout << endl <<"movement_plan_record_type size = "<<sizeof(MasterType::movement_plan_record_type);
	cout << endl <<"trajectory_unit_type size = "<<sizeof(MasterType::trajectory_unit_type);
	cout << endl <<"network_skim_type size = "<<sizeof(MasterType::network_skim_type);
	cout << endl <<"demand_type size = "<<sizeof(MasterType::demand_type);
	cout << endl <<"person_type size = "<<sizeof(MasterType::person_type);
	cout << endl <<"person_planner_type size = "<<sizeof(MasterType::person_planner_type);
	cout << endl <<"person_mover_type size = "<<sizeof(MasterType::person_mover_type);
	cout << endl <<"person_scheduler_type size = "<<sizeof(MasterType::person_scheduler_type);
	cout << endl <<"person_perception_type size = "<<sizeof(MasterType::person_perception_type);
	cout << endl <<"activity_generator_type size = "<<sizeof(MasterType::activity_generator_type);
	cout << endl <<"person_properties_type size = "<<sizeof(MasterType::person_properties_type);
	cout << endl <<"person_static_properties_type size = "<<sizeof(MasterType::person_static_properties_type);
	cout << endl <<"person_data_logger_type size = "<<sizeof(MasterType::person_data_logger_type);
	cout << endl <<"RNG size = "<<sizeof(MasterType::RNG);
	cout << endl <<"activity_type size = "<<sizeof(MasterType::activity_type);
	cout << endl <<"activity_plan_type size = "<<sizeof(MasterType::activity_plan_type);
	cout << endl <<"routine_activity_plan_type size = "<<sizeof(MasterType::routine_activity_plan_type);
	cout << endl <<"activity_record_type size = "<<sizeof(MasterType::activity_record_type);
	cout << endl <<"activity_timing_chooser_type size = "<<sizeof(MasterType::activity_timing_chooser_type);
	cout << endl <<"person_destination_chooser_type size = "<<sizeof(MasterType::person_destination_chooser_type);
	cout << endl <<"person_destination_choice_option_type size = "<<sizeof(MasterType::person_destination_choice_option_type);
	cout << endl <<"zone size = "<<sizeof(MasterType::zone);
	cout << endl <<"region size = "<<sizeof(MasterType::region);
	cout << endl <<"IPF_Solver_Settings size = "<<sizeof(MasterType::IPF_Solver_Settings);
	cout << endl <<"popsyn_solver size = "<<sizeof(MasterType::popsyn_solver);
	cout << endl <<"traffic_management_center_type size = "<<sizeof(MasterType::traffic_management_center_type);
	cout << endl <<"weather_network_event_type size = "<<sizeof(MasterType::weather_network_event_type);
	cout << endl <<"accident_network_event_type size = "<<sizeof(MasterType::accident_network_event_type);
	cout << endl <<"congestion_network_event_type size = "<<sizeof(MasterType::congestion_network_event_type);
	cout << endl <<"lane_closure_network_event_type size = "<<sizeof(MasterType::lane_closure_network_event_type);
	cout << endl <<"link_control_type size = "<<sizeof(MasterType::link_control_type);
	cout << endl <<"depot_type size = "<<sizeof(MasterType::depot_type);
	cout << endl <<"advisory_radio_type size = "<<sizeof(MasterType::advisory_radio_type);
	cout << endl <<"variable_word_sign_type size = "<<sizeof(MasterType::variable_word_sign_type);
	cout << endl <<"variable_speed_sign_type size = "<<sizeof(MasterType::variable_speed_sign_type);
	cout << endl <<"link_sensor_type size = "<<sizeof(MasterType::link_sensor_type);
	cout << endl <<"base_network_event_type size = "<<sizeof(MasterType::base_network_event_type);
	cout << endl <<"network_event_manager_type size = "<<sizeof(MasterType::network_event_manager_type);
}