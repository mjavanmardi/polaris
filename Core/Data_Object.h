#pragma once
#include "Memory_Engine.h"

///============================================================================
/// Forward declaration of Typed_Data_Pages and Data_Root
///============================================================================

template<typename DataType=NULLCLASS>
class Typed_Data_Pages;

class Data_Root;

///============================================================================
/// Data_Object
/// A basic object chunk with associated type, which is available for allocation
///============================================================================

struct Data_Object
{
	permit_state_checking;
	
	union
	{
		Event event_register;

		struct
		{
			Data_Object* next_free_cell;
		};
	};
};

