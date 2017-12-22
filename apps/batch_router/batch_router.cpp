#define EXCLUDE_DEMAND

#include "Polaris_PCH.h"
#include "Traffic_Simulator.h"
#include "Traveler_Simulator.h"
#include "Scenario_Manager.h"
#include "Application_Includes.h"

#include "Batch_Router_Implementation.h"

using namespace polaris;




struct MasterType
{
	typedef MasterType M;

	//==============================================================================================
	#pragma region Network Types
	//----------------------------------------------------------------------------------------------
	typedef Network_Components::Implementations::Network_Implementation<M> network_type;
	typedef Link_Components::Implementations::Link_Implementation<M> link_type;
	typedef Intersection_Components::Implementations::Intersection_Implementation<M> intersection_type;
	typedef Vehicle_Components::Implementations::Vehicle_Implementation<M> vehicle_type;
	typedef Vehicle_Components::Implementations::Vehicle_Characteristics_Implementation<M> vehicle_characteristics_type;
	typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;

	typedef Scenario_Components::Implementations::Scenario_Implementation<M> scenario_type;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<M> network_db_reader_type;
	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> movement_type;

	typedef Turn_Movement_Components::Implementations::Movement_Implementation<M> turn_movement_type;
	typedef Transit_Route_Components::Implementations::Transit_Route_Implementation<M> transit_route_type;
	typedef Transit_Pattern_Components::Implementations::Transit_Pattern_Implementation<M> transit_pattern_type;
	typedef Transit_Vehicle_Trip_Components::Implementations::Transit_Vehicle_Trip_Implementation<M> transit_vehicle_trip_type;
	typedef Routing_Components::Implementations::Routable_Network_Implementation<M> routable_network_type;
	typedef Routing_Components::Implementations::Routing_Implementation<M> routing_type;
	typedef Routing_Components::Implementations::Skim_Routing_Implementation<M> skim_routing_type;
	typedef Routing_Components::Implementations::Dijkstra_for_Heuristics_Implementation<M> dijkstra_heuristics_routing_type;
	typedef Activity_Location_Components::Implementations::Activity_Location_Implementation<M> activity_location_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> traveler_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> person_type;
	typedef Vehicle_Components::Implementations::Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;
	typedef Intersection_Components::Implementations::Inbound_Outbound_Movements_Implementation<M> inbound_outbound_movements_type;
	typedef Intersection_Components::Implementations::Outbound_Inbound_Movements_Implementation<M> outbound_inbound_movements_type;
	typedef Operation_Components::Implementations::Operation_Implementation<M> operation_type;
	typedef Intersection_Control_Components::Implementations::Intersection_Control_Implementation<M> intersection_control_type;
	typedef Intersection_Control_Components::Implementations::Control_Plan_Implementation<M> control_plan_type;
	typedef Intersection_Control_Components::Implementations::Phase_Implementation<M> phase_type;
	typedef Intersection_Control_Components::Implementations::Phase_Movement_Implementation<M> phase_movement_type;
	typedef Intersection_Control_Components::Implementations::Approach_Implementation<M> approach_type;
	
	typedef Analyze_Link_Group_Components::Implementations::Analyze_Link_Group_Implementation<MasterType> analyze_link_group_type;

	typedef Plan_Components::Implementations::Plan_Implementation<M> plan_type;

	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Record_Implementation<M> movement_plan_record_type;

	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<M> trajectory_unit_type;


	typedef Batch_Router_Components::Implementations::Routed_Trip_Implementation<M> trip_type;
	//typedef Network_Components::Implementations::Network_Validation_Unit_Implementation<M> network_validation_unit_type;
	#pragma endregion
	//----------------------------------------------------------------------------------------------


	//==============================================================================================
	#pragma region TMC Types
	//----------------------------------------------------------------------------------------------
	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
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

	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;
	#pragma endregion

