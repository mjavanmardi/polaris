#pragma once
///----------------------------------------------------------------------------------------------------
/// Data_Object.h - Top level data which passes through to Base_Object
///----------------------------------------------------------------------------------------------------

#include "Execution_Object.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Data_Object - Data object which passes through to Base_Object
	///----------------------------------------------------------------------------------------------------

	class Data_Object : public Base_Object
	{
	public:
		Data_Object():Base_Object(){}
		Data_Object(size_t component_id,int pid = -1):Base_Object(component_id, pid){}
	};
}