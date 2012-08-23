#pragma once

#include "Repository_Base_Includes.h"

//=============================================================
/// POLARIS SPATIAL INTERFACES
namespace Spatial_API
{
	namespace Types
	{
		namespace Coordinate_Types
		{
			typedef true_type XY_Type;
			typedef true_type UTM_Type;
			typedef true_type State_Plane_Type;
		}
	}
	namespace Concepts
	{
		concept Is_XY
		{
			begin_requirements_list;
			requires_typename_state(none, XY_Type, true_type,"This class is not XY coordinate type");
			end_requirements_list(XY_Type);
		};
		concept Is_UTM
		{
			begin_requirements_list;
			requires_typename_state(none, UTM_Type, true_type,"This class is not UTM coordinate type");
			end_requirements_list(UTM_Type);
		};
		//concept Is_Vadim_Location
		//{
		//	begin_requirements_list;
		//	requires_typename_state(none, Vadim_Location,true_type,"Type is not a Vadim Location");
		//	requires_typed_member(Vadim_Location,location,char*,"A Vadim_Location needs to have a string 'location' member.");
		//	end_requirements_list(location);
		//};
		//concept Is_Transims_Location
		//{
		//	begin_requirements_list;
		//	requires_typename_state(none,Transims_Location,true_type,"Type is not a Transims Location");
		//	requires_typed_member(Transims_Location,location,int,"A Transims_Location needs to have an int 'location' member.");
		//	end_requirements_list(location);
		//};
	}
	namespace Interfaces
	{
		/// Location Interface class
		struct Coordinate_System_Interface
		{
			facet void Initialize(float* var, TargetType& origin_coord_struct, call_requires(ThisType,Is_Dispatchable))
			{
				this->ptr_test<ThisType,CallerType,float*>(var);
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>(var, origin_coord_struct);
			}
			template<typename ThisType, typename CallerType, typename CoordinateStructType, typename ZoneStructType>
			void Initialize(CoordinateStructType& origin_coord_struct, ZoneStructType& zone_struct, call_requires(ThisType,Is_Dispatchable))
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,CoordinateStructType,ZoneStructType>(origin_coord_struct,zone_struct);
			}

			facet void Write(call_requirements(requires(TargetType, is_arithmetic) || requires_2(TargetType,string,is_same)))
			{
				TargetType l = location<ThisType,CallerType,TargetType>();
				cout << l;
			}

			facet void Convert()
			{
				int* test = this->ptr_test<ThisType,CallerType,int*>();
				cout <<*test;
			}
			facet void Distance(){}

			facet_accessor(ptr_test);
			facet_accessor(Origin_X);
			facet_accessor(Origin_Y);
			facet_accessor(Zone);
			facet_accessor(Reference);
		};
	}
	namespace Data_Structures
	{
		struct XY_Coord_Struct
		{
			typedef Spatial_API::Types::Coordinate_Types::XY_Type XY_Type;
			float x;
			float y;
		};
		
		struct UTM_Zone
		{
			typedef Spatial_API::Types::Coordinate_Types::UTM_Type UTM_Type;
			int zone;
			char dir;
		};
		struct State_Plane_Zone
		{
			typedef Spatial_API::Types::Coordinate_Types::State_Plane_Type State_Plane_Type;
			int zone;
		};

		struct UTM_Coord_Struct
		{
			typedef Spatial_API::Types::Coordinate_Types::UTM_Type UTM_Type;
			int zone;
			char dir;
		};
		struct State_Plane_Coord_Struct
		{
			typedef Spatial_API::Types::Coordinate_Types::State_Plane_Type State_Plane_Type;
			string zone;
		};

	}
}