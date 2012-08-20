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

	template<typename T> struct allocator_template{static Typed_Data_Pages<T> allocator_reference;};

	union
	{
		Event event_register;

		struct
		{
			Data_Object* next_free_cell;
		};
	};
};

template<typename T>
Typed_Data_Pages<T> Data_Object::allocator_template<T>::allocator_reference=Typed_Data_Pages<T>();




















///============================================================================
/// Forward declaration of Typed_Data_Pages
///============================================================================

//template<typename DataType=NULLTYPE>
//class Typed_Data_Pages;

///============================================================================
/// Data_Object
/// A basic object chunk with associated type, which is available for allocation
///============================================================================

//template<typename DataType=NULLTYPE>
//struct Data_Object
//{
//	static Typed_Data_Pages<DataType> allocator_reference;
//	
//	Data_Object* next_free_cell;
//};
//
//template<typename DataType>
//Typed_Data_Pages<DataType> Data_Object<DataType>::allocator_reference=Typed_Data_Pages<DataType>();

///============================================================================
/// Data_Object_Active
/// A basic object data chunk which has been allocated
///============================================================================

//template<typename DataType=NULLTYPE>
//struct Data_Object_Active
//{
//	static Typed_Data_Pages<DataType> allocator_reference;
//};
//
//template<typename DataType>
//Typed_Data_Pages<DataType> Data_Object<DataType>::allocator_reference=Typed_Data_Pages<DataType>();