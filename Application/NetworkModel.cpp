#define EXCLUDE_DEMAND

#include "Polaris_PCH.h"

#define FOR_LINUX_PORTING
#define SHOW_WARNINGS
#include "Application_Includes.h"
#include "../File_IO/network_models.h"

#ifdef EXCLUDE_DEMAND
#include "../Repository/RNG_Implementations.h"
#endif

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
	typedef Antares_Link_Implementation<MasterType> link_type;
	typedef Antares_Intersection_Implementation<MasterType> intersection_type;
	typedef Vehicle_Components::Implementations::Antares_Vehicle_Implementation<MasterType> vehicle_type;
#else
#ifndef EXCLUDE_DEMAND
	typedef Network_Components::Implementations::Integrated_Polaris_Network_Implementation<MasterType> network_type;
#else
	typedef Network_Components::Implementations::Polaris_Network_Implementation<MasterType> network_type;
#endif
	typedef Link_Components::Implementations::Polaris_Link_Implementation<MasterType> link_type;
	typedef Intersection_Components::Implementations::Polaris_Intersection_Implementation<MasterType> intersection_type;
	typedef Vehicle_Components::Implementations::Polaris_Vehicle_Implementation<MasterType> vehicle_type;

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
	typedef Scenario_Components::Implementations::Polaris_Scenario_Implementation<MasterType> scenario_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> movement_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> turn_movement_type;

	typedef Routing_Components::Implementations::Routable_Network_Implementation<MasterType> routable_network_type;
	
	typedef Routing_Components::Implementations::Polaris_Routing_Implementation<MasterType> routing_type;

	typedef Intersection_Components::Implementations::Routable_Intersection_Implementation<MasterType> routable_intersection_type;

	typedef Link_Components::Implementations::Routable_Link_Implementation<MasterType> routable_link_type;
	
	typedef Demand_Components::Implementations::Polaris_Demand_Implementation<MasterType> demand_type;

	typedef Activity_Location_Components::Implementations::Polaris_Activity_Location_Implementation<MasterType> activity_location_type;

	typedef Traveler_Components::Implementations::Polaris_Traveler_Implementation<MasterType> traveler_type;

	typedef Intersection_Components::Implementations::Polaris_Inbound_Outbound_Movements_Implementation<MasterType> inbound_outbound_movements_type;

	typedef Intersection_Components::Implementations::Polaris_Outbound_Inbound_Movements_Implementation<MasterType> outbound_inbound_movements_type;

	typedef Intersection_Components::Implementations::Routable_Inbound_Outbound_Movements_Implementation<MasterType> routable_inbound_outbound_movements_type;

	typedef Intersection_Components::Implementations::Routable_Outbound_Inbound_Movements_Implementation<MasterType> routable_outbound_inbound_movements_type;

	typedef Intersection_Components::Implementations::Routable_Movement_Implementation<MasterType> routable_movement_type;

	typedef Operation_Components::Implementations::Polaris_Operation_Implementation<MasterType> operation_type;
	
	typedef Intersection_Control_Components::Implementations::Polaris_Intersection_Control_Implementation<MasterType> intersection_control_type;

	typedef Intersection_Control_Components::Implementations::Polaris_Control_Plan_Implementation<MasterType> control_plan_type;

	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Implementation<MasterType> phase_type;

	typedef Intersection_Control_Components::Implementations::Polaris_Phase_Movement_Implementation<MasterType> phase_movement_type;
	
	typedef Intersection_Control_Components::Implementations::Polaris_Approach_Implementation<MasterType> approach_type;

	typedef Ramp_Metering_Components::Implementations::Polaris_Ramp_Metering_Implementation<MasterType> ramp_metering_type;

	typedef Zone_Components::Implementations::Polaris_Zone_Implementation<MasterType> zone_type;

	typedef Plan_Components::Implementations::Polaris_Plan_Implementation<MasterType> plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Implementation<MasterType> movement_plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;

	typedef Traveler_Components::Implementations::Polaris_Traveler_Implementation<MasterType> person_type;

	typedef Vehicle_Components::Implementations::Polaris_Switch_Decision_Data_Implementation<MasterType> switch_decision_data_type;

	typedef Analyze_Link_Group_Components::Implementations::Polaris_Analyze_Link_Group_Implementation<MasterType> analyze_link_group_type;


    //typedef Person_Components::Implementations::CTRAMP_Person_Planner_Implementation<MasterType, person_type> person_planner_type;

    //typedef Person_Components::Implementations::ADAPTS_Person_Properties_Implementation<MasterType,person_type> person_properties_type;

    typedef RNG_Components::Implementations::RngStream_Implementation<MasterType> RNG;
	typedef Network_Components::Implementations::Network_DB_Reader_Implementation<MasterType> network_db_reader_type;
	typedef Traffic_Management_Center_Components::Implementations::Simple_TMC<MasterType> traffic_management_center_type;
	typedef Network_Event_Components::Implementations::Base_Network_Event<MasterType> base_network_event_type;
