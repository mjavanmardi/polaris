//=========================================
// Inclusion of C-run time debugging
//#define MEM_DEBUG
//#ifdef MEM_DEBUG
//	#define _CRTDBG_MAP_ALLOC
//	#include <stdlib.h>
//	#include <crtdbg.h>
//#endif

//#define IntegratedModelApplication

//=========================================
// Debug build options
#ifdef _DEBUG
	//#define DEBUG_1
	#ifdef MEM_DEBUG
		#undef free
	#endif
	#undef ANTARES
#endif

//=========================================
// Miscellaneous warning and error message printing options
//#undef ENABLE_WARNINGS
//#define ENABLE_DEBUG_MESSAGES
//#define SHOW_WARNINGS


#include "Polaris_PCH.h"
#include "Traffic_Simulator.h"
#include "Traveler_Simulator.h"
#include "ITS_Simulator.h"
#include "Activity_Simulator.h"
#include "Population_Synthesis.h"
#include "Scenario_Manager.h"
#include "Application_Includes.h"

//TODO OMER: Delete when done
static File_IO::File_Writer fw_bus_mode;

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
	typedef NULLTYPE visual_vehicle_type;

	typedef Zone_Components::Implementations::Graphical_Zone_Implementation<M> zone_type;
	//typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;

	typedef Antares_Intersection_Implementation<M> intersection_type;
	typedef Zone_Components::Implementations::Graphical_Zone_Group_Implementation<M> graphical_zone_group_type;
	#else
	typedef Network_Components::Implementations::Integrated_Network_Implementation<M> network_type;
	typedef Link_Components::Implementations::Link_Implementation<M> link_type;
	typedef Intersection_Components::Implementations::Intersection_Implementation<M> intersection_type;
	typedef Vehicle_Components::Implementations::Vehicle_Implementation<M> vehicle_type;
	typedef Vehicle_Components::Implementations::Vehicle_Characteristics_Implementation<M> vehicle_characteristics_type;
	typedef Zone_Components::Implementations::Zone_Implementation<M> zone_type;
	#endif

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
	typedef Household_Components::Implementations::Vehicle_Chooser_Implementation<M> vehicle_chooser_type;
	typedef Household_Components::Implementations::UIC_Vehicle_Technology_Chooser_Implementation<M> vehicle_technology_chooser_type;
	
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
	typedef Person_Components::Implementations::ADAPTS_Mode_Chooser_Implementation<M> person_mode_chooser_type;
	typedef Person_Components::Implementations::ADAPTS_Mode_Choice_Option<M> mode_choice_option_type;
	typedef Person_Components::Implementations::Telecommute_Choice_Implementation<M> telecommute_chooser_type;

	typedef Choice_Model_Components::Implementations::MNL_Model_Implementation<MT> mnl_model_type;
	typedef Choice_Model_Components::Implementations::NL_Model_Implementation<MT> nl_model_type;
	typedef Hazard_Model_Components::Implementations::Additive_Weibull_Baseline_Hazard_Implementation<MT> hazard_model_type;
	
	#ifdef ANTARES
		//typedef Person_Components::Implementations::Antares_Person_Data_Logger_Implementation<M> person_data_logger_type;
		typedef Person_Components::Implementations::Person_Data_Logger_Implementation<M> person_data_logger_type;
		typedef Vehicle_Components::Implementations::Vehicle_Data_Logger_Implementation<M> vehicle_data_logger_type;
	#else
		typedef Person_Components::Implementations::Person_Data_Logger_Implementation<M> person_data_logger_type;
	#endif
	
	// POPULATION SYNTHESIS CLASSES
	typedef PopSyn::Implementations::Polaris_Synthesis_Zone_Implementation_Full<MasterType> synthesis_zone_type;
	typedef PopSyn::Implementations::Polaris_Synthesis_Region_Implementation_Full<MasterType> synthesis_region_type;
	typedef PopSyn::Implementations::IPF_Solver_Settings_Implementation<MasterType> solver_settings_type;
	typedef PopSyn::Implementations::ADAPTS_Population_Synthesis_Implementation<MasterType> population_synthesis_type;
	typedef PopSyn::Implementations::Popsyn_File_Linker_Implementation<MasterType> popsyn_file_linker_type;
	typedef polaris::io::Household hh_db_rec_type; // these represent the type of database record to write - use Household/Person for writing to Demand or Synthetic_Household/Person for writing to Popsyn only
	typedef polaris::io::Person person_db_rec_type;
	typedef polaris::io::Vehicle vehicle_db_rec_type;
	typedef polaris::io::Vehicle_Type vehicle_type_db_rec_type;
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
	typedef TYPELIST_3(/*link_control_type,depot_type,*/advisory_radio_type,variable_word_sign_type,variable_speed_sign_type) its_component_types;

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

