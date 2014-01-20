#pragma once
///----------------------------------------------------------------------------------------------------
/// Polaris_Component.h - Definition of POLARIS Component
///----------------------------------------------------------------------------------------------------

#include "Container_Forms.h"
#include "Data_Component_Manager.h"
#include "Execution_Component_Manager.h"
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
	struct Get_Component_Manager<NULLTYPE,ComponentType>
	{
		typedef NULLTYPE type;
	};
	
	///----------------------------------------------------------------------------------------------------
	/// Add_Component_Manager - compile-time method to add component managers to all_components
	///----------------------------------------------------------------------------------------------------

	template<typename Component_Manager_Type>
	static Component_Manager_Type* Add_Component_Manager(Component_Manager_Type* val, int component_id)
	{
		__all_components[component_id] = (Component_Manager_Base*)val;
		return val;
	}

	///----------------------------------------------------------------------------------------------------
	/// Polaris_Component - Basic hook for POLARIS utilities
	///----------------------------------------------------------------------------------------------------

	template<typename MasterType = NULLTYPE,typename InheritanceList = NULLTYPELIST,typename ObjectType = Data_Object>
	class Polaris_Component : public ObjectType
	{
	public:
		Polaris_Component(int uuid = -1):ObjectType(component_id,uuid){}

		const int uuid(){ return _uuid; }
		const int Identify() const{return _component_id;}

		typedef MasterType Master_Type;

		typedef InheritanceList Inheritance_List;
		
		typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
		typedef Component_Type ComponentType;

		static const int component_id;
		
		typedef typename Get_Component_Manager<ObjectType,Component_Type>::type Component_Manager_Type;

		static Component_Manager_Type* const component_manager;
	};

	template<typename MasterType,typename InheritanceList,typename ObjectType>
	const int Polaris_Component<MasterType,InheritanceList,ObjectType>::component_id = typeid(Polaris_Component<MasterType,InheritanceList,ObjectType>).hash_code();
	
	template<typename MasterType,typename InheritanceList,typename ObjectType>
	typename Polaris_Component<MasterType,InheritanceList,ObjectType>::Component_Manager_Type* const Polaris_Component<MasterType,InheritanceList,ObjectType>::component_manager 
		= Add_Component_Manager( new Polaris_Component<MasterType,InheritanceList,ObjectType>::Component_Manager_Type(), Polaris_Component<MasterType,InheritanceList,ObjectType>::component_id );

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

		static const int component_id;
	};

	template<typename MasterType,typename InheritanceList>
	const int Polaris_Component<MasterType,InheritanceList,NT>::component_id = -1;
}