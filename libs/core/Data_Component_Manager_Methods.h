#pragma once
///----------------------------------------------------------------------------------------------------
/// Data_Component_Manager_Methods.h - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "Data_Component_Manager.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Allocate - Allocate a Data object of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Data_Component_Manager<DataType>::Allocate(int uuid)
	{
		DataType* return_memory = new DataType();
		
		// add information about the uuid
		if(uuid!=-1) _object_repository[__thread_id][uuid] = return_memory;
		
		((DataType*)return_memory)->_uuid = uuid;

		return return_memory;
	}

	///----------------------------------------------------------------------------------------------------
	/// Allocate_Array - Allocate Data objects of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Data_Component_Manager<DataType>::Allocate_Array( unsigned int num )
	{
		return new DataType[num]();
	}

	///----------------------------------------------------------------------------------------------------
	/// Free - Deallocate a Data object of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Data_Component_Manager<DataType>::Free( DataType* ptr )
	{
		delete ptr;
	}

	///----------------------------------------------------------------------------------------------------
	/// Lazy_Free - Currently identical to free for data objects
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Data_Component_Manager<DataType>::Lazy_Free( DataType* ptr )
	{
		delete ptr;
	}

	///----------------------------------------------------------------------------------------------------
	/// Free_Array - Deallocate Data objects of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Data_Component_Manager<DataType>::Free_Array( DataType* ptr )
	{
		delete[] ptr;
	}
}