	//==============================================================================================
	#pragma region ROUTING Types
	//----------------------------------------------------------------------------------------------
	typedef Routable_Agent_Implementation<MasterType> routable_agent_type;
	typedef Tree_Agent_Implementation<MasterType> tree_agent_type;
	typedef Multi_Modal_Tree_Agent_Implementation<MasterType> multi_modal_tree_agent_type;
	typedef Walk_to_Transit_Tree_Agent_Implementation<MasterType> walk_to_transit_tree_agent_type;
	typedef Drive_to_Transit_Tree_Agent_Implementation<MasterType> drive_to_transit_tree_agent_type;
	typedef Graph_Implementation<MasterType, NTL, Base_Edge_A_Star<MasterType>> base_graph_type;
	typedef Graph_Pool_Implementation<MasterType, NTL, base_graph_type> graph_pool_type;
	typedef Edge_Implementation<Routing_Components::Types::static_attributes<MasterType>> static_edge_type;
	typedef Graph_Implementation<MasterType, NTL, static_edge_type> static_graph_type;
	typedef Routing_Components::Types::static_to_static static_to_static_type;
	typedef Custom_Connection_Group<MasterType, static_graph_type, static_graph_type, static_to_static_type> static_to_static_connection_type;

	typedef Edge_Implementation<Routing_Components::Types::multimodal_attributes<MasterType>> multimodal_edge_type;
	typedef Graph_Implementation<MasterType, NTL, multimodal_edge_type> multimodal_graph_type;
	typedef Routing_Components::Types::multimodal_to_multimodal multimodal_to_multimodal_type;
	typedef Custom_Connection_Group<MasterType, multimodal_graph_type, multimodal_graph_type, multimodal_to_multimodal_type> multimodal_to_multimodal_connection_type;
	
	typedef Edge_Implementation<Routing_Components::Types::time_dependent_attributes<MasterType>> time_dependent_edge_type;
	typedef Graph_Implementation<MasterType, NTL, time_dependent_edge_type> time_dependent_graph_type;
	typedef Routing_Components::Types::time_dependent_to_time_dependent time_dependent_to_time_dependent_type;
	typedef Custom_Connection_Group<MasterType, time_dependent_graph_type, time_dependent_graph_type, time_dependent_to_time_dependent_type> time_dependent_to_time_dependent_connection_type;

	#pragma endregion
	//----------------------------------------------------------------------------------------------
};


void write_scenario_file(File_IO::File_Info& scenario, File_IO::File_Info& db, File_IO::File_Info& historical_db);

