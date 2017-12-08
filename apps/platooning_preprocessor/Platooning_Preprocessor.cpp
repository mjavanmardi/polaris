#define EXCLUDE_DEMAND

#include "Polaris_PCH.h"
#include "Traffic_Simulator.h"
#include "Traveler_Simulator.h"
#include "Scenario_Manager.h"
#include "Application_Includes.h"

#include "Platooning_Preprocessor_Implementation.h"

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


	typedef Platooning_Preprocessor_Components::Implementations::Routed_Trip_Implementation<M> trip_type;
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




	typedef Demand_Components::Implementations::Demand_Implementation<MasterType> demand_type;

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
// Main Platooning_Preprocessor function
// This routine expects 3 (or optionally 4) call line arguments:
// argv[1] = database_filename: the filepath to the supply database containing the network information
// argv[2] = historical_moe_database_name: the filepath to the database containing the historical network MOE data
// argv[3] = trips_filename: filepath to a delimited data file containing the list of trips to be routed in the following format:
//	1			2							3															4										5
//	Trip_ID		Mode ('sov,rail,bus,walk')	Origin Location (location id code from supply database)		Destination Location (same as origin)	Departure time
// argv[4] = num_threads: defaults to 1 if not present, more than 1 runs multithreaded mode
int main(int argc,char** argv)
{

	if (argc < 4) THROW_EXCEPTION("ERROR: specify a database filename as the first command line argument.");
	
	char* scenario_filename = "scenario_platoon.json";
	char* database_filename = "";
	char* historical_database_filename = "";
	char* input_filename = "";

	
	//if (argc >= 2) scenario_filename = argv[1];
	int threads = 1;
	/*if (argc >= 5) threads = std::max(atoi(argv[4]), threads);*/

	//database_filename = argv[2];
	//historical_database_filename = argv[3];
	//input_filename = argv[4];

	

	//==================================================================================================================================
	// NETWORK MODEL STUFF
	//----------------------------------------------------------------------------------------------------------------------------------
	#pragma region New network_model.cpp stuff
	Network_Components::Types::Network_IO_Maps network_io_maps;
	typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::ODB_Network, Network_Components::Types::Network_IO_Maps&> Net_IO_Type;

	//==================================================================================================================================
	// Allocation hints block
	//----------------------------------------------------------------------------------------------------------------------------------
	Simulation_Configuration cfg;
	cfg.Multi_Threaded_Setup(86400, threads);
	INITIALIZE_SIMULATION(cfg);

	typedef Scenario<typename MasterType::scenario_type> _Scenario_Interface;
	_Scenario_Interface* scenario = (_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;

	cout << "reading scenario data..." << endl;
	scenario->read_scenario_data<Scenario_Components::Types::ODB_Scenario>(scenario_filename);


	typedef Network<typename MasterType::network_type> _Network_Interface;
	_Network_Interface* network = (_Network_Interface*)Allocate<typename MasterType::network_type>();

	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);

	typedef MasterType::network_type::link_dbid_dir_to_ptr_map_type link_dbid_dir_to_ptr_map_type;
	link_dbid_dir_to_ptr_map_type* link_dbid_dir_to_ptr_map = network->template link_dbid_dir_to_ptr_map<link_dbid_dir_to_ptr_map_type*>();

	cout << "reading network data..." << endl;
	network->read_network_data<Net_IO_Type>(network_io_maps);

	
	typedef Link<remove_pointer<_Network_Interface::get_type_of(links_container)::value_type>::type> _Link_Interface;
	typedef Random_Access_Sequence<_Network_Interface::get_type_of(links_container), _Link_Interface*> _Links_Container_Interface;
	


	auto floyd_warshall_map = new std::map <_Link_Interface*, std::map <_Link_Interface*, double> *>();
	auto selected_links = new std::vector< _Link_Interface*>();
	_Links_Container_Interface::iterator links_itr;
	for (links_itr = network->links_container<_Links_Container_Interface&>().begin(); links_itr != network->links_container<_Links_Container_Interface&>().end(); links_itr++)
	{
		//((_Link_Interface*)(*links_itr))->Initialize<NULLTYPE>();
		//typedef Intersection<remove_pointer<_Network_Interface::get_type_of(intersections_container)::value_type>::type> _Intersection_Interface;			
		//typedef  Intersection_Components::Prototypes::Intersection<typename remove_pointer< typename type_of(network_reference)::get_type_of(intersections_container)::value_type>::type>  _Intersection_Interface;
		auto link = (_Link_Interface*)(*links_itr);
		Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();
		if (link_type == Link_Components::Types::Link_Type_Keys::FREEWAY || link_type == Link_Components::Types::Link_Type_Keys::EXPRESSWAY ||
			link_type == Link_Components::Types::Link_Type_Keys::ON_RAMP || link_type == Link_Components::Types::Link_Type_Keys::OFF_RAMP)
		{
			selected_links->push_back(link);
		}
	}

	//Filling out the connected edges cost
	for (auto lnk_itr = selected_links->begin(); lnk_itr != selected_links->end() ; lnk_itr++)
	{
		_Link_Interface* link = *lnk_itr;
		floyd_warshall_map->insert(std::pair <_Link_Interface*, std::map <_Link_Interface*, double> *>(link, new std::map <_Link_Interface*, double>()));

		for (auto lnk_itr2 = selected_links->begin(); lnk_itr2 != selected_links->end(); lnk_itr2++)
		{
			_Link_Interface* link2 = *lnk_itr2;
			(*floyd_warshall_map)[link]->insert(std::pair <_Link_Interface*, double>(link2, 10000000.0));
		}

		auto travel_time = link->realtime_link_moe_data.link_travel_time;
		//Intersection<typename MasterType::intersection_type>* upstream_intersection = link->template upstream_intersection<Intersection<typename MasterType::intersection_type>*>();
		Intersection<typename MasterType::intersection_type>* downstream_intersection = link->template downstream_intersection<Intersection<typename MasterType::intersection_type>*>();
		_Links_Container_Interface::iterator links_itr2;
		for (links_itr2 = downstream_intersection->template outbound_links<_Links_Container_Interface&>().begin(); links_itr2 != downstream_intersection->template outbound_links<_Links_Container_Interface&>().end(); ++links_itr2)
		{
			//->template outbound_links<_Links_Container_Interface&>()
			_Link_Interface* link2 = (_Link_Interface*) (*links_itr2);
			
			(*(*floyd_warshall_map)[link])[link2] = travel_time;
		}
	}
	

	//Looping through edges and calculating the shortest paths cost
	for (auto lnk_itr1 = selected_links->begin(); lnk_itr1 != selected_links->end(); lnk_itr1++)
	{
		auto link1 = *lnk_itr1;
		for (auto lnk_itr2 = selected_links->begin(); lnk_itr2 != selected_links->end(); lnk_itr2++)
		{
			auto link2 = *lnk_itr2;
			for (auto lnk_itr3 = selected_links->begin(); lnk_itr3 != selected_links->end(); lnk_itr3++)
			{				
				auto link3 = *lnk_itr3;
				//Lets see if  travelling from link2 to link3 through link1 gives a better cost
				auto new_cost = (*(*floyd_warshall_map)[link2])[link1] + (*(*floyd_warshall_map)[link1])[link3];
				if (new_cost < (*(*floyd_warshall_map)[link2])[link3])
				{
					(*(*floyd_warshall_map)[link2])[link3] = new_cost;
				}
			}
		}
	}
	


	//// getting access to result database 
	//shared_ptr<odb::database> db_ptr = ((_Scenario_Interface*)_global_scenario)->template result_db_ptr<shared_ptr<odb::database>>();



	//typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename _Network_Interface::get_type_of(links_container)::value_type>::type>  _Link_Interface;
	//typedef  Random_Access_Sequence< type_of(links_container), _Link_Interface*> _Links_Container_Interface;

	//for (const auto& link : network->links_container<_Links_Container_Interface&>())
	//{
		//link->Initialize<NULLTYPE>();
		//_Link_Interface* link = (_Link_Interface*)_inbound_link_reference;

		//int num_lanes = link->template num_lanes<int>();

		// skip non-arterial/local links

		//Link_Components::Types::Link_Type_Keys link_type = link->template link_type<Link_Components::Types::Link_Type_Keys>();

		//// skip non-arterial/local links
		//if (link_type == Link_Components::Types::Link_Type_Keys::FREEWAY ||
		//	link_type == Link_Components::Types::Link_Type_Keys::EXPRESSWAY ||
		//	link_type == Link_Components::Types::Link_Type_Keys::ON_RAMP ||
		//	link_type == Link_Components::Types::Link_Type_Keys::OFF_RAMP)
		//{
		//	return;
		//}

	//}

	#pragma endregion

}


