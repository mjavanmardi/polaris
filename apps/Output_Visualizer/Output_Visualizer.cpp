#define EXCLUDE_DEMAND


#include "Polaris_PCH.h"
#include "Antares_Visual_Link_Implementation.h"
#include "Antares_Visual_Intersection_Implementation.h"
#include "Antares_Visual_Vehicle_Implementation.h"

#define SHOW_WARNINGS
#include "Application_Includes.h"



struct MasterType
{
#ifdef ANTARES
	typedef Conductor_Implementation<MasterType> conductor_type;
	typedef Control_Panel_Implementation<MasterType> control_panel_type;
	typedef Time_Panel_Implementation<MasterType> time_panel_type;
	typedef Information_Panel_Implementation<MasterType> information_panel_type;
	typedef Canvas_Implementation<MasterType> canvas_type;
	typedef Antares_Layer_Implementation<MasterType> antares_layer_type;
	typedef Layer_Options_Implementation<MasterType> layer_options_type;
	typedef Attributes_Panel_Implementation<MasterType> attributes_panel_type;
	typedef Control_Dialog_Implementation<MasterType> control_dialog_type;
	typedef Information_Page_Implementation<MasterType> information_page_type;
	typedef Splash_Panel_Implementation<MasterType> splash_panel_type;

	typedef Antares_Link_Implementation<MasterType> graphical_link_type;
	typedef Antares_Intersection_Implementation<MasterType> graphical_intersection_type;

	typedef Antares_Network_Implementation<MasterType> network_type;
	typedef Antares_Visual_Link_Implementation<MasterType> link_type;
	
	typedef Antares_Visual_Intersection_Implementation<MasterType> intersection_type;
	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<MasterType> vehicle_type;
	typedef Vehicle_Components::Implementations::Antares_Visual_Vehicle_Implementation<MasterType> visual_vehicle_type;

#else
#ifndef EXCLUDE_DEMAND
	typedef Network_Components::Implementations::Integrated_Network_Implementation<MasterType> network_type;
#else
	typedef Network_Components::Implementations::Network_Implementation<MasterType> network_type;
#endif
	typedef Link_Components::Implementations::Visual_Link_Implementation<MasterType> link_type;
	typedef Intersection_Components::Implementations::Intersection_Implementation<MasterType> intersection_type;
	typedef Vehicle_Components::Implementations::Vehicle_Implementation<MasterType> vehicle_type;
#endif

	//==============================================================================================
	// Signalization Types
	//typedef Signal_Components::Components::HCM_Signal_Full<T>::type				SIGNAL_TYPE;
	//typedef Signal_Components::Components::HCM_Phase_Full<T>::type				PHASE_TYPE;
	//typedef Signal_Components::Components::HCM_LaneGroup_Full<T>::type			LANE_GROUP_TYPE;
	//typedef Signal_Components::Components::HCM_Approach_Full<T>::type			APPROACH_TYPE;
	//typedef Signal_Components::Components::Signal_Indicator_Basic_Display<T>::type	INDICATOR_TYPE;
	//typedef Signal_Components::Components::Signal_Detector<T>::type				DETECTOR_TYPE;

	//typedef Signal_Components::Components::HCM_Signal_Full<T>::type			FULL_SIGNAL_TYPE;
	//typedef Signal_Components::Components::HCM_Signal_Simple<T>::type		SIMPLE_SIGNAL_TYPE;	
	//typedef Signal_Components::Components::HCM_Phase_Full<T>::type			FULL_PHASE_TYPE;
	//typedef Signal_Components::Components::HCM_Phase_Simple<T>::type		SIMPLE_PHASE_TYPE;
	//typedef Signal_Components::Components::HCM_LaneGroup_Full<T>::type		FULL_LANE_GROUP_TYPE;
	//typedef Signal_Components::Components::HCM_LaneGroup_Simple<T>::type	SIMPLE_LANE_GROUP_TYPE;

	//==============================================================================================
	// Network Types
	typedef Scenario_Components::Implementations::Scenario_Implementation<MasterType> scenario_type;
	
	typedef Turn_Movement_Components::Implementations::Movement_Implementation<MasterType> movement_type;
	
	typedef Turn_Movement_Components::Implementations::Movement_Implementation<MasterType> turn_movement_type;

	
	typedef Routing_Components::Implementations::Routable_Network_Implementation<MasterType> routable_network_type;
	
