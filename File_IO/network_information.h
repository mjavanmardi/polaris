#ifndef NETWORK_INFORMATION_H
#define NETWORK_INFORMATION_H

#include "network_data.h"
#include "network_scenario_data.h"
#include "network_demand_data.h"
#include "network_operation_data.h"

#include "utilities.h"

using namespace std;
using namespace network_models::network_information::network_data_information;
using namespace network_models::network_information::demand_data_information;
using namespace network_models::network_information::scenario_data_information;
using namespace network_models::network_information::operation_data_information;

namespace network_models
	{
	namespace network_information {

		//information processing
		void information_statistics
			(ScenarioData& scenario_data, NetworkData& network_data, DemandData& demand_data);
		//io

	}
}

#endif