bool InitializeMultiModalRoutingParameters(MasterType::scenario_type* scenario)
{
	if (!MasterType::routable_network_type::static_initialize(scenario->multimodal_routing_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize Multimodal Routing parameters." << endl;
		return false;
	}
	MasterType::routable_network_type::print_parameters();

	return true;
}

//==============================================================================================
// Main Batch_Router function
// This routine expects 3 (or optionally 4) call line arguments:
// argv[1] = database_filename: the filepath to the supply database containing the network information
// argv[2] = historical_moe_database_name: the filepath to the database containing the historical network MOE data
// argv[3] = trips_filename: filepath to a delimited data file containing the list of trips to be routed in the following format:
//	1			2							3															4										5
//	Trip_ID		Mode ('sov,rail,bus,walk')	Origin Location (location id code from supply database)		Destination Location (same as origin)	Departure time
// argv[4] = num_threads: defaults to 1 if not present, more than 1 runs multithreaded mode
int main(int argc,char** argv)
{

	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region Scenario Initialization
	char* scenario_filename = "scenario_router.json";
	char* database_filename = "";
	char* historical_database_filename = "";
	char* input_filename = "";

	if (argc < 4) THROW_EXCEPTION("ERROR: specify a database filename as the first command line argument.");
	
	database_filename = argv[1];
	historical_database_filename = argv[2];
	input_filename = argv[3];

	File_IO::File_Info db(database_filename);
	File_IO::File_Info results_db(historical_database_filename);
	File_IO::File_Info scenario_file(scenario_filename);

	int threads = 1;
	if (argc >= 5) threads = std::max(atoi(argv[4]),threads);

	for (int i = 0; i < threads; ++i)
	{
		results_by_thread.push_back(stringstream(""));
		summary_by_thread.push_back(stringstream(""));
		details_by_thread.push_back(stringstream(""));
		object_count_by_thread.push_back(0);
	}

	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(100, threads);
	INITIALIZE_SIMULATION(cfg);

	write_scenario_file(scenario_file, db, results_db);
	#pragma endregion

	

	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region New network_model.cpp stuff
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

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

	//==================================================================================================================================
	// Initialize global randon number generators - if seed set to zero or left blank use system time
	//---------------------------------------------------------------------------------------------------------------------------------- 
	#pragma region RNG initialization
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
	#pragma endregion

	typedef MasterType::network_type::link_dbid_dir_to_ptr_map_type link_dbid_dir_to_ptr_map_type;

	link_dbid_dir_to_ptr_map_type* link_dbid_dir_to_ptr_map = network->template link_dbid_dir_to_ptr_map<link_dbid_dir_to_ptr_map_type*>();

	// Initialize routing parameters
	if (!InitializeMultiModalRoutingParameters(scenario)) return 1;

	cout << "reading network data..." <<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);
	typedef Operation<MasterType::operation_type> _Operation_Interface;
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	if (scenario->intersection_control_flag<int>() == 1) {
		cout <<"reading intersection control data..." << endl;
		operation->read_intersection_control_data<Net_IO_Type>(network_io_maps);
	}
	network->simulation_initialize<NULLTYPE>();


	if (scenario->ramp_metering_flag<bool>() == true) {
		cout <<"reading ramp metering data..." << endl;
		operation->read_ramp_metering_data<Net_IO_Type>(network_io_maps);
	}


	if(scenario->use_network_events<bool>())
	{
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
	// Batch Routing stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	// activity location map - link the location id to the actual object pointer
	typedef Random_Access_Sequence< _Network_Interface::get_type_of(activity_locations_container)> _Activity_Locations_Container_Interface;
	typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(_Activity_Locations_Container_Interface)> _Activity_Location_Interface;
	typedef Batch_Router_Components::Prototypes::Routed_Trip<typename MasterType::trip_type> _Trip_Interface;

	_Activity_Locations_Container_Interface& activity_locations = network->template activity_locations_container<_Activity_Locations_Container_Interface&>();
	dense_hash_map<int,_Activity_Location_Interface*> activity_id_to_ptr;
	activity_id_to_ptr.set_empty_key(-1);
	activity_id_to_ptr.set_deleted_key(-2);
	for(_Activity_Locations_Container_Interface::iterator activity_locations_itr=activity_locations.begin();activity_locations_itr!=activity_locations.end();activity_locations_itr++)
	{
		_Activity_Location_Interface& activity_location=(_Activity_Location_Interface&)**activity_locations_itr;
		activity_id_to_ptr[activity_location.template uuid<int>()]=&activity_location;
	}

	// Read the input data file
	File_IO::File_Reader fr;
	fr.Open(input_filename,true,",\t");
	while (fr.Read())
	{
		int trip_id = fr.Get_Int(0);
		string mode = fr.Get_String(1);
		int orig = fr.Get_Int(2);
		int dest = fr.Get_Int(3);
		int time = fr.Get_Int(4);

		_Activity_Location_Interface* orig_loc = activity_id_to_ptr[orig];
		_Activity_Location_Interface* dest_loc = activity_id_to_ptr[dest];

		if (orig_loc == nullptr || dest_loc == nullptr)
		{
			cout << "Origin or destination are invalid for trip " << trip_id << endl;
			continue;
		}

		_Trip_Interface* trip=(_Trip_Interface*)Allocate<typename MasterType::trip_type>();
		trip->Initialize<_Network_Interface*,_Activity_Location_Interface*,Simulation_Timestep_Increment>(mode, trip_id, network,orig_loc,dest_loc,time);
	}

	// WRITE results
	stringstream output_filename("");
	output_filename << scenario->template output_dir_name<string>();
	output_filename << "routed_results.dat";
	fw_output.Open(output_filename.str());

	bool debug_route = Routing_Components::Implementations::Routable_Network_Implementation<MasterType>::debug_route<bool>();
	if (debug_route)
	{
		stringstream details_title("");
		details_title << "Origin_ID\tDestination_ID\tDeparture_Time\tSub_Mode\tLink_Ctr\tNode_A\tNode_B\tTrip_ID\tSequence\tType\tArr_Time\tGen_Cost\tTime\tWait_Count\tWait_Time\tWalk_Time\tBike_Time\tIVTT\tCar_Time\tTransfer_Pen\tEst_Cost\tScan_Count\taStar_Time";
		stringstream details_filename("");
		details_filename << scenario->template output_dir_name<string>();
		details_filename << "sp_output.dat";
		fw_mm_sp_details.Open(details_filename.str());
		fw_mm_sp_details.Write(details_title);

		stringstream summary_title("");
		summary_title << "Thread\tOrigin\tDestination\tDeparture_Time\tSub_Mode\tArrival_Time\tGen_Cost\tDuration\tWait_Count\tWait_Time\tWalk_Time\tBike_Time\tIVTT\tCar_Time\tTransfer_Pen\tEst_Cost\tScan_Count\taStar_Time\tNeighbor_Visit_Time\tSuccess_Status\tEuc_Distance\n";
		//summary_title << "Origin\tDestination\tDeparture_Time\tArrival_Time\tGen_Cost\tDuration\tEst_Cost\tScan_Count\n";
		stringstream summary_filename("");
		summary_filename << scenario->template output_dir_name<string>();
		summary_filename << "sp_labels_output.dat";
		fw_mm_sp_summary.Open(summary_filename.str());
		fw_mm_sp_summary.Write_NoDelim(summary_title);
	}

	//==================================================================================================================================
	// Start Simulation
	//----------------------------------------------------------------------------------------------------------------------------------
	cout <<"Starting simulation..."<<endl;
	try	{START();}
	catch (std::exception ex){ cout << ex.what();}

	// WRITE results
	for (int i = 0; i < num_sim_threads(); ++i) 
	{ 		
		fw_output.Write(results_by_thread[i]);
		fw_mm_sp_summary.Write_NoDelim(summary_by_thread[i]);
		fw_mm_sp_details.Write_NoDelim(details_by_thread[i]);
	}	
		
	fw_mm_sp_details.Close();
	fw_mm_sp_summary.Close();
	fw_output.Close();
	cout << "Finished! Press 'Any' key" << endl;

}


void write_scenario_file(File_IO::File_Info& scenario, File_IO::File_Info& db, File_IO::File_Info& historical_db)
{
	// strip out the extension and db schema info (just retain the base db name - until this is changed in the DB schema - which it should be.)
	string database_name("");
	if (db.Is_File_Type("sqlite"))
		database_name = db.db_name();
	else if (!db.Has_Extension())
		database_name = db.full_name();
	else
		THROW_EXCEPTION("ERROR, the input db should be specified as either a .sqlite file or an extensionless db name.");
	string historical_db_name("");
	if (historical_db.Is_File_Type("sqlite"))
		historical_db_name = historical_db.db_name();
	else if (!historical_db.Has_Extension())
		historical_db_name = historical_db.full_name();
	else
		THROW_EXCEPTION("ERROR, the historical db should be specified as either a .sqlite file or an extensionless db name.");

	//if (!historical_db.Is_File_Type("sqlite")) THROW_EXCEPTION("ERROR: historical database_filename must be a .sqlite file");
	if (!scenario.Is_File_Type("json")) THROW_EXCEPTION("ERROR: scenario filename must be a .json file");

	File_IO::File_Writer fw;
	fw.Open(scenario.full_name());
	fw.Write_Line("{");
	fw.Write_Line("\t\"output_dir_name\" : \"batch_router\",");
	stringstream s("");
	s<<"\t\"database_name\" : \""<<database_name<<"\",";
	fw.Write_Line(s);
	s.clear(); s.str("");
	s<<"\t\"historical_results_database_name\" : \""<<historical_db_name<<"\",";
	fw.Write_Line(s);
	fw.Write_Line("\t\"seed\" : 1234567,");
	fw.Write_Line("\t\"multimodal_routing\" : true,");
	fw.Write_Line("\t\"multimodal_routing_model_file\" : \"MultiModalRoutingModel.json\",");
	fw.Write_Line("\t\"time_dependent_routing_weight_factor\" : 0.0,");
	fw.Write_Line("\t\"time_dependent_routing\" : false,");
	fw.Write_Line("\t\"write_demand_to_database\" : true\n}");
	
	fw.Close();
}