	//TODO:ROUTING
	
	typedef Routing_Components::Implementations::Routing_Implementation<MasterType> routing_type;
	//typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<MasterType> routable_intersection_type;
	//typedef Link_Components::Implementations::Routable_Link_Implementation<MasterType> routable_link_type;
	
	typedef Demand_Components::Implementations::Demand_Implementation<MasterType> demand_type;

	typedef Activity_Location_Components::Implementations::Activity_Location_Implementation<MasterType> activity_location_type;

	typedef Traveler_Components::Implementations::Traveler_Implementation<MasterType> traveler_type;

	typedef Intersection_Components::Implementations::Inbound_Outbound_Movements_Implementation<MasterType> inbound_outbound_movements_type;

	typedef Intersection_Components::Implementations::Outbound_Inbound_Movements_Implementation<MasterType> outbound_inbound_movements_type;

	//TODO:ROUTING
	//typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<MasterType> routable_inbound_outbound_movements_type;

	//TODO:ROUTING
	//typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<MasterType> routable_outbound_inbound_movements_type;

	//TODO:ROUTING
	//typedef Intersection_Components::Implementations::Routable_Movement_Implementation<MasterType> routable_movement_type;

	typedef Operation_Components::Implementations::Operation_Implementation<MasterType> operation_type;
	
	typedef Intersection_Control_Components::Implementations::Intersection_Control_Implementation<MasterType> intersection_control_type;

	typedef Intersection_Control_Components::Implementations::Control_Plan_Implementation<MasterType> control_plan_type;

	typedef Intersection_Control_Components::Implementations::Phase_Implementation<MasterType> phase_type;

	typedef Intersection_Control_Components::Implementations::Phase_Movement_Implementation<MasterType> phase_movement_type;
	
	typedef Intersection_Control_Components::Implementations::Approach_Implementation<MasterType> approach_type;


	typedef Zone_Components::Implementations::Zone_Implementation<MasterType> zone_type;

	typedef Plan_Components::Implementations::Plan_Implementation<MasterType> plan_type;

	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<MasterType> movement_plan_type;

	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;

	typedef Traveler_Components::Implementations::Traveler_Implementation<MasterType> person_type;

	typedef Vehicle_Components::Implementations::Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;

	typedef Analyze_Link_Group_Components::Implementations::Analyze_Link_Group_Implementation<MasterType> analyze_link_group_type;


	typedef Vehicle_Components::Implementations::Vehicle_Data_Logger_Implementation<MasterType> vehicle_data_logger_type;


    typedef RNG_Components::Implementations::RngStream_Implementation<MasterType> RNG;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<MasterType> network_db_reader_type;
	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
#ifndef EXCLUDE_DEMAND    
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<MasterType> network_skim_type;
    typedef Network_Skimming_Components::Implementations::Mode_Skim_Table_Implementation<MasterType> network_mode_skim_type;
    typedef Routing_Components::Implementations::Skim_Routing_Implementation<MasterType> skim_routing_type;
#endif
#ifdef ANTARES
	typedef Network_Event_Components::Implementations::Antares_Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Antares_Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;
	typedef Ramp_Metering_Components::Implementations::Antares_Ramp_Metering_Implementation<MasterType> ramp_metering_type;

	typedef Link_Control_Components::Implementations::Antares_Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Antares_Tow_Truck<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Antares_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Antares_Variable_Speed_Sign<MasterType> variable_speed_sign_type;
	typedef Sensor_Components::Implementations::Antares_Link_Sensor<MasterType> link_sensor_type;
#else
	typedef Network_Event_Components::Implementations::Weather_Network_Event<MasterType> weather_network_event_type;
	typedef Network_Event_Components::Implementations::Accident_Network_Event<MasterType> accident_network_event_type;
	typedef Network_Event_Components::Implementations::Congestion_Network_Event<MasterType> congestion_network_event_type;
	typedef Network_Event_Components::Implementations::Lane_Closure_Network_Event<MasterType> lane_closure_network_event_type;

	typedef Ramp_Metering_Components::Implementations::Ramp_Metering_Implementation<MasterType> ramp_metering_type;

