#define StatewideModelApplication


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

	//typedef Zone_Components::Implementations::Graphical_Zone_Implementation<M> zone_type;
	typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;

	typedef Antares_Intersection_Implementation<M> intersection_type;
	typedef Zone_Components::Implementations::Graphical_Zone_Group_Implementation<M> graphical_zone_group_type;
	#else
	typedef Network_Components::Implementations::Integrated_Network_Implementation<M> network_type;
	typedef Link_Components::Implementations::Link_Implementation<M> link_type;
	typedef Intersection_Components::Implementations::Intersection_Implementation<M> intersection_type;
	typedef Vehicle_Components::Implementations::Vehicle_Implementation<M> vehicle_type;
	typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;
	#endif

	typedef Scenario_Components::Implementations::Scenario_Implementation<M> scenario_type;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<M> network_db_reader_type;
	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> movement_type;

	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> turn_movement_type;
	typedef Routing_Components::Implementations::Routable_Network_Implementation<M> routable_network_type;
	typedef Routing_Components::Implementations::Routing_Implementation<M> routing_type;
	typedef Routing_Components::Implementations::Skim_Routing_Implementation<M> skim_routing_type;
	//typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<M> routable_intersection_type;
	//typedef Link_Components::Implementations::Routable_Link_Implementation<M> routable_link_type;
	typedef Activity_Location_Components::Implementations::Activity_Location_Implementation<M> activity_location_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> traveler_type;
	typedef Vehicle_Components::Implementations::Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;
	typedef Intersection_Components::Implementations::Inbound_Outbound_Movements_Implementation<M> inbound_outbound_movements_type;
	typedef Intersection_Components::Implementations::Outbound_Inbound_Movements_Implementation<M> outbound_inbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<M> routable_inbound_outbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<M> routable_outbound_inbound_movements_type;
	//typedef Intersection_Components::Implementations::Routable_Movement_Implementation<M> routable_movement_type;
	typedef Operation_Components::Implementations::Operation_Implementation<M> operation_type;
	typedef Intersection_Control_Components::Implementations::Intersection_Control_Implementation<M> intersection_control_type;
	typedef Intersection_Control_Components::Implementations::Control_Plan_Implementation<M> control_plan_type;
	typedef Intersection_Control_Components::Implementations::Phase_Implementation<M> phase_type;
	typedef Intersection_Control_Components::Implementations::Phase_Movement_Implementation<M> phase_movement_type;
	typedef Intersection_Control_Components::Implementations::Approach_Implementation<M> approach_type;
	
	typedef Analyze_Link_Group_Components::Implementations::Analyze_Link_Group_Implementation<MasterType> analyze_link_group_type;

	typedef Plan_Components::Implementations::Plan_Implementation<M> plan_type;

	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> basic_movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Integrated_Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Record_Implementation<M> movement_plan_record_type;

	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	typedef Network_Skimming_Components::Implementations::LOS_Value_Implementation<M> los_value_type;
	typedef Network_Skimming_Components::Implementations::LOS_Time_Invariant_Value_Implementation<M> los_invariant_value_type;
	//typedef Network_Skimming_Components::Implementations::Mode_Skim_Table_Implementation<M> network_mode_skim_type;
	#pragma endregion
	//----------------------------------------------------------------------------------------------


	//==============================================================================================
	#pragma region DEMAND Types
	//----------------------------------------------------------------------------------------------
	typedef Demand_Components::Implementations::Demand_Implementation<MasterType> demand_type;

	typedef Person_Components::Implementations::Person_Implementation<M> person_type;
	typedef Household_Components::Implementations::Household_Implementation<M> household_type;

	typedef Person_Components::Implementations::General_Person_Planner_Implementation<M> person_planner_type;
	typedef Person_Components::Implementations::Person_Mover_Implementation<M> person_mover_type;
	typedef Person_Components::Implementations::General_Person_Scheduler_Implementation<M> person_scheduler_type;
	typedef Person_Components::Implementations::General_Person_Perception_Implementation<M> person_perception_type;
	typedef Person_Components::Implementations::Simple_Activity_Generator_Implementation<M> activity_generator_type;
	typedef Person_Components::Implementations::ADAPTS_Person_Properties_Implementation<M> person_properties_type;
	typedef Person_Components::Implementations::ACS_Person_Static_Properties_Implementation<M> person_static_properties_type;
	typedef Household_Components::Implementations::ADAPTS_Household_Properties_Implementation<M> household_properties_type;
	typedef Household_Components::Implementations::ACS_Household_Static_Properties_Implementation<M> household_static_properties_type;
	
	//typedef RNG_Components::Implementations::Uniform_RNG<M> rng_type;

	typedef Activity_Components::Implementations::Basic_Activity_Plan_Implementation<M> activity_type;
	typedef Activity_Components::Implementations::ADAPTS_Activity_Plan_Implementation<M> activity_plan_type;
	typedef Activity_Components::Implementations::ADAPTS_Routine_Activity_Plan_Implementation<M> routine_activity_plan_type;
	typedef Activity_Components::Implementations::ADAPTS_At_Home_Activity_Plan_Implementation<M> at_home_activity_plan_type;
	typedef Activity_Components::Implementations::Activity_Record<M> activity_record_type;

	// Choice model types
	typedef Person_Components::Implementations::Activity_Timing_Chooser_Implementation<M> activity_timing_chooser_type;
	typedef Person_Components::Implementations::ADAPTS_Destination_Chooser_Implementation<M> person_destination_chooser_type;
	typedef Person_Components::Implementations::ADAPTS_Destination_Choice_Option<M> person_destination_choice_option_type;
	typedef Person_Components::Implementations::Mode_Chooser_Implementation<M> person_mode_chooser_type;
	typedef Person_Components::Implementations::Mode_Choice_Option<M> mode_choice_option_type;

	typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MT> mnl_model_type;
	
	#ifdef ANTARES
		//typedef Person_Components::Implementations::Antares_Person_Data_Logger_Implementation<M> person_data_logger_type;
		typedef Person_Components::Implementations::Person_Data_Logger_Implementation<M> person_data_logger_type;
	#else
		typedef Person_Components::Implementations::Person_Data_Logger_Implementation<M> person_data_logger_type;
	#endif
	
	// POPULATION SYNTHESIS CLASSES
	typedef PopSyn::Implementations::Synthesis_Zone_Implementation_Full<MasterType> synthesis_zone_type;
	typedef PopSyn::Implementations::Synthesis_Region_Implementation_Full<MasterType> synthesis_region_type;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<MasterType> solver_settings_type;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<MasterType> population_synthesis_type;
	typedef PopSyn::Implementations::Popsyn_File_Linker_Implementation<MasterType> popsyn_file_linker_type;
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
	typedef Buildings_Components::Implementations::Antares_Buildings_Implementation<M> buildings_type;
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

	typedef Ramp_Metering_Components::Implementations::Ramp_Metering_Implementation<M> ramp_metering_type;
	#endif

	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;


	typedef Routable_Agent_Implementation<MasterType> routable_agent_type;
	typedef Tree_Agent_Implementation<MasterType> tree_agent_type;
	typedef Graph_Implementation<MasterType, NTL, Base_Edge_A_Star<MasterType>> base_graph_type;
	typedef Graph_Pool_Implementation<MasterType, NTL, base_graph_type> graph_pool_type;
	typedef Edge_Implementation<Routing_Components::Types::static_attributes<MasterType>> static_edge_type;
	typedef Graph_Implementation<MasterType, NTL, static_edge_type> static_graph_type;
	typedef Routing_Components::Types::static_to_static static_to_static_type;
	typedef Custom_Connection_Group<MasterType, static_graph_type, static_graph_type, static_to_static_type> static_to_static_connection_type;
	
	typedef Edge_Implementation<Routing_Components::Types::time_dependent_attributes<MasterType>> time_dependent_edge_type;
	typedef Graph_Implementation<MasterType, NTL, time_dependent_edge_type> time_dependent_graph_type;
	typedef Routing_Components::Types::time_dependent_to_time_dependent time_dependent_to_time_dependent_type;
	typedef Custom_Connection_Group<MasterType, time_dependent_graph_type, time_dependent_graph_type, time_dependent_to_time_dependent_type> time_dependent_to_time_dependent_connection_type;

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
	int threads = 1;
	if (argc >= 3) threads = std::max(atoi(argv[2]),threads);
	int people_hint = 0;
	if (argc >= 4) people_hint = std::max(atoi(argv[3]),threads);

	
	//==================================================================================================================================
	// Allocation hints block
	//----------------------------------------------------------------------------------------------------------------------------------
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(86400, threads);
	INITIALIZE_SIMULATION(cfg);

	if (people_hint > 0)
	{
		Average_Execution_Objects_Hint<MasterType::activity_plan_type>(people_hint*1.5);
		Average_Execution_Objects_Hint<MasterType::routine_activity_plan_type>(people_hint);
		Average_Execution_Objects_Hint<MasterType::at_home_activity_plan_type>(people_hint*2.0);
		Average_Execution_Objects_Hint<MasterType::routing_type>(people_hint);
		Average_Execution_Objects_Hint<MasterType::person_type>(people_hint);
		Average_Execution_Objects_Hint<MasterType::person_mover_type>(people_hint);
	}

	//polaris::_type_counter.resize(__all_components->size(),threads+1,0);

	//Average_Execution_Objects_Hint<MasterType::person_type>(9000000);

	
	

	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region New network_model.cpp stuff
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	output_object_sizes();

	//===============
	// OUTPUT OPTIONS
	//----------------
	ofstream log_file("signal_log3.txt");
	ostream output_stream(log_file.rdbuf());
	//stream_ptr = &output_stream;	

	string output_dir_name = "";

	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();

	cout << "allocating data structures..." <<endl;	


	typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	typedef Network<typename MasterType::network_type> _Network_Interface;
	_Network_Interface* network=(_Network_Interface*)Allocate<typename MasterType::network_type>();

	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);

	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename);

	typedef MasterType::network_type::link_dbid_dir_to_ptr_map_type link_dbid_dir_to_ptr_map_type;

	link_dbid_dir_to_ptr_map_type* link_dbid_dir_to_ptr_map = network->template link_dbid_dir_to_ptr_map<link_dbid_dir_to_ptr_map_type*>();

	cout << "reading network data..." <<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);
	typedef Operation<MasterType::operation_type> _Operation_Interface;
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	if (scenario->intersection_control_flag<int>() == 1) {
		cout <<"reading intersection control data..." << endl;
		operation->read_intersection_control_data<Net_IO_Type>(network_io_maps);
	}
	//cout << "initializing simulation..." <<endl;	
	network->simulation_initialize<NULLTYPE>();

	//define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
	//typedef Demand<MasterType::demand_type> _Demand_Interface;
	//_Demand_Interface* demand = (_Demand_Interface*)Allocate<typename MasterType::demand_type>();
	//demand->scenario_reference<_Scenario_Interface*>(scenario);
	//demand->network_reference<_Network_Interface*>(network);
	//cout << "reading demand data..." <<endl;
	//demand->read_demand_data<Net_IO_Type>(network_io_maps);

	//define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);

	if (scenario->ramp_metering_flag<bool>() == true) {
		cout <<"reading ramp metering data..." << endl;
		operation->read_ramp_metering_data<Net_IO_Type>(network_io_maps);
	}

	

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
		//define_component_interface(_Network_Event_Manager_Interface, typename MasterType::network_event_manager_type, Network_Event_Manager, NULLTYPE);
		typedef Network_Event_Manager<MasterType::network_event_manager_type> _Network_Event_Manager_Interface;
		_Network_Event_Manager_Interface* net_event_manager=(_Network_Event_Manager_Interface*)Allocate<typename MasterType::network_event_manager_type>();
		network->network_event_manager<_Network_Event_Manager_Interface*>(net_event_manager);
		net_event_manager->Initialize<NT>();

		if (scenario->use_tmc<bool>())
		{
			typedef Traffic_Management_Center<MasterType::traffic_management_center_type> TMC_Interface;

			TMC_Interface* tmc = (TMC_Interface*) Allocate< MasterType::traffic_management_center_type >();
			tmc->network_event_manager<_Network_Event_Manager_Interface*>(net_event_manager);
			tmc->Initialize<NT>();
		}
	}

	////initialize network agents
	cout << "initializing link agents..." <<endl;

	//define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
	
	typedef Link<remove_pointer<_Network_Interface::get_type_of(links_container)::value_type>::type> _Link_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(links_container),_Link_Interface*> _Links_Container_Interface;

	_Links_Container_Interface::iterator links_itr;

	for(links_itr=network->links_container<_Links_Container_Interface&>().begin();
		links_itr!=network->links_container<_Links_Container_Interface&>().end();
		links_itr++)
	{
		((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
	}

	cout << "initializing intersection agents..." <<endl;
	//define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, _Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Prototype, NULLTYPE);
	//
	
	typedef Intersection<remove_pointer<_Network_Interface::get_type_of(intersections_container)::value_type>::type> _Intersection_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(intersections_container),_Intersection_Interface*> _Intersections_Container_Interface;

	_Intersections_Container_Interface::iterator intersections_itr;

	for(intersections_itr=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().begin();
		intersections_itr!=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().end();
		intersections_itr++)
	{
		((_Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();
	}

	cout << "initializing ramp metering agents..." <<endl;
	//define_container_and_value_interface(_Ramp_Metering_Container_Interface, _Ramp_Metering_Interface, _Network_Interface::get_type_of(ramp_metering_container), Random_Access_Sequence_Prototype, Ramp_Metering_Prototype, NULLTYPE);

	typedef Ramp_Metering<remove_pointer<_Network_Interface::get_type_of(ramp_metering_container)::value_type>::type> _Ramp_Metering_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(ramp_metering_container),_Ramp_Metering_Interface*> _Ramp_Metering_Container_Interface;

	_Ramp_Metering_Container_Interface::iterator ramp_metering_itr;

	for(ramp_metering_itr=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().begin();
		ramp_metering_itr!=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().end();
		ramp_metering_itr++)
	{
		((_Ramp_Metering_Interface*)(*ramp_metering_itr))->Initialize<NULLTYPE>();
	}

	if (scenario->use_network_events<bool>())
	{
		MasterType::link_type::subscribe_events<NT>();
	}
	#pragma endregion
	


	//==================================================================================================================================
	// EXTERNAL Demand
	//----------------------------------------------------------------------------------------------------------------------------------

	if (scenario->read_demand_from_database<bool>())
	{
		typedef Demand_Components::Prototypes::Demand<MasterType::demand_type> _Demand_Interface;
		_Demand_Interface* demand = (_Demand_Interface*)Allocate<MasterType::demand_type>();
		demand->scenario_reference<_Scenario_Interface*>(scenario);
		demand->network_reference<_Network_Interface*>(network);
		cout << "reading external demand data..." <<endl;
		demand->read_demand_data<Net_IO_Type>(network_io_maps);
	}

	


	//==================================================================================================================================
	// Initialize global randon number generators - if seed set to zero or left blank use system time
	//---------------------------------------------------------------------------------------------------------------------------------- 
	int seed = scenario->iseed<int>();
	if (seed != 0)
	{
		GLOBALS::Normal_RNG.Set_Seed<int>(seed);
		GLOBALS::Uniform_RNG.Set_Seed<int>(seed);
	}
	else
	{
		GLOBALS::Normal_RNG.Set_Seed<int>();
		GLOBALS::Uniform_RNG.Set_Seed<int>();
	}
	

	//==================================================================================================================================
	// Set up graphical display
	//----------------------------------------------------------------------------------------------------------------------------------
	#ifdef ANTARES
	//define_container_and_value_interface(_Zones_Container_Interface, _Zone_Interface, typename _Network_Interface::get_type_of(zones_container), Containers::Associative_Container_Prototype, Zone_Components::Prototypes::Zone_Prototype, NULLTYPE);
	//_Zones_Container_Interface::iterator zone_itr;
	//_Zones_Container_Interface* zone_list = network->zones_container<_Zones_Container_Interface*>();

	////--------------------------------------------------------------------------------------------
	//// Graphical zone group display - integrate to graphical network when database is fixed
	//typedef Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,NULLTYPE> zone_group_interface;
	//zone_group_interface* _graphical_zone_group = (zone_group_interface*) Allocate<MasterType::graphical_zone_group_type>();	
	//// initialize zone static reference to the graphical zone group
	//MasterType::zone_type::_graphical_zone_group=(Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type,MasterType::zone_type>*)_graphical_zone_group;
	//_graphical_zone_group->configure_zones_layer<NULLTYPE>();
	#endif


	//==================================================================================================================================
	// Network Skimming stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	cout << "Initializing network skims..." <<endl;
	typedef Network_Skimming_Components::Prototypes::Network_Skimming<MasterType::network_skim_type/*_Network_Interface::get_type_of(skimming_faculty)*/> _network_skim_itf;
	_network_skim_itf* skimmer = (_network_skim_itf*)Allocate<MasterType::network_skim_type/*_Network_Interface::get_type_of(skimming_faculty)*/>();
	skimmer->read_input<bool>(scenario->read_skim_tables<bool>());
	if (skimmer->read_input<bool>())
	{
		skimmer->read_transit<bool>(true);
		skimmer->read_highway_cost<bool>(true);
		if (!skimmer->highway_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_highway_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: input binary highway skim file '" << scenario->input_highway_skim_file_path_name<string>() << "' could not be opened. Highway skims are required, application terminating.");
		if (!skimmer->transit_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_transit_skim_file_path_name<string>().c_str()))
		{
			skimmer->read_transit<bool>(false);
			cout << "Error: input binary transit skim file '" << scenario->input_transit_skim_file_path_name<string>() << "' not found.  Transit mode set to unavailable.";
		}
		if (!skimmer->highway_cost_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_highway_cost_skim_file_path_name<string>().c_str())) 
		{
			skimmer->read_highway_cost<bool>(false);
			cout << "Error: input binary highway cost skim file '" << scenario->input_highway_cost_skim_file_path_name<string>() << "' not found. Highway tolls and parking cost set to 0.";
		}
	}
	skimmer->write_output<bool>(scenario->write_skim_tables<bool>());	
	if (skimmer->write_output<bool>())
	{
		if (!skimmer->highway_output_file<File_IO::Binary_File_Writer&>().Open(scenario->output_highway_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: output binary skim file '" << scenario->output_highway_skim_file_path_name<string>() << "' could not be opened.");
		if (!skimmer->transit_output_file<File_IO::Binary_File_Writer&>().Open(scenario->output_transit_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: output binary transit skim file '" << scenario->output_transit_skim_file_path_name<string>() << "' could not be opened.");
		if (!skimmer->highway_cost_output_file<File_IO::Binary_File_Writer&>().Open(scenario->output_highway_cost_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: output binary highway cost skim file '" << scenario->output_highway_cost_skim_file_path_name<string>() << "' could not be opened.");
	}
	skimmer->Initialize<_Network_Interface*>(network);
	network->skimming_faculty<_network_skim_itf*>(skimmer);

	cout << "Network skims done." <<endl;

	//==================================================================================================================================
	// Destination choice model - set parameters
	//----------------------------------------------------------------------------------------------------------------------------------
	MasterType::person_destination_chooser_type::_choice_set_size = 100;

	// Initialize start time model
	MasterType::activity_timing_chooser_type::static_initializer("start_time_duration_data.txt");	
	// Initialize person properties with average activity frequency and duration
	MasterType::person_properties_type::Static_Initializer();

	//==================================================================================================================================
	// POPSYN stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef PopSyn::Prototypes::Population_Synthesizer<MasterType::population_synthesis_type> popsyn_itf;
	popsyn_itf* popsyn = (popsyn_itf*)Allocate<MasterType::population_synthesis_type>();
	popsyn->Initialize<_Network_Interface*, _Scenario_Interface*>(network,scenario);

	//----------------------------------------------------------------------------------------------------------------------------------

	//==================================================================================================================================
	// Logging of activity generation / scheduling outputs
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef Person_Components::Prototypes::Person_Data_Logger<MasterType::person_data_logger_type> _Logger_Interface;	
	_Logger_Interface* logger=(_Logger_Interface*)Allocate<MasterType::person_data_logger_type>();
	logger->Initialize<NT>();
	_global_person_logger = logger;

	if (scenario->use_network_events<bool>()) MasterType::link_type::subscribe_events<NT>();



	//==================================================================================================================================
	// Start Simulation
	//----------------------------------------------------------------------------------------------------------------------------------
	try
	{
		cout <<"Starting simulation..."<<endl;
		START();
	}
	catch (std::exception ex)
	{
		cout << ex.what();
	}

	cout << "Finished!" << endl;
	//system("PAUSE");
}


