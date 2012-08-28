#pragma once

#include "Spatial_Interfaces.h"


namespace Spatial_Components
{
	namespace Bases
	{
		//================================================================
		/// Base for the XY coordinate system class
		class XY_Coordinate_System_Base
		{
			facet void Initialize(float* var, TargetType& orig, call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,Concepts::Is_XY)))
			{
				this->_ptr_test = var;
				this->_Origin_X=orig.x;
				this->_Origin_Y=orig.y;
			}
			facet void Initialize(call_requirements(requires(ThisType,!Is_Dispatched) || requires(TargetType,!Concepts::Is_XY)))
			{
				assert_requirements(TargetType,Is_Transims_Location,"TargetType is not a transims location - required to use this version of initialize");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
			}

			member_data(float*,ptr_test,requires(TargetType,is_arithmetic),false);
			member_data(float,Origin_X,requires(TargetType,is_arithmetic),false);
			member_data(float,Origin_Y,requires(TargetType,is_arithmetic),false);
			declare_facet_getter(Zone,true)
			{
				static_assert(false,"XY coordinate system class has no 'Zone' member.");
			}
			declare_facet_getter(Reference,true)
			{
				static_assert(false,"XY coordinate system class has no 'Reference' sphere member defined.");
			}

		};

	
		//================================================================
		/// Base for the XY coordinate system class
		class UTM_Coordinate_System_Base
		{
			facet
			void Initialize(TargetType& zone_struct, call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(TargetType,Concepts::Is_UTM)))
			{

			}
			template<typename ThisType, typename CallerType, typename CoordinateStructType, typename ZoneStructType>
			void Initialize(CoordinateStructType& origin_coord_struct, ZoneStructType& zone_struct, call_requirements(
				requires(ThisType,Is_Dispatched) &&
				requires(CoordinateStructType,Concepts::Is_UTM) &&
				requires(ZoneStructType,Concepts::Is_UTM)))
			{

			}
			template<typename ThisType, typename CallerType, typename CoordinateStructType, typename ZoneStructType>
			void Initialize(CoordinateStructType& origin_coord_struct, ZoneStructType& zone_struct, call_requirements(
				requires(ThisType,!Is_Dispatched) || 
				requires(CoordinateStructType,!Concepts::Is_UTM)))
			{
				assert_requirements(TargetType,Is_Transims_Location,"TargetType is not a transims location - required to use this version of initialize");
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
			}

			declare_facet_getter(Origin_X,requires(TargetType,is_arithmetic))
			{
				return _x;
			}
			declare_facet_getter(Origin_Y,requires(TargetType,is_arithmetic))
			{
				return _y;
			}
			declare_facet_getter(Zone,true)
			{
				static_assert(false,"XY coordinate system class has no 'Zone' member.");
			}
			declare_facet_getter(Reference,true)
			{
				static_assert(false,"XY coordinate system class has no 'Reference' sphere member defined.");
			}

		protected:
			float _x;
			float _y;
		};	
	}


	namespace Components
	{
		typedef Polaris_Component<Interfaces::Coordinate_System_Interface,Bases::XY_Coordinate_System_Base> XY_Coordinate_System;
		typedef Polaris_Component<Interfaces::Coordinate_System_Interface,Bases::UTM_Coordinate_System_Base> UTM_Coordinate_System;
	}
}