bool InitializeChoiceModelParameters(MasterType::scenario_type* scenario)
{
	if (!MasterType::mode_choice_option_type::static_initialize(scenario->mode_choice_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize Mode Choice Model parameters." << endl;
		return false;
	}
	MasterType::mode_choice_option_type::print_parameters();

	if (!MasterType::person_destination_choice_option_type::static_initialize(scenario->template destination_choice_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize Destination Choice Model parameters." << endl;
		return false;
	}
	MasterType::person_destination_choice_option_type::print_parameters();

	if (!MasterType::activity_timing_chooser_type::static_initialize(scenario->template timing_choice_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize Timing Choice Model parameters." << endl;
		return false;
	}

	if (!MasterType::telecommute_chooser_type::static_initialize(scenario->template telecommute_choice_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize Telecommute Choice Model parameters." << endl;
		return false;
	}
	MasterType::telecommute_chooser_type::print_parameters();

	if (!MasterType::vehicle_technology_chooser_type::static_initialize(scenario->template cav_wtp_model_file<string>()))
	{
		cout << "ERROR: Unable to initialize CAV WTP Model parameters." << endl;
		return false;
	}
	MasterType::vehicle_technology_chooser_type::print_parameters();

	return true;
}

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

int main(int argc,char** argv)
{
	#ifdef MEM_DEBUG
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	//==================================================================================================================================
	// Scenario initialization
	//----------------------------------------------------------------------------------------------------------------------------------
	string scenario_filename = "scenario.json";
	
	// command line running
	if (argc == 2)
	{
		string command = argv[1];
		if (std::strncmp(command.c_str(), "-h", 2) == 0) // Do help
		{
			cout << "Keyword arguments for Polaris Command line:" << endl;
			cout << "-scenario:\tGenerate default scenario file." << endl;
			cout << "-databases:\tGenerate default input databases." << endl;
			cout << "-help:\t\tDisplay Polaris keyword help for command line." << endl;
			cout << "-usage:\t\tDescribe Polaris command line usage pattern." << endl << endl;
			return EXIT_SUCCESS;
		}
		else if (std::strncmp(command.c_str(), "-s", 2) == 0) // Generate default scenario file
		{
			ofstream outfile;
			outfile.open("scenario.json");
			outfile << "{" << endl;
			outfile << "\t\"accident_event_duration_reduction\" : 1," << endl;
			outfile << "\t\"activity_start_time_model_file_name\" : \"start_time_duration_data_new.txt\"," << endl;
			outfile << "\t\"aggregate_routing\" : false," << endl;
			outfile << "\t\"analyze_link_groups_file_path_name\" : \"analyze_link_groups\"," << endl;
			outfile << "\t\"automation_cost\" : 9999999999999," << endl;
			outfile << "\t\"buildings_geometry_file\" : \"\"," << endl;
			outfile << "\t\"calculate_realtime_moe\" : true," << endl;
			outfile << "\t\"capacity_adjustment_arterial\" : 1," << endl;
			outfile << "\t\"capacity_adjustment_highway\" : 1," << endl;
			outfile << "\t\"cav_market_penetration\" : 0," << endl;
			outfile << "\t\"cav_vott_adjustment\" : 1," << endl;
			outfile << "\t\"cav_wtp_model_file\" : \"\"," << endl;
			outfile << "\t\"color_cars_randomly\" : false," << endl;
			outfile << "\t\"compare_with_historic_moe\" : false," << endl;
			outfile << "\t\"count_integrated_in_network_vehicles_only\" : false," << endl;
			outfile << "\t\"database_name\" : \"\"," << endl;
			outfile << "\t\"demand_reduction_factor\" : 1," << endl;
			outfile << "\t\"destination_choice_model_file\" : \"\"," << endl;
			outfile << "\t\"do_planner_routing\" : false," << endl;
			outfile << "\t\"ending_time_hh_mm\" : 86400," << endl;
			outfile << "\t\"enroute_excessive_delay_factor\" : 1," << endl;
			outfile << "\t\"enroute_switching_enabled\" : true ," << endl;
			outfile << "\t\"enroute_switching_on_excessive_delay\" : true," << endl;
			outfile << "\t\"flexible_work_percentage\" : 0.12," << endl;
			outfile << "\t\"historic_demand_moe_directory\" : \"\"," << endl;
			outfile << "\t\"historic_link_moe_file_path_name\" : \"historic_moe_link.csv\"," << endl;
			outfile << "\t\"historic_network_moe_file_path_name\" : \"historic_realtime_moe_network.csv\"," << endl;
			outfile << "\t\"historical_results_database_name\" : \"\"," << endl;
			outfile << "\t\"information_compliance_rate_mean\" : 1," << endl;
			outfile << "\t\"information_compliance_rate_standard_deviation\" : 0.5," << endl;
			outfile << "\t\"input_highway_cost_skim_file_path_name\" : \"\"," << endl;
			outfile << "\t\"input_highway_skim_file_path_name\" : \"\"," << endl;
			outfile << "\t\"input_network_snapshots_file_path_name\" : \"input_network_snapshots\"," << endl;
			outfile << "\t\"input_popsyn_database_name\" : \"\"," << endl;
			outfile << "\t\"input_transit_skim_file_path_name\" : \"\"," << endl;
			outfile << "\t\"ipf_tolerance\" : 0.01," << endl;
			outfile << "\t\"jam_density_constraints_enforced\" : true," << endl;
			outfile << "\t\"load_analyze_link_groups_from_file\" : false," << endl;
			outfile << "\t\"marginal_tolerance\" : 5," << endl;
			outfile << "\t\"maximum_flow_rate_constraints_enforced\" : true," << endl;
			outfile << "\t\"maximum_iterations\" : 100," << endl;
			outfile << "\t\"merging_mode\" :  \"PROPORTION_TO_DEMAND\"," << endl;
			outfile << "\t\"minimum_delay_ratio_for_enroute_switching\" : 3," << endl;
			outfile << "\t\"minimum_delay_seconds_for_enroute_switching\" : 300," << endl;
			outfile << "\t\"minimum_seconds_from_arrival_for_enroute_switching\" : 300," << endl;
			outfile << "\t\"minimum_travel_time_saving_mean\" : 1," << endl;
			outfile << "\t\"minimum_travel_time_saving_standard_deviation\" : 1," << endl;
			outfile << "\t\"mode_choice_model_file\" : \"\"," << endl;
			outfile << "\t\"multimodal_network_input\" : false," << endl;
			outfile << "\t\"node_control_flag\" : 0," << endl;
			outfile << "\t\"num_simulation_intervals_per_assignment_interval\" : 50," << endl;
			outfile << "\t\"output_analzye_link_group_moe_for_assignment_interval\" : false," << endl;
			outfile << "\t\"output_dir_name\" : \"\"," << endl;
			outfile << "\t\"output_highway_cost_skim_file_path_name\" : \"highway_cost_skim_file_out.txt\"," << endl;
			outfile << "\t\"output_highway_skim_file_path_name\" : \"highway_skim_file_out.txt\"," << endl;
			outfile << "\t\"output_link_moe_for_assignment_interval\" : false," << endl;
			outfile << "\t\"output_link_moe_for_simulation_interval\" : false," << endl;
			outfile << "\t\"output_network_moe_for_assignment_interval\" : false," << endl;
			outfile << "\t\"output_network_moe_for_simulation_interval\" : false," << endl;
			outfile << "\t\"output_transit_skim_file_path_name\" : \"transit_skim_file_out.txt\"," << endl;
			outfile << "\t\"output_turn_movement_moe_for_assignment_interval\" : false," << endl;
			outfile << "\t\"output_turn_movement_moe_for_simulation_interval\" : false," << endl;
			outfile << "\t\"percent_to_synthesize\" : 1," << endl;
			outfile << "\t\"popsyn_control_file\" : \"popsyn_control_file.txt\"," << endl;
			outfile << "\t\"pretrip_informed_market_share\" : 0.5," << endl;
			outfile << "\t\"ramp_metering_flag\" : false," << endl;
			outfile << "\t\"read_demand_from_database\" : false," << endl;
			outfile << "\t\"read_network_snapshots\" : false," << endl;
			outfile << "\t\"read_normal_day_link_moe\" : false," << endl;
			outfile << "\t\"read_population_from_database\" : false," << endl;
			outfile << "\t\"read_skim_tables\" : false," << endl;
			outfile << "\t\"realtime_informed_vehicle_market_share\" : 0.5," << endl;
			outfile << "\t\"relative_indifference_band_route_choice_mean\" : 0.1," << endl;
			outfile << "\t\"rng_type\" :  \"DETERMINISTIC\"," << endl;
			outfile << "\t\"routing_with_snapshots\" : false," << endl;
			outfile << "\t\"run_simulation_for_db_input\" : true," << endl;
			outfile << "\t\"seed\" : 0," << endl;
			outfile << "\t\"simulate_cacc\" : false," << endl;
			outfile << "\t\"simulation_interval_length_in_second\" : 6," << endl;
			outfile << "\t\"skim_interval_length_minutes\" : 1440," << endl;
			outfile << "\t\"snapshot_period\" : 300," << endl;
			outfile << "\t\"starting_time_hh_mm\" : 0," << endl;
			outfile << "\t\"telecommute_choice_model_file\" : \"\"," << endl;
			outfile << "\t\"tile_imagery_alpha_level\" : 255," << endl;
			outfile << "\t\"tile_imagery_file\" : \"\"," << endl;
			outfile << "\t\"time_dependent_routing\" : false," << endl;
			outfile << "\t\"time_dependent_routing_weight_factor\" : 1," << endl;
			outfile << "\t\"time_dependent_routing_weight_scale\" : 1000," << endl;
			outfile << "\t\"time_dependent_routing_weight_shape\" : 2," << endl;
			outfile << "\t\"multimodal_routing\" : false," << endl;
			outfile << "\t\"use_link_based_routing\" : false," << endl;
			outfile << "\t\"use_network_events\" : false," << endl;
			outfile << "\t\"use_realtime_travel_time_for_enroute_switching\" : false," << endl;
			outfile << "\t\"use_tmc\" : false," << endl;
			outfile << "\t\"vehicle_distribution_file_name\" : \"vehicle_distribution.txt\"," << endl;
			outfile << "\t\"vehicle_taking_action\" : false," << endl;
			outfile << "\t\"vehicle_tracking_list_file_name\" : \"\"," << endl;
			outfile << "\t\"vehicle_trajectory_output_threshold\" : -1," << endl;
			outfile << "\t\"vehicle_trajectory_sample_rate\" : 1," << endl;
			outfile << "\t\"write_activity_output\" : false," << endl;
			outfile << "\t\"write_db_input_to_files\" : false," << endl;
			outfile << "\t\"write_demand_to_database\" : false," << endl;
			outfile << "\t\"write_full_output\" : false," << endl;
			outfile << "\t\"write_marginal_output\" : false," << endl;
			outfile << "\t\"write_network_link_flow\" : false," << endl;
			outfile << "\t\"write_network_link_turn_time\" : false," << endl;
			outfile << "\t\"write_network_snapshots\" : false," << endl;
			outfile << "\t\"write_node_control_state\" : false," << endl;
			outfile << "\t\"write_output_summary\" : true," << endl;
			outfile << "\t\"write_skim_tables\" : false," << endl;
			outfile << "\t\"write_ttime_distribution_from_network_model\" : false," << endl;
			outfile << "\t\"write_vehicle_trajectory\" : false," << endl;
			outfile << "\t\"write_visualizer_snapshot\" : false," << endl;
			outfile << "}" << endl;

			return EXIT_SUCCESS;
		}
		else if (std::strncmp(command.c_str(), "-d", 2) == 0) // Generate default databases
		{
			unique_ptr<odb::database> db(create_sqlite_database("db", polaris::io::db_inventory[0]));
			odb::transaction t(db->begin());
			t.commit();

			unique_ptr<odb::database> db2(create_sqlite_database("db", polaris::io::db_inventory[2]));
			odb::transaction t2(db2->begin());
			t2.commit();
			return EXIT_SUCCESS;
		}
		else if (std::strncmp(command.c_str(), "-u", 2) == 0) // Display usage information
		{
			cout << endl << "To run Polaris from the command line use the following calling pattern:" << endl;
			cout << "<binary path>\\Integrated_Model.exe <scenario_file> <number of threads: optional, default = 1> <number of agents hint: optional, default = 0>" << endl;
			return EXIT_SUCCESS;
		}	
	}

	// simulation running
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


	typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario = (_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	cout << "reading scenario data..." << endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename.c_str());


	//TODO OMER: Delete when done
	stringstream bus_mode_title("");
	bus_mode_title << "Source\twalkThreshold_init\twalk_distance_to_transit\tOrigin\tDestination\tDeparture_Time\tMode\n";
	stringstream bus_mode_filename("");
	bus_mode_filename << scenario->template output_dir_name<string>();
	bus_mode_filename << "bus_mode_output.dat";
	fw_bus_mode.Open(bus_mode_filename.str());
	fw_bus_mode.Write_NoDelim(bus_mode_title);


	//==================================================================================================================================
	// Initialize global randon number generators - if seed set to zero or left blank use system time
	//---------------------------------------------------------------------------------------------------------------------------------- 
	GLOBALS::Normal_RNG.Initialize();
	GLOBALS::Uniform_RNG.Initialize();
	unsigned long seed = scenario->iseed<unsigned long>();
	if (seed != 0)
	{
		GLOBALS::Normal_RNG.Set_Seed<unsigned long>(seed);
		GLOBALS::Uniform_RNG.Set_Seed<unsigned long>(seed);
	}
	else
	{
		GLOBALS::Normal_RNG.Set_Seed<unsigned long>();
		GLOBALS::Uniform_RNG.Set_Seed<unsigned long>();
	}


	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region New network_model.cpp stuff
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network,Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	cout << "allocating data structures..." << endl;

	//===============
	// OUTPUT OPTIONS
	//----------------
	string output_dir_name = "";

	typedef Network<typename MasterType::network_type> _Network_Interface;
	_Network_Interface* network=(_Network_Interface*)Allocate<typename MasterType::network_type>();

	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);

	if (!InitializeMultiModalRoutingParameters(scenario)) return 1;

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

			TMC_Interface* tmc = static_cast<TMC_Interface*>(Allocate< MasterType::traffic_management_center_type >());
			tmc->network_event_manager<_Network_Event_Manager_Interface*>(net_event_manager);
			tmc->Initialize<NT>();
		}
	}

	////initialize network agents
	cout << "initializing link agents..." <<endl;

	//define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Prototype, NULLTYPE);
	
	typedef Link<remove_pointer<_Network_Interface::get_type_of(links_container)::value_type>::type> _Link_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(links_container),_Link_Interface*> _Links_Container_Interface;

	for (const auto& link : network->links_container<_Links_Container_Interface&>())
	{
		link->Initialize<NULLTYPE>();
	}

	cout << "initializing intersection agents..." <<endl;
	//define_container_and_value_interface(_Intersections_Container_Interface, _Intersection_Interface, _Network_Interface::get_type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Prototype, NULLTYPE);
	//
	
	typedef Intersection<remove_pointer<_Network_Interface::get_type_of(intersections_container)::value_type>::type> _Intersection_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(intersections_container),_Intersection_Interface*> _Intersections_Container_Interface;

	for (const auto& intersection : network->intersections_container<typename MasterType::network_type::intersections_container_type&>())
	{
		intersection->Initialize<NULLTYPE>();
	}

	cout << "initializing ramp metering agents..." <<endl;
	//define_container_and_value_interface(_Ramp_Metering_Container_Interface, _Ramp_Metering_Interface, _Network_Interface::get_type_of(ramp_metering_container), Random_Access_Sequence_Prototype, Ramp_Metering_Prototype, NULLTYPE);

	typedef Ramp_Metering<remove_pointer<_Network_Interface::get_type_of(ramp_metering_container)::value_type>::type> _Ramp_Metering_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(ramp_metering_container),_Ramp_Metering_Interface*> _Ramp_Metering_Container_Interface;

	_Ramp_Metering_Container_Interface::iterator ramp_metering_itr;

	for (const auto& ramp : network->ramp_metering_container<_Ramp_Metering_Container_Interface&>())
	{
		ramp->Initialize<NULLTYPE>();
	}

	if (scenario->use_network_events<bool>())
	{
		MasterType::link_type::subscribe_events<NT>();
	}
	#pragma endregion
	




	//==================================================================================================================================
	// EXTERNAL Demand
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef Demand_Components::Prototypes::Demand<MasterType::demand_type> _Demand_Interface;
	_Demand_Interface* demand = (_Demand_Interface*)Allocate<MasterType::demand_type>();
	demand->scenario_reference<_Scenario_Interface*>(scenario);
	demand->network_reference<_Network_Interface*>(network);
	cout << "reading external demand data..." <<endl;
	demand->read_vehicle_type_data<NT>();
	if (scenario->read_demand_from_database<bool>()) demand->read_demand_data<Net_IO_Type>(network_io_maps);
	_global_demand = demand;
	

	//==================================================================================================================================
	// Set up graphical display
	//----------------------------------------------------------------------------------------------------------------------------------
	#ifdef ANTARES
	typedef polaris::Pair_Associative_Container<typename _Network_Interface::get_type_of(zones_container)> _Zones_Container_Interface;
	typedef Zone_Components::Prototypes::Zone<get_mapped_component_type(_Zones_Container_Interface)> _Zone_Interface;
	_Zones_Container_Interface::iterator zone_itr;
	_Zones_Container_Interface* zone_list = network->zones_container<_Zones_Container_Interface*>();

	//--------------------------------------------------------------------------------------------
	// Graphical zone group display - integrate to graphical network when database is fixed
	typedef Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type> zone_group_interface;
	zone_group_interface* _graphical_zone_group = (zone_group_interface*) Allocate<MasterType::graphical_zone_group_type>();	
	// initialize zone static reference to the graphical zone group
	MasterType::zone_type::_graphical_zone_group=(Zone_Components::Prototypes::Graphical_Zone_Group<MasterType::graphical_zone_group_type>*)_graphical_zone_group;
	_graphical_zone_group->configure_zones_layer<NULLTYPE>();
	#endif


	//==================================================================================================================================
	// Network Skimming stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	if (scenario->do_skimming<bool>())
	{
		cout << "Initializing network skims..." <<endl;
		typedef Network_Skimming_Components::Prototypes::Network_Skimming<MasterType::network_skim_type> _network_skim_itf;
		_network_skim_itf* skimmer = (_network_skim_itf*)Allocate<MasterType::network_skim_type>();



		skimmer->read_input<bool>(scenario->read_skim_tables<bool>());
		if (skimmer->read_input<bool>())
		{
			skimmer->read_transit<bool>(true);
			skimmer->read_highway_cost<bool>(true);
			if (!skimmer->highway_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_highway_skim_file_path_name<string>().c_str())) THROW_EXCEPTION("Error: input binary highway skim file '" << scenario->input_highway_skim_file_path_name<string>() << "' could not be opened. Highway skims are required, application terminating.");
			if (!skimmer->transit_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_transit_skim_file_path_name<string>().c_str()))
			{
				skimmer->read_transit<bool>(false);
				cout << "WARNING: input binary transit skim file '" << scenario->input_transit_skim_file_path_name<string>() << "' not found.  Transit mode set to unavailable.";
			}
			if (!skimmer->highway_cost_input_file<File_IO::Binary_File_Reader&>().Open(scenario->input_highway_cost_skim_file_path_name<string>().c_str())) 
			{
				skimmer->read_highway_cost<bool>(false);
				cout << "WARNING: input binary highway cost skim file '" << scenario->input_highway_cost_skim_file_path_name<string>() << "' not found. Highway tolls and parking cost set to 0.";
			}
		}
		else
		{
			skimmer->read_transit<bool>(false);
			skimmer->read_highway_cost<bool>(false);
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
	}
	else
	{
		THROW_EXCEPTION("ERROR: No network skimming properties specified in the scenario file.  Please set either 'READ_SKIM_TABLES' or 'WRITE_SKIM_TABLES' to true in order to continue.");
	}


	//==================================================================================================================================
	// Choice models - set parameters
	//----------------------------------------------------------------------------------------------------------------------------------
	MasterType::person_destination_chooser_type::choice_set_size(100);

	// Initialize all choice model parameters
	if (!InitializeChoiceModelParameters(scenario))
	{
		THROW_EXCEPTION("Ending...");
	}
	
	// Initialize start time model
	//MasterType::activity_timing_chooser_type::static_initializer(scenario->activity_start_time_model_file_name<string>());	

	// Initialize person properties with average activity frequency and duration
	MasterType::person_properties_type::Static_Initializer();
	
	// Initialize Vehicle Choice Model
	MasterType::vehicle_chooser_type::distribution_static_initializer(scenario->vehicle_distribution_file_name<string>(), demand);


	//==================================================================================================================================
	// POPSYN stuff
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef PopSyn::Prototypes::Population_Synthesizer<MasterType::population_synthesis_type> popsyn_itf;
	popsyn_itf* popsyn = static_cast<popsyn_itf*>(Allocate<MasterType::population_synthesis_type>());
	if (scenario->read_population_from_database<bool>())
	{
		popsyn->Read_From_Database<_Network_Interface*, _Scenario_Interface*>(network,scenario);
	}
	else
	{
		popsyn->Initialize<_Network_Interface*, _Scenario_Interface*>(network,scenario);
	}


	//==================================================================================================================================
	// Logging of activity generation / scheduling outputs
	//----------------------------------------------------------------------------------------------------------------------------------
	typedef Person_Components::Prototypes::Person_Data_Logger<MasterType::person_data_logger_type> _Logger_Interface;	
	_Logger_Interface* logger=(_Logger_Interface*)Allocate<MasterType::person_data_logger_type>();
	logger->Initialize<NT>();
	_global_person_logger = logger;

#ifdef ANTARES
	typedef Vehicle_Components::Prototypes::Vehicle_Data_Logger<MasterType::vehicle_data_logger_type> _Vehicle_Logger_Interface;	
	_Vehicle_Logger_Interface* vlogger=(_Vehicle_Logger_Interface*)Allocate<MasterType::vehicle_data_logger_type>();
	vlogger->Initialize<NT>();
	_global_vehicle_logger = vlogger;

	if (scenario->use_buildings<bool>())
	{
		MasterType::buildings_type::Initialize_Type();
		MasterType::buildings_type* buildings = Allocate<MasterType::buildings_type>();
		buildings->Initialize(scenario->buildings_geometry_file<string&>());
	}
#endif

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
	
	#ifdef MEM_DEBUG
		_CrtDumpMemoryLeaks();
	#endif

	cout << "Finished!" << endl;
}

