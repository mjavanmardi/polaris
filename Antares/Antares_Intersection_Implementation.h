//*********************************************************
//	Graphical_Intersection_Implementation.h - Graphical Intersection Variant
//*********************************************************

#pragma once
#include "Antares_Includes.h"
#include "User_Space\Polaris_Intersection_Implementation.h"

namespace Intersection_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Antares_Intersection_Implementation:public Polaris_Intersection_Implementation<MasterType,ParentType,APPEND_CHILD(Antares_Intersection_Implementation)>
		{
			static bool fetch_attributes(Antares_Intersection_Implementation* _this,vector<string>& bucket)
			{
				stringstream s;
				s << _this->_internal_id;
				bucket.push_back(s.str());
				s.str("");
			
				switch(_this->_intersection_type)
				{
				case Types::Intersection_Type_Keys::NO_CONTROL:
					bucket.push_back("NO_CONTROL");
					break;
				case Types::Intersection_Type_Keys::YIELD_SIGN:
					bucket.push_back("YIELD_SIGN");
					break;
				case Types::Intersection_Type_Keys::ALL_WAY_STOP_SIGN:
					bucket.push_back("ALL_WAY_STOP_SIGN");
					break;
				case Types::Intersection_Type_Keys::TWO_WAY_STOP_SIGN:
					bucket.push_back("TWO_WAY_STOP_SIGN");
					break;
				case Types::Intersection_Type_Keys::PRE_TIMED_SIGNAL_CONTROL:
					bucket.push_back("PRE_TIMED_SIGNAL_CONTROL");
					break;
				case Types::Intersection_Type_Keys::ACTUATED_SIGNAL_CONTROL:
					bucket.push_back("ACTUATED_SIGNAL_CONTROL");
					break;
				case Types::Intersection_Type_Keys::ADAPTIVE_SIGNAL_CONTROL:
					bucket.push_back("ADAPTIVE_SIGNAL_CONTROL");
					break;
				default:
					bucket.push_back("NON_RECOGNIZED");
					break;
				}
				return true;
			}
		};
	}
}

using namespace Intersection_Components::Implementations;
