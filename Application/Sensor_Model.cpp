#include "Model_Selection.h"

#ifdef SENSOR
#include "Application_Includes.h"
struct MasterType
{

};

int main()
{
	Sensor<Fixed_Sensor<MasterType>>* sensor_interface=(Sensor<Fixed_Sensor<MasterType>>*)Allocate<Fixed_Sensor<MasterType>>();
	
	sensor_interface->Initialize<NULLTYPE>();

	START();
}
#endif