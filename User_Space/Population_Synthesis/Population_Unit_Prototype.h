#pragma once

#include "User_Space\User_Space_Includes.h"

using namespace std;

//---------------------------------------------------------
//	POP_UNIT OBJECT CLASS (i.e. HH, PERSON, VEHICLE, FIRM, etc.)
//---------------------------------------------------------

namespace PopSyn
{
	namespace Prototypes
	{
		prototype struct Population_Unit_Prototype : public ComponentType
		{
			tag_as_prototype;
			typedef Population_Unit_Prototype<ComponentType, CallerType> This_Type;
			feature_prototype void Initialize(TargetType object_to_copy/*, requires(check_2(TargetType,This_Type,is_same))*/)
			{
				uint i = object_to_copy.ID<get_type_of(ID)>();
				this->ID<get_type_of(ID)>(object_to_copy.ID<get_type_of(ID)>());
				this->Weight<get_type_of(Weight)>(object_to_copy.Weight<get_type_of(Weight)>());
				this->Index<get_type_of(Index)>(object_to_copy.Index<get_type_of(Index)>());

				typedef get_type_of(Characteristics)::unqualified_value_type value_type;
				define_simple_container_interface(characteristics_itf,get_type_of(Characteristics),Random_Access_Sequence_Prototype,value_type,NULLTYPE);
				characteristics_itf& data = this->Characteristics<characteristics_itf&>();
				characteristics_itf& obj_data = object_to_copy.Characteristics<characteristics_itf&>();
				characteristics_itf::iterator itr = obj_data.begin();
				for (itr; itr != obj_data.end(); ++itr) data.push_back(*itr);
			}
			feature_accessor(ID,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Weight,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Index,check(ReturnValueType,is_arithmetic),check(SetValueType,is_arithmetic));
			feature_accessor(Characteristics,none,none);
			feature_prototype void Normalize_Weight(TargetType normalization_factor, requires(check(TargetType,is_arithmetic)))
			{
				this->Weight<TargetType>(this->Weight<TargetType>()/normalization_factor);
			}
		};
	}
}

