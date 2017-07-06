#pragma once
#include "Traveler_Simulator_Includes.h"
#include "antares\Geometry_Implementation.h"

namespace Vehicle_Components
{

namespace Prototypes
{
	prototype struct Vehicle_Data_Logger ADD_DEBUG_INFO
	{
		tag_as_prototype;
		void Add_Record(Point_3D<typename ComponentType::Master_Type> &point, True_Color_RGBA<NT> &color)
		{
			this_component()->Add_Record(point,color);
		}
		
		template<typename TargetType> void Write_Data_To_File()
		{
			this_component()->template Write_Data_To_File<TargetType>();
		}
		template<typename TargetType> void Initialize(bool read=false, string filename = "snapshot.bin")
		{
			this_component()->template Initialize<TargetType>(read,filename);
		}

		template<typename TargetType> void Increment_Cancelled_Activities()
		{
			this_component()->template Increment_Cancelled_Activities<TargetType>();
		}
		template<typename TargetType> void Increment_Replanned_Activities()
		{
			this_component()->template Increment_Replanned_Activities<TargetType>();
		}

		accessor(Next_Logging_Time, NONE, NONE);
		accessor(Logging_Interval, NONE, NONE);
		accessor(Read, NONE, NONE);
	};
}

}
