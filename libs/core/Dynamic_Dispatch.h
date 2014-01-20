#pragma once
///----------------------------------------------------------------------------------------------------
/// Dynamic_Dispatch.h - POLARIS dynamic dispatch capability with re-bindable functions
///----------------------------------------------------------------------------------------------------

#include "Polaris_Component.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// static_hash_calc - compile time string hashing
	///----------------------------------------------------------------------------------------------------

	template<size_t N, size_t I=0>
	struct static_hash_calc
	{
		static unsigned int apply (const char (&s)[N])
		{
			return  (static_hash_calc<N, I+1>::apply(s) ^ s[I]) * 16777619u;
		};
	};

	template<size_t N>
	struct static_hash_calc<N,N>
	{
		static unsigned int apply (const char (&s)[N])
		{
			return  2166136261u;
		};
	};

	template<size_t N>
	unsigned int static_hash (const char (&s)[N])
	{
		return static_hash_calc<N>::apply(s);
	}

	typedef Polaris_Component<NT,NULLTYPELIST,Execution_Object> NC;
	
	///----------------------------------------------------------------------------------------------------
	/// Dynamic_Dispatch - run time dispatch to function matching alias, signature, and component_id
	///----------------------------------------------------------------------------------------------------

	// USAGE NOTE: Dynamic_Dispatch(ALIAS,FUNCTION_SIGNATURE)(arg1,arg2,...)

	#ifdef SAFE_MODE
		#define Dynamic_Dispatch(ALIAS,FUNCTION_SIGNATURE)\
			if(!((Component_Manager_Base*)(__all_components[Identify()]))->function_map.count(static_hash(#ALIAS))) cout << "No Matching Function for Alias: " << #ALIAS << endl;\
			else if(FUNCTION_SIGNATURE::function_id != ((Component_Manager_Base*)(__all_components[Identify()]))->function_map[static_hash(#ALIAS)].first) cout << "No Matching Function for Signature: " << #FUNCTION_SIGNATURE << endl;\
			else (((NULLTYPE*)this)->*( (FUNCTION_SIGNATURE::function_type) ( ((Component_Manager_Base*)(__all_components[Identify()]))->function_map[ static_hash(#ALIAS) ].second )  ))
	#else
		#define Dynamic_Dispatch(ALIAS,FUNCTION_SIGNATURE)\
			(((NULLTYPE*)this)->*( (FUNCTION_SIGNATURE::function_type) ( ((Component_Manager_Base*)(__all_components[Identify()]))->function_map[ static_hash(#ALIAS) ].second )  ))
	#endif
	
	///----------------------------------------------------------------------------------------------------
	/// Bind_Function - static operation which binds a function to an alias, signature, and component type
	///----------------------------------------------------------------------------------------------------

	#ifdef SAFE_MODE
		#define Bind_Function(ALIAS,FUNCTION_SIGNATURE,COMPONENT,FUNCTION_NAME)\
			if(!(__all_components.count(COMPONENT::component_id))) cout << "No Matching Component to Bind: " << #FUNCTION_NAME << endl;\
			else ((Component_Manager_Base*)__all_components[COMPONENT::component_id])->function_map[ static_hash(#ALIAS) ] = pair<int,void(NULLTYPE::*)(void)>( FUNCTION_SIGNATURE::function_id,(Member_Function_Signature<>::function_type)&COMPONENT::FUNCTION_NAME)
	#else
		#define Bind_Function(ALIAS,FUNCTION_SIGNATURE,COMPONENT,FUNCTION_NAME)\
			((Component_Manager_Base*)__all_components[COMPONENT::component_id])->function_map[ static_hash(#ALIAS) ] = pair<int,void(NULLTYPE::*)(void)>( FUNCTION_SIGNATURE::function_id,(Member_Function_Signature<>::function_type)&COMPONENT::FUNCTION_NAME)
	#endif

}