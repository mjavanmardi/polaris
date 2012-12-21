#ifndef NETWORK_ASSIGNMENT_MODELS_H
#define NETWORK_ASSIGNMENT_MODELS_H
#include "network_information.h"
#include "network_routing_algorithms.h"
//#include "utilities.h"

using namespace network_models::network_routing_algorithms;

namespace network_models
{
	namespace network_assignment_models
	{
		class AssignmentModel
		{
		private:

		public:
			AssignmentModel(void);
			~AssignmentModel(void);
		
			//route choice model
			void pretrip_route_choice_model(VehicleData& vehicle_data,RoutingAlgorithm& routing_algorithm, NetworkData& network_data);
			void enroute_switching_model(VehicleData& vehicle_data,RoutingAlgorithm& routing_algorithm);
			void user_equilibrium_route_choice_model(VehicleData& vehicle_data,RoutingAlgorithm& routing_algorithm);
			void system_optimal_route_choice_model(VehicleData& vehicle_data,RoutingAlgorithm& routing_algorithm);

			//Dynamic Traffic Assignment Mode
			void one_shot_assignment_model
				(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data);
			void user_equilibrium_assignment_model
				(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data);
			void system_optimal_assignment_model
				(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data);
			void multiple_user_class_dynamic_traffic_assignment_model
				(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data);

			//
		};
	}
}
#endif