//void output_object_sizes()
//{
//#ifdef ENABLE_MEMORY_LOGGING
//	ofstream file;
//	file.open("memory_logging_typeids.csv");
//
//	file << endl <<"network_type size = "<<sizeof(MasterType::network_type)<<","<<(int)MasterType::network_type::component_id<<endl;
//	file << endl <<"link_type size = "<<sizeof(MasterType::link_type)<<","<<(int)MasterType::link_type::component_id<<endl;
//	file << endl <<"intersection_type size = "<<sizeof(MasterType::intersection_type)<<","<<(int)MasterType::intersection_type::component_id<<endl;
//	file << endl <<"vehicle_type size = "<<sizeof(MasterType::vehicle_type)<<","<<(int)MasterType::vehicle_type::component_id<<endl;
//	file << endl <<"zone_type size = "<<sizeof(MasterType::zone_type)<<","<<(int)MasterType::zone_type::component_id<<endl;
//	file << endl <<"scenario_type size = "<<sizeof(MasterType::scenario_type)<<","<<(int)MasterType::scenario_type::component_id<<endl;
//	file << endl <<"network_db_reader_type size = "<<sizeof(MasterType::network_db_reader_type)<<","<<(int)MasterType::network_db_reader_type::component_id<<endl;
//	file << endl <<"movement_type size = "<<sizeof(MasterType::movement_type)<<","<<(int)MasterType::movement_type::component_id<<endl;
//	file << endl <<"turn_movement_type size = "<<sizeof(MasterType::turn_movement_type)<<","<<(int)MasterType::turn_movement_type::component_id<<endl;
//	file << endl <<"routable_network_type size = "<<sizeof(MasterType::routable_network_type)<<","<<(int)MasterType::routable_network_type::component_id<<endl;
//	file << endl <<"routing_type size = "<<sizeof(MasterType::routing_type)<<","<<(int)MasterType::routing_type::component_id<<endl;
//	file << endl <<"skim_routing_type size = "<<sizeof(MasterType::skim_routing_type)<<","<<(int)MasterType::skim_routing_type::component_id<<endl;
//	file << endl <<"activity_location_type size = "<<sizeof(MasterType::activity_location_type)<<","<<(int)MasterType::activity_location_type::component_id<<endl;
//	file << endl <<"traveler_type size = "<<sizeof(MasterType::traveler_type)<<","<<(int)MasterType::traveler_type::component_id<<endl;
//	file << endl <<"switch_decision_data_type size = "<<sizeof(MasterType::switch_decision_data_type)<<","<<(int)MasterType::switch_decision_data_type::component_id<<endl;
//	file << endl <<"inbound_outbound_movements_type size = "<<sizeof(MasterType::inbound_outbound_movements_type)<<","<<(int)MasterType::inbound_outbound_movements_type::component_id<<endl;
//	file << endl <<"outbound_inbound_movements_type size = "<<sizeof(MasterType::outbound_inbound_movements_type)<<","<<(int)MasterType::outbound_inbound_movements_type::component_id<<endl;
//	file << endl <<"operation_type size = "<<sizeof(MasterType::operation_type)<<","<<(int)MasterType::operation_type::component_id<<endl;
//	file << endl <<"intersection_control_type size = "<<sizeof(MasterType::intersection_control_type)<<","<<(int)MasterType::intersection_control_type::component_id<<endl;
//	file << endl <<"control_plan_type size = "<<sizeof(MasterType::control_plan_type)<<","<<(int)MasterType::control_plan_type::component_id<<endl;
//	file << endl <<"phase_type size = "<<sizeof(MasterType::phase_type)<<","<<(int)MasterType::phase_type::component_id<<endl;
//	file << endl <<"phase_movement_type size = "<<sizeof(MasterType::phase_movement_type)<<","<<(int)MasterType::phase_movement_type::component_id<<endl;
//	file << endl <<"approach_type size = "<<sizeof(MasterType::approach_type)<<","<<(int)MasterType::approach_type::component_id<<endl;
//	file << endl <<"plan_type size = "<<sizeof(MasterType::plan_type)<<","<<(int)MasterType::plan_type::component_id<<endl;
//	file << endl <<"movement_plan_type size = "<<sizeof(MasterType::movement_plan_type)<<","<<(int)MasterType::movement_plan_type::component_id<<endl;
//	file << endl <<"movement_plan_record_type size = "<<sizeof(MasterType::movement_plan_record_type)<<","<<(int)MasterType::movement_plan_record_type::component_id<<endl;
//	file << endl <<"trajectory_unit_type size = "<<sizeof(MasterType::trajectory_unit_type)<<","<<(int)MasterType::trajectory_unit_type::component_id<<endl;
//	file << endl <<"network_skim_type size = "<<sizeof(MasterType::network_skim_type)<<","<<(int)MasterType::network_skim_type::component_id<<endl;
//	file << endl <<"demand_type size = "<<sizeof(MasterType::demand_type)<<","<<(int)MasterType::demand_type::component_id<<endl;
//	file << endl <<"person_type size = "<<sizeof(MasterType::person_type)<<","<<(int)MasterType::person_type::component_id<<endl;
//	file << endl <<"person_planner_type size = "<<sizeof(MasterType::person_planner_type)<<","<<(int)MasterType::person_planner_type::component_id<<endl;
//	file << endl <<"person_mover_type size = "<<sizeof(MasterType::person_mover_type)<<","<<(int)MasterType::person_mover_type::component_id<<endl;
//	file << endl <<"person_scheduler_type size = "<<sizeof(MasterType::person_scheduler_type)<<","<<(int)MasterType::person_scheduler_type::component_id<<endl;
//	file << endl <<"person_perception_type size = "<<sizeof(MasterType::person_perception_type)<<","<<(int)MasterType::person_perception_type::component_id<<endl;
//	file << endl <<"activity_generator_type size = "<<sizeof(MasterType::activity_generator_type)<<","<<(int)MasterType::activity_generator_type::component_id<<endl;
//	file << endl <<"person_properties_type size = "<<sizeof(MasterType::person_properties_type)<<","<<(int)MasterType::person_properties_type::component_id<<endl;
//	file << endl <<"person_static_properties_type size = "<<sizeof(MasterType::person_static_properties_type)<<","<<(int)MasterType::person_static_properties_type::component_id<<endl;
//	file << endl <<"person_data_logger_type size = "<<sizeof(MasterType::person_data_logger_type)<<","<<(int)MasterType::person_data_logger_type::component_id<<endl;
//	file << endl <<"activity_type size = "<<sizeof(MasterType::activity_type)<<","<<(int)MasterType::activity_type::component_id<<endl;
//	file << endl <<"activity_plan_type size = "<<sizeof(MasterType::activity_plan_type)<<","<<(int)MasterType::activity_plan_type::component_id<<endl;
//	file << endl <<"routine_activity_plan_type size = "<<sizeof(MasterType::routine_activity_plan_type)<<","<<(int)MasterType::routine_activity_plan_type::component_id<<endl;
//	file << endl <<"at_home_activity_plan_type size = "<<sizeof(MasterType::at_home_activity_plan_type)<<","<<(int)MasterType::at_home_activity_plan_type::component_id<<endl;
//	file << endl <<"activity_record_type size = "<<sizeof(MasterType::activity_record_type)<<","<<(int)MasterType::activity_record_type::component_id<<endl;
//	file << endl <<"activity_timing_chooser_type size = "<<sizeof(MasterType::activity_timing_chooser_type)<<","<<(int)MasterType::activity_timing_chooser_type::component_id<<endl;
//	file << endl <<"person_destination_chooser_type size = "<<sizeof(MasterType::person_destination_chooser_type)<<","<<(int)MasterType::person_destination_chooser_type::component_id<<endl;
//	file << endl <<"person_destination_choice_option_type size = "<<sizeof(MasterType::person_destination_choice_option_type)<<","<<(int)MasterType::person_destination_choice_option_type::component_id<<endl;
//	file << endl <<"synthesis zone size = "<<sizeof(MasterType::synthesis_zone_type)<<","<<(int)MasterType::synthesis_zone_type::component_id<<endl;
//	file << endl <<"region size = "<<sizeof(MasterType::synthesis_region_type)<<","<<(int)MasterType::synthesis_region_type::component_id<<endl;
//	file << endl <<"IPF_Solver_Settings size = "<<sizeof(MasterType::solver_settings_type)<<","<<(int)MasterType::solver_settings_type::component_id<<endl;
//	file << endl <<"popsyn_solver size = "<<sizeof(MasterType::population_synthesis_type)<<","<<(int)MasterType::population_synthesis_type::component_id<<endl;
//	file << endl <<"traffic_management_center_type size = "<<sizeof(MasterType::traffic_management_center_type)<<","<<(int)MasterType::traffic_management_center_type::component_id<<endl;
//	file << endl <<"weather_network_event_type size = "<<sizeof(MasterType::weather_network_event_type)<<","<<(int)MasterType::weather_network_event_type::component_id<<endl;
//	file << endl <<"accident_network_event_type size = "<<sizeof(MasterType::accident_network_event_type)<<","<<(int)MasterType::accident_network_event_type::component_id<<endl;
//	file << endl <<"congestion_network_event_type size = "<<sizeof(MasterType::congestion_network_event_type)<<","<<(int)MasterType::congestion_network_event_type::component_id<<endl;
//	file << endl <<"lane_closure_network_event_type size = "<<sizeof(MasterType::lane_closure_network_event_type)<<","<<(int)MasterType::lane_closure_network_event_type::component_id<<endl;
//	file << endl <<"link_control_type size = "<<sizeof(MasterType::link_control_type)<<","<<(int)MasterType::link_control_type::component_id<<endl;
//	file << endl <<"depot_type size = "<<sizeof(MasterType::depot_type)<<","<<(int)MasterType::depot_type::component_id<<endl;
//	file << endl <<"advisory_radio_type size = "<<sizeof(MasterType::advisory_radio_type)<<","<<(int)MasterType::advisory_radio_type::component_id<<endl;
//	file << endl <<"variable_word_sign_type size = "<<sizeof(MasterType::variable_word_sign_type)<<","<<(int)MasterType::variable_word_sign_type::component_id<<endl;
//	file << endl <<"variable_speed_sign_type size = "<<sizeof(MasterType::variable_speed_sign_type)<<","<<(int)MasterType::variable_speed_sign_type::component_id<<endl;
//	file << endl <<"link_sensor_type size = "<<sizeof(MasterType::link_sensor_type)<<","<<(int)MasterType::link_sensor_type::component_id<<endl;
//	file << endl <<"base_network_event_type size = "<<sizeof(MasterType::base_network_event_type)<<","<<(int)MasterType::base_network_event_type::component_id<<endl;
//	file << endl <<"network_event_manager_type size = "<<sizeof(MasterType::network_event_manager_type)<<","<<(int)MasterType::network_event_manager_type::component_id<<endl;
//	file.close();
//#endif
//}
//





