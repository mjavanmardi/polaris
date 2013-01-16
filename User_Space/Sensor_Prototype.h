#pragma once
#include "User_Space_Includes.h"

namespace Sensor_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Sensor:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(operation,none,none);
			feature_accessor(malfunction,none,none);
			feature_accessor(quality,none,none);

			feature_accessor(location,none,none);

			feature_accessor(volumes,none,none);
			feature_accessor(occupancies,none,none);
			feature_accessor(counts,none,none);

			feature_accessor(storage_start,none,none);
			feature_accessor(storage_interval,none,none);
			feature_accessor(storage_memory,none,none);

			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			declare_feature_conditional(Maintenance_Condition)
			{
				response.next._iteration=_iteration+((Sensor*)_this)->storage_interval<int>();
				response.next._sub_iteration=0;

				response.result=true;
			}

			declare_feature_event(Maintenance)
			{
				((Sensor*)_this)->Perform_Maintenance<NULLTYPE>();
			}

			feature_prototype void Perform_Maintenance()
			{
				// Make interface to counts
				define_simple_container_interface(Counts_Interface,get_type_of(counts),Random_Access_Sequence_Prototype,typename get_type_of(counts)::unqualified_value_type,NULLTYPE);
				Counts_Interface* counts_container=counts<Counts_Interface*>();

				// Push_back a new member
				counts_container->push_back( get_type_of(counts)::unqualified_value_type() );

				// If the container is too large, pop_front
				if( counts_container->size() > storage_memory<int>() )
				{
					counts_container->pop_front();
					storage_start<int&>()+=storage_interval<int>();
				}
			}

			feature_prototype typename TargetType::ReturnType Average(typename TargetType::ParamType time_start,typename TargetType::Param2Type time_end)
			{
				return this_component()->Average<ComponentType,CallerType,TargetType>(time_start,time_end);
			}

			feature_prototype void Push_Data(typename TargetType::ParamType value, typename TargetType::Param2Type time)
			{
				// Make interface to counts
				define_simple_container_interface(Counts_Interface,get_type_of(counts),Random_Access_Sequence_Prototype,typename get_type_of(counts)::unqualified_value_type,NULLTYPE);
				Counts_Interface& counts_container = counts<Counts_Interface&>();

				// Compute target block, ensure in range
				int block = ( time - storage_start<int>() ) / storage_memory<int>();

				assert( block < counts_container.size() && block >= 0 );

				counts_container[ block ]+=value;
			}
			
			feature_prototype typename TargetType::ReturnType Pull_Data(typename TargetType::ParamType time_start,typename TargetType::Param2Type time_end)
			{
				return this_component()->Pull_Data<ComponentType,CallerType,TargetType>(time_start,time_end);
			}

			feature_prototype void Push_Data(typename TargetType::ParamType value,typename TargetType::Param2Type time_start,typename TargetType::Param3Type time_end)
			{
				this_component()->Push_Data<ComponentType,CallerType,TargetType>(value,time_start,time_end);
			}
		};
	}
}

using namespace Sensor_Components::Prototypes;