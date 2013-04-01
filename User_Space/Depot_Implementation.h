#pragma once
#include "Depot_Prototype.h"
#include "Geometry_Implementation.h"

namespace Depot_Components
{
	namespace Types{}

	namespace Concepts{}
	
	namespace Implementations
	{
		implementation struct Tow_Truck_Depot:public Polaris_Component<APPEND_CHILD(Tow_Truck_Depot),MasterType,Data_Object>
		{
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(vector<Point_3D<MasterType>>, covered_area, none, none);
			member_data(vector<string>, active_properties, none, none);
		};
	}
}

using namespace Depot_Components::Implementations;