#ifndef EXCLUDE_DEMAND    
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<MasterType> network_skim_type;
    typedef Network_Skimming_Components::Implementations::Mode_Skim_Table_Implementation<MasterType> network_mode_skim_type;
    typedef Routing_Components::Implementations::Polaris_Skim_Routing_Implementation<MasterType> skim_routing_type;
#endif
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
#endif
	
	typedef TYPELIST_4(weather_network_event_type,accident_network_event_type,congestion_network_event_type,lane_closure_network_event_type) network_event_types;
	typedef TYPELIST_5(link_control_type,depot_type,advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

	typedef Network_Event_Components::Implementations::Network_Event_Manager_Implementation<MasterType> network_event_manager_type;

};

ostream* stream_ptr;
void run_with_input_from_db();
void run_with_input_from_files();

int main(int argc, char* argv[])
{
	cout << "Running NetworkModel" << endl;
	if (argc > 1 && strcmp(argv[1], "input_from_files") == 0)
	{
		cout << "Input from files" << endl;
		run_with_input_from_files();
	}
	else
	{

		cout << "Input from DB" << endl;
		run_with_input_from_db();

	}
}


void run_with_input_from_db()
{
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	//===============
	// OUTPUT OPTIONS
	//----------------
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

	cout << "allocating data structures..." <<endl;	
	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
	_Network_Interface* network=(_Network_Interface*)Allocate<typename MasterType::network_type>();
	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);
	
	cout << "reading scenario data..." <<endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>("scenario.json");

	cout << "reading network data..." <<endl;	
	network->read_network_data<Net_IO_Type>(network_io_maps);

	//cout << "initializing simulation..." <<endl;	
	network->simulation_initialize<NULLTYPE>();

	define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
	_Demand_Interface* demand = (_Demand_Interface*)Allocate<typename MasterType::demand_type>();
	demand->scenario_reference<_Scenario_Interface*>(scenario);
	demand->network_reference<_Network_Interface*>(network);
	cout << "reading demand data..." <<endl;
	demand->read_demand_data<Net_IO_Type>(network_io_maps);

	define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	if (scenario->intersection_control_flag<int>() == 1) {
		cout <<"reading intersection control data..." << endl;
		operation->read_intersection_control_data<Net_IO_Type>(network_io_maps);
	}
	if (scenario->ramp_metering_flag<bool>() == true) {
		cout <<"reading ramp metering data..." << endl;
		operation->read_ramp_metering_data<Net_IO_Type>(network_io_maps);
	}

	if (scenario->write_db_input_to_files<bool>())
	{
		cout << "converting scenario data..." << endl;
		scenario->write_scenario_data<NULLTYPE>(scenario_data_for_output);
		network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);
		cout<<"writing scenario data..."<<endl;
		network_models::network_information::scenario_data_information::write_scenario_data(scenario_data_for_output);	

		cout << "converting network data..." << endl;
		network->write_network_data<Target_Type<NULLTYPE,NULLTYPE,network_models::network_information::network_data_information::NetworkData&>>(network_data_for_output);
		network_models::network_information::network_data_information::write_network_data("", network_data_for_output);
		cout<<"writing network data..."<<endl;
		network_models::network_information::network_data_information::write_network_data(output_dir_name,network_data_for_output);

		cout << "converting demand data..." << endl;
		demand->write_demand_data<NULLTYPE>(demand_data_for_output);
		cout<<"writing demand data..."<<endl;
		network_models::network_information::demand_data_information::write_demand_vehicle(output_dir_name,scenario_data_for_output,demand_data_for_output,network_data_for_output);

		cout <<"converting operation data..." << endl;
		operation->write_operation_data<NULLTYPE>(network_data_for_output, operation_data_for_output);
		cout<<"writing operation data..."<<endl;
		network_models::network_information::operation_data_information::write_operation_data(output_dir_name,scenario_data_for_output,operation_data_for_output,network_data_for_output);
	}

	if (scenario->run_simulation_for_db_input<bool>())
	{
#ifdef ANTARES
		network->set_network_bounds<NULLTYPE>();
		Rectangle_XY<MasterType>* local_bounds=network->network_bounds<Rectangle_XY<MasterType>*>();
		START_UI(MasterType,local_bounds->_xmin,local_bounds->_ymin,local_bounds->_xmax,local_bounds->_ymax);
		MasterType::vehicle_type::Initialize_Layer();
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

		if(scenario->use_network_events<bool>())
		{
			define_component_interface(_Network_Event_Manager_Interface, typename MasterType::network_event_manager_type, Network_Event_Manager, NULLTYPE);
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

		cout << "initializing ramp metering agents..." <<endl;
		define_container_and_value_interface(_Ramp_Metering_Container_Interface, _Ramp_Metering_Interface, _Network_Interface::get_type_of(ramp_metering_container), Random_Access_Sequence_Prototype, Ramp_Metering_Prototype, NULLTYPE);
		_Ramp_Metering_Container_Interface::iterator ramp_metering_itr;

		for(ramp_metering_itr=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().begin();
			ramp_metering_itr!=network->ramp_metering_container<_Ramp_Metering_Container_Interface&>().end();
			ramp_metering_itr++)
		{
			((_Ramp_Metering_Interface*)(*ramp_metering_itr))->Initialize<NULLTYPE>();
		}

		if (scenario->use_network_events<bool>())
		{
			MasterType::link_type::subscribe_events();
		}
#ifndef EXCLUDE_DEMAND
		//==================================================================================================================================
		// Network Skimming stuff
		//----------------------------------------------------------------------------------------------------------------------------------
		define_component_interface(_network_skim_itf, _Network_Interface::get_type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,NULLTYPE);
		_network_skim_itf* skimmer = (_network_skim_itf*)Allocate<_Network_Interface::get_type_of(skimming_faculty)>();
		skimmer->read_input<bool>(false);
		skimmer->write_output<bool>(scenario->write_skim_tables<bool>());
		if (skimmer->write_output<bool>())
		{
				skimmer->output_file<File_IO::Binary_File_Writer&>().Open(scenario->output_skim_file_path_name<string>().c_str());
		}
		skimmer->Initialize<_Network_Interface*>(network);
		network->skimming_faculty<_network_skim_itf*>(skimmer);
#endif
		cout << "starting sim..." <<endl;

		START();

		cout << "Finished!" << endl;
		system("PAUSE");
	}
}

