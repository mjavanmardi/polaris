#pragma once

using namespace std;

//---------------------------------------------------------
//	SUMMARY FILE CLASS
//---------------------------------------------------------
namespace Scenario_Components
{
	namespace Types
	{
		struct Hard_Coded_Scenario{};
		struct ODB_Scenario{};
		struct File_Scenario{};

			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
			};
			
			enum Type_Sub_Iteration_keys
			{
				START_OF_ITERATION = 0, 
				NETWORK_SNAPSHOT_SUB_ITERATION, //outputs the snapshot from previous iteration
				EVENTS_UPDATE_SUB_ITERATION, //for the wether events, at this point they notify subscribing links that the event occured
				ROUTING_SUB_ITERATION, //for any route computations
				TRAVELER_SET_DEPARTURE_SUB_ITERATION, //traveler get pushed to the network
				RAMP_METERING_SUB_ITERATION, //updates the ramp meeters according to current conditions
				CONTROL_SUB_ITERATION, //traffic signal states updated
				/* update link supply */
				LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,//network simulation
				/* 1. update allowed flow for each turn movement.
				 * 2. transfer vehicle */
				INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,//network simulation
				INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION,//network simulation
				/* vehicles take action for transferring */
				VEHICLE_ACTION_TRANSFER_SUB_ITERATION,//network simulation
				/* origin link loading */
				LINK_ORIGIN_LINK_LOADING_SUB_ITERATION,//network simulation
				/* vehicles take action for origin loading */
				VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION,//network simulation
				/* link moving */
				LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION,//network simulation
				INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION,//network simulation
				INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION,//computing MOE
				INTERSECTION_MOE_COMPUTATION_SUB_ITERATION,//computing MOE
				MOE_COMPUTATION_SUB_ITERATION,//computing MOE
				MOE_VISUALIZATION_SUB_ITERATIONS,//computing MOE
				END_OF_ITERATION=40,
				OUTPUT_WRITING_SUB_ITERATION,
			};
			enum Demand_Sub_Iteration_keys
			{
				PRETRIP_INFORMATION_ACQUISITION_SUB_ITERATION = 14,
				PRETRIP_PLANNING_SUB_ITERATION,
				PRETRIP_ROUTING_SUB_ITERATION,
				ACTIVITY_ATTRIBUTE_PLANNING_SUB_ITERATION,
				MOVEMENT_SUB_ITERATION,
				ACTIVITY_GENERATION_SUB_ITERATION=19,
				ACTIVITY_PLANNING_SUB_ITERATION=20,
				MOVEMENT_PLANNING_SUB_ITERATION=21,
			};
			enum RNG_Type_Keys
			{
				DETERMINISTIC = 0,
				RANDOM,
			};

			enum Merging_Mode_Keys
			{
				DRIVING_RULE = 0,
				PROPORTION_TO_DEMAND,
				PROPORTION_TO_LINK,
				PROPORTION_TO_LANE,
			};

			enum Cause_For_Enroute_Switching
			{
				EXCESSIVE_DELAY = 0,
				REALTIME_INFORMED,
				ITS_INFORMED
			};

			enum Area_Type_Keys
			{

			};
	}
}