void select_platooning_vehicles(MasterType::scenario_type* scenario)
{
	//if we load demand from database
	if (scenario->read_demand_from_database<bool>())
	{
		typedef Demand_Components::Prototypes::Demand<MasterType::demand_type> _Demand_Interface;
		_Demand_Interface* demand = (_Demand_Interface*)_global_demand;
		
		typedef Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> _Vehicle_Interface;
		auto vehicles_container = demand->template vehicles_container<_Demand_Interface::vehicles_container_type>();
		//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
		//typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;


		//typedef Vehicle_Components::Prototypes::Vehicle<typename get_component_type(_Vehicles_Container_Interface)> _Vehicle_Interface;

		for (auto veh_it = vehicles_container.begin(); veh_it != vehicles_container.end(); veh_it++)
		{
			auto veh = *veh_it;

		}

		//typedef Random_Access_Sequence< typename _Demand_Interface::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
		//_Demand_Interface::vehicles_container
		//typedef Random_Access_Sequence< typename household_itf::get_type_of(Vehicles_Container)> _Vehicles_Container_Interface;
		//typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
		//typedef  Random_Access_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;
		//auto xuz = demand->template vehicles_container<_Vehicles_Container_Interface&>();
		//vehicles_container<_Vehicles_Container_Interface&>()
		//for (Vehicles_Container_type::iterator v_itr = _Vehicles_Container.begin(); v_itr != _Vehicles_Container.end(); ++v_itr)
		//{

		//}
		

	}

	//if demand is not read from database

}

void generate_PQ_file()
{

}

void fill_floyed_warshal_table()
{


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
	fw.Write_Line("\t\"output_dir_name\" : \"Platooning_Preprocessor\",");
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
	fw.Write_Line("\t\"time_dependent_routing\" : true,");
	fw.Write_Line("\t\"write_demand_to_database\" : true\n}");
	
	fw.Close();
}