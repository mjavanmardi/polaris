#pragma once
///----------------------------------------------------------------------------------------------------
/// Data_Component_Manager.h - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "Execution_Component_Manager.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Data_Component_Manager_Base - Base manager defines members and provides general interface
	///----------------------------------------------------------------------------------------------------

	class Data_Component_Manager_Base : public Component_Manager_Base
	{
	public:
		virtual void Initialize(){ _object_repository = new std::unordered_map<int,void*>[num_sim_threads() + 1]; }
		virtual void Terminate(){}
		virtual Component_Manager_Types Component_Manager_Type(){ return DATA_MANAGER; }
	};

	///----------------------------------------------------------------------------------------------------
	/// Data_Component_Manager - Implements type-specific allocation
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	class Data_Component_Manager : public Data_Component_Manager_Base
	{
	public:
		typedef DataType managed_type;

		DataType* Allocate( int pid = -1, bool bInPlaceNew = true );
		DataType* Allocate_Array( unsigned int num );

		void Free( DataType* ptr );
		void Lazy_Free( DataType* ptr );
		void Free_Array( DataType* ptr );
	};
}