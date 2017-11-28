#pragma once
///----------------------------------------------------------------------------------------------------
/// Polaris_Component.h - Definition of POLARIS Component
///----------------------------------------------------------------------------------------------------

#include "Container_Forms.h"
#include "Data_Component_Manager.h"
#include "Execution_Component_Manager.h"
#include "Event_Component_Manager.h"
#include "Data_Object.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Get_Component_Manager - convert object type to appropriate manager class
	///----------------------------------------------------------------------------------------------------

	template<typename ObjectType,typename ComponentType>
	struct Get_Component_Manager{};

	template<typename ComponentType>
	struct Get_Component_Manager<Data_Object,ComponentType>
	{
		typedef Data_Component_Manager<ComponentType> type;
	};

	template<typename ComponentType>
	struct Get_Component_Manager<Execution_Object,ComponentType>
	{
		typedef Execution_Component_Manager<ComponentType> type;
	};
	
	template<typename ComponentType>
	struct Get_Component_Manager<Event_Object,ComponentType>
	{
		typedef Event_Component_Manager<ComponentType> type;
	};

	template<typename ComponentType>
	struct Get_Component_Manager<NULLTYPE,ComponentType>
	{
		typedef NULLTYPE type;
	};
	
	///----------------------------------------------------------------------------------------------------
	/// Add_Component_Manager - compile-time method to add component managers to all_components
	///----------------------------------------------------------------------------------------------------

	template<typename Component_Manager_Type>
	static Component_Manager_Type* Add_Component_Manager(Component_Manager_Type* val, size_t component_id)
	{
		if(__all_components==nullptr) __all_components=new std::unordered_map<size_t, Component_Manager_Base*>();
		Component_Manager_Type::managed_type::component_id = component_id;
		(*__all_components)[component_id] = (Component_Manager_Base*)val;
		return val;
	}

	///----------------------------------------------------------------------------------------------------
	/// Polaris_Component - Basic hook for POLARIS utilities
	///----------------------------------------------------------------------------------------------------

	template<typename MasterType = NULLTYPE,typename InheritanceList = NULLTYPELIST,typename ObjectType = Data_Object>
	class Polaris_Component : public ObjectType
	{
	public:
		using ObjectType::_pid;
		using ObjectType::_component_id;

		Polaris_Component(int pid = -1):ObjectType(component_id, pid){}

		const int pid(){ return _pid; }
		const size_t Identify() const{return _component_id;}

		typedef MasterType Master_Type;

		typedef ObjectType Object_Type;

		typedef InheritanceList Inheritance_List;
		
		typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
		typedef Component_Type ComponentType;

		static size_t component_id;
		
		typedef typename Get_Component_Manager<ObjectType,Component_Type>::type Component_Manager_Type;

		static Component_Manager_Type* const component_manager;
	};


	
	template<typename MasterType,typename InheritanceList,typename ObjectType>
	typename Polaris_Component<MasterType,InheritanceList,ObjectType>::Component_Manager_Type* const Polaris_Component<MasterType,InheritanceList,ObjectType>::component_manager 
		= Add_Component_Manager( new Polaris_Component<MasterType,InheritanceList,ObjectType>::Component_Manager_Type(), ++__component_counter );
	
	template<typename MasterType,typename InheritanceList,typename ObjectType>
	size_t Polaris_Component<MasterType,InheritanceList,ObjectType>::component_id;

	template<typename MasterType,typename InheritanceList>
	class Polaris_Component<MasterType,InheritanceList,NT>
	{
	public:
		Polaris_Component(){}

		const int Identify() const{return -1;}

		typedef MasterType Master_Type;

		typedef InheritanceList Inheritance_List;
		
		typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
		typedef Component_Type ComponentType;

		static const size_t component_id;
	};

	template<typename MasterType,typename InheritanceList>
	const size_t Polaris_Component<MasterType,InheritanceList,NT>::component_id = -1;
}