void run_with_input_from_files()
{
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::File_Network,network_models::network_information::network_data_information::NetworkData&> Net_IO_Type;

	//reading from files
	network_models::network_information::scenario_data_information::ScenarioData scenario_data;
	network_models::network_information::network_data_information::NetworkData network_data;
	network_models::network_information::demand_data_information::DemandData demand_data;
	network_models::network_information::operation_data_information::OperationData operation_data;
	network_models::initialization(scenario_data,network_data,demand_data,operation_data);
	scenario_data.input_dir_name = "";
	scenario_data.output_dir_name = "";
	network_models::read_data(scenario_data,network_data,demand_data,operation_data);

	cout << "reading input data..." <<endl;	
	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;
	scenario->write_output_summary<bool>(true);
	scenario->write_network_snapshots<bool>(false);
	scenario->read_network_snapshots<bool>(false);
	scenario->write_network_link_turn_time<bool>(false);
	scenario->output_network_moe_for_simulation_interval<bool>(true);
	scenario->output_link_moe_for_assignment_interval<bool>(true);
	scenario->output_network_moe_for_assignment_interval<bool>(true);
	scenario->calculate_realtime_moe<bool>(true);
	scenario->compare_with_historic_moe<bool>(false);
	scenario->read_normal_day_link_moe<bool>(false);
	scenario->rng_type<int>(Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC);
	scenario->merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LANE);
	scenario->write_vehicle_trajectory<bool>(false);
	//scenario->compare_with_moe_reference<bool>(false);

	scenario->read_scenario_data<Scenario_Components::Types::File_Scenario>(scenario_data);

	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
	_Network_Interface* network = (_Network_Interface*)Allocate<typename MasterType::network_type>();
	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);
	network->read_network_data<Net_IO_Type>(network_data);

	//typedef Traffic_Management_Center<MasterType::traffic_management_center_type,NT> TMC_Interface;

	//TMC_Interface* tmc = (TMC_Interface*) Allocate< MasterType::traffic_management_center_type >();
	//tmc->scenario_reference<_Scenario_Interface*>(scenario);
	//tmc->Initialize<NT>();

	define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
	_Demand_Interface* demand = (_Demand_Interface*)Allocate<typename MasterType::demand_type>();
	
	demand->scenario_reference<_Scenario_Interface*>(scenario);
	demand->network_reference<_Network_Interface*>(network);
	demand->read_demand_data<Network_Components::Types::File_Network>(scenario_data, network_data, demand_data);

	define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	operation->read_operation_data<Operation_Components::Types::File_Operation>(scenario_data, network_data, operation_data);

	network->simulation_initialize<NULLTYPE>();

#ifdef ANTARES
	network->set_network_bounds<NULLTYPE>();
	Rectangle_XY<MasterType>* local_bounds=network->network_bounds<Rectangle_XY<MasterType>*>();
	START_UI(MasterType,local_bounds->_xmin,local_bounds->_ymin,local_bounds->_xmax,local_bounds->_ymax);
	MasterType::vehicle_type::Initialize_Layer();
	network->initialize_antares_layers<NULLTYPE>();
	MasterType::link_type::configure_link_moes_layer();
#endif

	cout << "world started..." << endl;
	////initialize network agents
	
	define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
	_Links_Container_Interface::iterator links_itr;

	for(links_itr=network->links_container<_Links_Container_Interface&>().begin();
		links_itr!=network->links_container<_Links_Container_Interface&>().end();
		links_itr++)
	{
		((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
	}

	define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, _Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Prototype, NULLTYPE);
	_Intersections_Container_Interface::iterator intersections_itr;

	for(intersections_itr=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().begin();
		intersections_itr!=network->intersections_container<typename MasterType::network_type::intersections_container_type&>().end();
		intersections_itr++)
	{

		((_Intersection_Interface*)(*intersections_itr))->Initialize<NULLTYPE>();		
	}

	//MasterType::link_type::subscribe_events();

	START();

	cout << "Finished!" << endl;
	system("PAUSE");
}

