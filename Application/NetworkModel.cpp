#include "Model_Selection.h"
#define EXCLUDE_DEMAND
#ifdef NetworkModelApplication
#define FOR_LINUX_PORTING
//#define DBIO
#ifdef DBIO

#include "Application_Includes.h"
#include "../File_IO/network_models.h"
struct MasterType
{

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
	
	typedef Network_Components::Implementations::Polaris_Network_Implementation<MasterType> network_type;
	
	typedef Intersection_Components::Implementations::Polaris_Intersection_Implementation<MasterType> intersection_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> movement_type;
	
	typedef Link_Components::Implementations::Polaris_Link_Implementation<MasterType> link_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> turn_movement_type;
	
	typedef Vehicle_Components::Implementations::Polaris_Vehicle_Implementation<MasterType> vehicle_type;

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

	typedef Zone_Components::Implementations::Polaris_Zone_Implementation<MasterType> zone_type;

	typedef Plan_Components::Implementations::Polaris_Plan_Implementation<MasterType> plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Implementation<MasterType> movement_plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;

//#ifndef FOR_LINUX_PORTING
	typedef Person_Components::Implementations::Person_Implementation<MasterType, demand_type> person_type;

    typedef Person_Components::Implementations::TRANSIMS_Person_Planner_Implementation<MasterType, person_type> person_planner_type;

    typedef Person_Components::Implementations::TRANSIMS_Person_Properties_Implementation<MasterType,person_type> person_properties_type;

    typedef RNG_Components::Implementations::RngStream_Implementation<MasterType> RNG;

    typedef Activity_Components::Implementations::Activity_Plan_Implementation<MasterType,person_type> activity_plan_type;
//#endif


};

ostream* stream_ptr;

int main()
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
	network->write_network_data<Target_Type<NULLTYPE,NULLTYPE,network_models::network_information::network_data_information::NetworkData&>>(network_data_for_output);
	network_models::network_information::network_data_information::write_network_data("", network_data_for_output);
	//cout<<"writing network data..."<<endl;
	//network_models::network_information::network_data_information::write_network_data(output_dir_name,network_data_for_output);


	//cout << "initializing simulation..." <<endl;	
	network->simulation_initialize<NULLTYPE>();

	define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
	_Demand_Interface* demand = (_Demand_Interface*)Allocate<typename MasterType::demand_type>();
	demand->scenario_reference<_Scenario_Interface*>(scenario);
	demand->network_reference<_Network_Interface*>(network);
	cout << "reading demand data..." <<endl;
	demand->read_demand_data<Net_IO_Type>(network_io_maps);
	cout << "converting demand data..." << endl;
	demand->write_demand_data<NULLTYPE>(demand_data_for_output);
	cout<<"writing demand data..."<<endl;
	network_models::network_information::demand_data_information::write_demand_vehicle(output_dir_name,scenario_data_for_output,demand_data_for_output,network_data_for_output);


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
#define COMPLETE_WORK
#ifdef COMPLETE_WORK	
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

	START();

	cout << "Finished!" << endl;
	system("PAUSE");
#endif
}
#endif

#define FILE_IO
#ifdef FILE_IO

#include "Application_Includes.h"
#include "../File_IO/network_models.h"
struct MasterType
{

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
	
	typedef Network_Components::Implementations::Polaris_Network_Implementation<MasterType> network_type;
	
	typedef Intersection_Components::Implementations::Polaris_Intersection_Implementation<MasterType> intersection_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> movement_type;
	
	typedef Link_Components::Implementations::Polaris_Link_Implementation<MasterType> link_type;
	
	typedef Turn_Movement_Components::Implementations::Polaris_Movement_Implementation<MasterType> turn_movement_type;
	
	typedef Vehicle_Components::Implementations::Polaris_Vehicle_Implementation<MasterType> vehicle_type;

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

	typedef Zone_Components::Implementations::Polaris_Zone_Implementation<MasterType> zone_type;

	typedef Plan_Components::Implementations::Polaris_Plan_Implementation<MasterType> plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Movement_Plan_Implementation<MasterType> movement_plan_type;

	typedef Movement_Plan_Components::Implementations::Polaris_Trajectory_Unit_Implementation<MasterType> trajectory_unit_type;

	typedef Traveler_Components::Implementations::Polaris_Traveler_Implementation<MasterType> person_type;

    typedef Person_Components::Implementations::TRANSIMS_Person_Planner_Implementation<MasterType, person_type> person_planner_type;

    typedef Person_Components::Implementations::TRANSIMS_Person_Properties_Implementation<MasterType,person_type> person_properties_type;

    typedef RNG_Components::Implementations::RngStream_Implementation<MasterType> RNG;

    typedef Activity_Components::Implementations::Activity_Plan_Implementation<MasterType,person_type> activity_plan_type;


};

ostream* stream_ptr;

int main()
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

	//converting between k and p
	network_models::network_information::scenario_data_information::ScenarioData scenario_data_for_output;
	network_models::network_information::network_data_information::NetworkData network_data_for_output;
	network_models::network_information::demand_data_information::DemandData demand_data_for_output;
	network_models::network_information::operation_data_information::OperationData operation_data_for_output;
	network_models::initialization(scenario_data_for_output,network_data_for_output,demand_data_for_output,operation_data_for_output);

	cout << "reading input data..." <<endl;	
	define_component_interface(_Scenario_Interface, MasterType::scenario_type, Scenario_Prototype, NULLTYPE);
	_Scenario_Interface* scenario=(_Scenario_Interface*)Allocate<typename MasterType::scenario_type>();
	_global_scenario = scenario;
	scenario->read_scenario_data<Scenario_Components::Types::File_Scenario>(scenario_data);
	//scenario->write_scenario_data<NULLTYPE>(scenario_data_for_output);

	define_component_interface(_Network_Interface, MasterType::network_type, Network_Prototype, NULLTYPE);
	_Network_Interface* network = (_Network_Interface*)Allocate<typename MasterType::network_type>();
	_global_network = network;
	network->scenario_reference<_Scenario_Interface*>(scenario);
	network->read_network_data<Net_IO_Type>(network_data);
	//network->write_network_data<NULLTYPE>(network_data_for_output);

	define_component_interface(_Demand_Interface, MasterType::demand_type, Demand_Prototype, NULLTYPE);
	_Demand_Interface* demand = (_Demand_Interface*)Allocate<typename MasterType::demand_type>();
	
	demand->scenario_reference<_Scenario_Interface*>(scenario);
	demand->network_reference<_Network_Interface*>(network);
	demand->read_demand_data<Network_Components::Types::File_Network>(scenario_data, network_data, demand_data);
	//demand->write_demand_data<NULLTYPE>(demand_data_for_output);


	define_component_interface(_Operation_Interface, MasterType::operation_type, Operation_Components::Prototypes::Operation_Prototype, NULLTYPE);
	_Operation_Interface* operation = (_Operation_Interface*)Allocate<typename MasterType::operation_type>();
	operation->network_reference<_Network_Interface*>(network);
	operation->read_operation_data<Operation_Components::Types::File_Operation>(scenario_data, network_data, operation_data);
	//operation->write_operation_data<NULLTYPE>(network_data_for_output, operation_data_for_output);
	
	//output data
	//network_models::write_data("",scenario_data_for_output,demand_data_for_output,network_data_for_output, operation_data);


	network->simulation_initialize<NULLTYPE>();
	
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

	START();

	cout << "Finished!" << endl;
	system("PAUSE");
}
#endif
#endif