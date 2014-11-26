#define EXCLUDE_DEMAND

#include "Polaris_PCH.h"
#include "Application_Includes.h"
#include "Network_Skimming_Prototype.h"


using namespace polaris;


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
	typedef Network_Components::Implementations::Network_Implementation<M> network_type;
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

	typedef Movement_Plan_Components::Implementations::Movement_Plan_Implementation<M> movement_plan_type;
	typedef Movement_Plan_Components::Implementations::Movement_Plan_Record_Implementation<M> movement_plan_record_type;

	typedef Movement_Plan_Components::Implementations::Trajectory_Unit_Implementation<M> trajectory_unit_type;
	typedef Network_Skimming_Components::Implementations::Basic_Network_Skimming_Implementation<M> network_skim_type;
	typedef Network_Skimming_Components::Implementations::LOS_Value_Implementation<M> los_value_type;
	typedef Network_Skimming_Components::Implementations::LOS_Time_Invariant_Value_Implementation<M> los_invariant_value_type;
	//typedef Network_Components::Implementations::Network_Validation_Implementation<M> network_validation_type;
	//typedef Network_Components::Implementations::Network_Validation_Unit_Implementation<M> network_validation_unit_type;
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


#include <iostream>
#include <string>

char *
bad_strdup(const char *s)
{
  char s2[10000];
  strncpy(s2, s, 10000);
  return s2;
}

int
main(int argc, char *argv[])
{
  //char *s2 = bad_strdup("this is a test");
  //char *s1 = bad_strdup("eat my lunch");
	int *p1 = nullptr;
	for (int i=0; i<100000; i++)
	{
		p1 = new int[20]();

		*p1 = 5;

		//cout <<p1 << ": " << *p1<<endl;

		delete p1;
	}
	delete p1;
	cout <<p1 << ": " << *p1<<endl;

	//cout << "s2 = " << s2 << endl;

	char test;
	cin >> test;
}



//==================================================
// SKIMFILE READER APPLICATION
/*int main(int argc, char** argv)
{
	char test;
	cin >> test;

	string input_filename, output_filename;

	if (argc < 2)
	{
		cout << "Enter input skimfile name: ";
		cin >> input_filename;
	}
	else
	{
		input_filename = argv[1];
	}
	
	output_filename = input_filename + ".csv";

	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(1, 1);
	INITIALIZE_SIMULATION(cfg);

	// Create interface
	typedef Network_Skimming_Components::Prototypes::Network_Skimming<MasterType::network_skim_type> skim_interface;
	typedef skim_interface::get_type_of(update_interval_endpoints) intervals_type;

	skim_interface* skimmer = (skim_interface*)Allocate<MasterType::network_skim_type>();
	if (!skimmer->highway_input_file<File_IO::Binary_File_Reader&>().Open(input_filename)) THROW_EXCEPTION("Error: input binary highway skim file '" << input_filename << "' could not be opened. Highway skims are required, application terminating.");


	int num_modes;
	int num_zones;
	int update_increment;
	intervals_type intervals;

	skimmer->Read_Binary_Headers<intervals_type*>(num_modes, num_zones, &intervals, false);


	ofstream outfile;
	outfile.open(output_filename);

			
	Simulation_Timestep_Increment start;
	//===========================================================================
	// create the skim_table time periods, for basic create only a single time period skim_table

	for (int i=0; i < intervals.size(); ++i)
	{
		if (intervals[i]>GLOBALS::Time_Converter.template Convert_Value<Time_Hours,Time_Minutes>(1440)) break;
		if (i > 0)
		{
			if (intervals[i] == intervals[i-1]) break;
		}

		float* data = new float[num_zones*num_zones];
		skimmer->Read_Binary_Data<float*>(data,num_zones);

		outfile << endl << "Skim matrix for hour: " << GLOBALS::Time_Converter.template Convert_Value<Time_Minutes,Time_Hours>(intervals[i])<<endl;

		for (int i =0; i < num_zones; i++)
		{
			for (int j=0; j < num_zones; j++)
			{
				outfile << data[i*num_zones + j] << "," ;
			}
			outfile << endl;
		}
	}


	int test;
	cin >> test;
}
*/