	typedef Link_Control_Components::Implementations::Lane_Link_Control<MasterType> link_control_type;
	typedef Depot_Components::Implementations::Tow_Truck_Depot<MasterType> depot_type;
	typedef Depot_Components::Implementations::Tow_Truck_Implementation<MasterType> tow_truck_type;

	typedef Advisory_Radio_Components::Implementations::Highway_Advisory_Radio<MasterType> advisory_radio_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Word_Sign<MasterType> variable_word_sign_type;
	typedef Variable_Message_Sign_Components::Implementations::Variable_Speed_Sign<MasterType> variable_speed_sign_type;
	typedef Sensor_Components::Implementations::Link_Sensor<MasterType> link_sensor_type;
#endif
	
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;


	
	//typedef Time_Dependent_Agent_Implementation<MasterType> time_dependent_agent_type;




	
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

	
	typedef Routable_Agent_Implementation<MasterType> routable_agent_type;

	typedef Tree_Agent_Implementation<MasterType> tree_agent_type;


};


ostream* stream_ptr;
void run_with_input_from_db(char* scenario_filename, char* snapshot_filename);

int main(int argc, char* argv[])
{
	//initialize the core
	Simulation_Configuration cfg;
	//cfg.Single_Threaded_Setup(24*60*60);
	//nuber of iterations, number of threads, TODO: should use argv instedad
	cfg.Multi_Threaded_Setup(24*60*60,12);
	INITIALIZE_SIMULATION(cfg);

	char* scenario_filename = "scenario.json";
	char* snapshot_filename = "snapshot.bin";
	if (argc >= 2) scenario_filename = argv[1];
	if (argc >= 3) snapshot_filename = argv[2];

	run_with_input_from_db(scenario_filename, snapshot_filename);
}


void run_with_input_from_db(char* scenario_filename, char* snapshot_filename)
{

	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;


	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();

	cout << "allocating data structures..." <<endl;	

	//input from the json control file is sotred in this class
	typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	//interface to all of the objects related to network simulation
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


	//cout << "initializing simulation..." <<endl;	
	//network->simulation_initialize<NULLTYPE>();
	
	#ifdef ANTARES
	network->set_network_bounds<NULLTYPE>();
	Rectangle_XY<MasterType>* local_bounds=network->network_bounds<Rectangle_XY<MasterType>*>();
	START_UI(MasterType,local_bounds->_xmin,local_bounds->_ymin,local_bounds->_xmax,local_bounds->_ymax);
	MasterType::visual_vehicle_type::Initialize_Layer();
	network->initialize_antares_layers<NULLTYPE>();
	MasterType::link_type::configure_link_moes_layer();
	#endif

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

	////initialize network agents
	cout << "initializing link agents..." <<endl;

	//define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
		
	typedef Link<typename MasterType::link_type> _Link_Interface;
	typedef Random_Access_Sequence<_Network_Interface::Component_Type::links_container_accessible_type,_Link_Interface*> _Links_Container_Interface;

	_Links_Container_Interface::iterator links_itr;

	for(links_itr=network->links_container<_Links_Container_Interface&>().begin();
		links_itr!=network->links_container<_Links_Container_Interface&>().end();
		links_itr++)
	{
		((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
	}

	cout << "starting sim..." <<endl;

	//==================================================================================================================================
	// Logging of vehicle outputs
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef Vehicle_Components::Prototypes::Vehicle_Data_Logger<MasterType::vehicle_data_logger_type> _Vehicle_Logger_Interface;	
	_Vehicle_Logger_Interface* vlogger=(_Vehicle_Logger_Interface*)Allocate<MasterType::vehicle_data_logger_type>();
	string snapshot_name(snapshot_filename);
	vlogger->Initialize<NT>(true, snapshot_name);
	_global_vehicle_logger = vlogger;
	

	// Connect the vehicle display class to the time panel, so that when time changes the vehicle displayer updates
	Time_Panel<MasterType::time_panel_type>* time_panel = ((Antares_Implementation<MasterType>*)((Antares_App<MasterType>*)antares)->main)->time_panel<Time_Panel<MasterType::time_panel_type>*>();
	time_panel->Enable_Time_Navigation<void*>(_global_vehicle_logger);
	MasterType::vehicle_data_logger_type::Initialize_Type(time_panel);

	START();

	cout << "Finished!" << endl;
	system("PAUSE");
}
