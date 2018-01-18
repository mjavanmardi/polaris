#define EXCLUDE_DEMAND

#undef ANTARES

#include "Polaris_PCH.h"
#include "Application_Includes.h"

#include "Network_Validator_Implementation.h"

using namespace polaris;



struct MasterType
{
	typedef MasterType M;
	typedef Network_Components::Implementations::Network_Validation_Implementation<M> network_validation_type;
	typedef Network_Components::Implementations::Network_Validation_Unit_Implementation<M> network_validation_unit_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> traveler_type;
	typedef Traveler_Components::Implementations::Traveler_Implementation<M> person_type;

	//==============================================================================================
#pragma region Network Types
	//----------------------------------------------------------------------------------------------

	typedef Network_Components::Implementations::Integrated_Network_Implementation<M> network_type;
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
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> basic_movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Record_Implementation<M> movement_plan_record_type;
	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	typedef Network_Skimming_Components::Implementations::LOS_Value_Implementation<M> los_value_type;
	typedef Network_Skimming_Components::Implementations::LOS_Time_Invariant_Value_Implementation<M> los_invariant_value_type;
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
	typedef TYPELIST_4(weather_network_event_type, accident_network_event_type, congestion_network_event_type, lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_3(/*link_control_type,depot_type,*/advisory_radio_type, variable_word_sign_type, variable_speed_sign_type) its_component_types;

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
	


void write_scenario_file(char* database_name, char* scenario_name);


int main(int argc,char** argv)
{

	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region Scenario Initialization
	char* scenario_filename = "scenario_tester.json";
	char* database_filename = "a";
	if (argc >= 2) database_filename = argv[1];
	else THROW_EXCEPTION("ERROR: specify a database filename as the first command line argument.");
	int checks = 3;
	if (argc >= 3) checks = std::max(atoi(argv[2]),checks);
	int threads = 1;
	if (argc >= 4) threads = std::max(atoi(argv[3]),threads);
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(100, threads);
	INITIALIZE_SIMULATION(cfg);
	#pragma endregion

	write_scenario_file(database_filename, scenario_filename);

	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region New network_model.cpp stuff
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	//===============
	// OUTPUT OPTIONS
	//----------------
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

	cout << "reading network data..."<<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);
	cout << "done."<<endl;
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
	
	network->initialize_antares_layers<NULLTYPE>();
	MasterType::vehicle_type::Initialize_Layer();

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



	//==================================================================================================================================
	// Network Skimming stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	cout << "Initializing network validation..." <<endl;
	typedef Network_Components::Prototypes::Network_Validator<MasterType::network_validation_type> _network_validation_itf;
	typedef Random_Access_Sequence<typename _Network_Interface::get_type_of(activity_locations_container)> locations_itf;
	typedef Activity_Location_Components::Prototypes::Activity_Location<get_component_type(locations_itf)> location_itf;
	locations_itf* locations = network->activity_locations_container<locations_itf*>();
			
	_network_validation_itf* validator = (_network_validation_itf*)Allocate<MasterType::network_validation_type>();

	validator->network_reference<_Network_Interface*>(network);
	validator->Initialize<location_itf*>(checks);


	//==================================================================================================================================
	// Start Simulation
	//----------------------------------------------------------------------------------------------------------------------------------
	cout <<"Starting simulation..."<<endl;
	try	{START();}
	catch (std::exception ex){ cout << ex.what();}
	cout << "Finished! Press 'Any' key" << endl;

}

void write_scenario_file(char* database_name, char* scenario_name)
{
	// strip extension from database name
	string db_name(database_name);
	int name_pos = db_name.find_last_of('-');
	int pos = db_name.find_last_of('.');
	int len = db_name.length();
	string db(db_name.c_str());
	string ext("");
	if (pos != std::string::npos)
	{
		db = db_name.substr(0,name_pos);
		ext = db_name.substr(pos+1,len-pos-1);

		if (strcmp(ext.c_str(),"sqlite") != 0) THROW_EXCEPTION("ERROR: database_filename must be a sqlite file: "<<database_name<<", "<<db<<", "<<ext);
	}

	File_IO::File_Writer fw;
	fw.Open(string(scenario_name));
	fw.Write_Line("{");
	fw.Write_Line("\t\"io_source_flag\" : \"ODB_IO_SOURCE\",");
	stringstream s("");
	s<<"\t\"database_name\" : \""<<db<<"\"\n}";
	fw.Write(s);
	fw.Close();

	cout <<"Reading information from '"<<db<<"' sqlite file."<<endl;
}
