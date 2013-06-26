#ifndef NETWORK_TURN_MOVEMENT_DATA_H
#define NETWORK_TURN_MOVEMENT_DATA_H
#include "utilities.h"

using namespace std;

namespace network_models
{

	namespace network_information
	{
		namespace network_data_information
		{
			enum Turn_Movement_Type_Keys
			{
				LEFT_TURN=0,
				THROUGH_TURN,
				RIGHT_TURN,
				U_TURN,
				MERGE_RIGHT,
				MERGE_LEFT,
				DIVERGE_RIGHT,
				DIVERGE_LEFT,
			};

			enum Turn_Movement_Rule_Keys
			{
				PROHIBITED=0,
				ALLOWED,
			};

			struct TurnMovementData
			{
				int uuid;
				int turn_movement_index;

				int inbound_link_index;
				int outbound_link_index;

				Turn_Movement_Type_Keys turn_movement_type; ///LT; TH; RT; UT
				Turn_Movement_Rule_Keys turn_movement_rule; ///prohibited; allowed;

				int num_turn_lanes;
			};
		}
	}
}
#endif