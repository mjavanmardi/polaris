#pragma once
///----------------------------------------------------------------------------------------------------
/// Function_Signature.h - Templated Representation of Function Signatures
///----------------------------------------------------------------------------------------------------

#include "Generic.h"

namespace polaris
{

	///----------------------------------------------------------------------------------------------------
	/// function_counter - tracks number of unique function signature types
	///----------------------------------------------------------------------------------------------------

	extern int __function_counter;

	struct NULLPARAMTYPE{};

	typedef NULLPARAMTYPE NPT;

	///----------------------------------------------------------------------------------------------------
	/// Member_Function_Signature - Templated Representation of Member Function Signatures up to 4 args
	///----------------------------------------------------------------------------------------------------

	template<typename ClassType=NPT,typename ReturnType=NPT,typename Arg1Type=NPT,typename Arg2Type=NPT,typename Arg3Type=NPT,typename Arg4Type=NPT>
	struct Member_Function_Signature
	{
		typedef ReturnType(ClassType::* function_type)(Arg1Type,Arg2Type,Arg3Type,Arg4Type);
		static const int function_id;
	};

	template<typename ClassType,typename ReturnType,typename Arg1Type,typename Arg2Type,typename Arg3Type,typename Arg4Type>
	const int Member_Function_Signature<ClassType,ReturnType,Arg1Type,Arg2Type,Arg3Type,Arg4Type>::function_id = ++__function_counter;

	template<typename ClassType,typename ReturnType,typename Arg1Type,typename Arg2Type,typename Arg3Type>
	struct Member_Function_Signature<ClassType,ReturnType,Arg1Type,Arg2Type,Arg3Type,NPT>
	{
		typedef ReturnType(ClassType::* function_type)(Arg1Type,Arg2Type,Arg3Type);
		static const int function_id;
	};

	template<typename ClassType,typename ReturnType,typename Arg1Type,typename Arg2Type,typename Arg3Type>
	const int Member_Function_Signature<ClassType,ReturnType,Arg1Type,Arg2Type,Arg3Type,NPT>::function_id = ++__function_counter;

	template<typename ClassType,typename ReturnType,typename Arg1Type,typename Arg2Type>
	struct Member_Function_Signature<ClassType,ReturnType,Arg1Type,Arg2Type,NPT,NPT>
	{
		typedef ReturnType(ClassType::* function_type)(Arg1Type,Arg2Type);
		static const int function_id;
	};

	template<typename ClassType,typename ReturnType,typename Arg1Type,typename Arg2Type>
	const int Member_Function_Signature<ClassType,ReturnType,Arg1Type,Arg2Type,NPT,NPT>::function_id = ++__function_counter;

	template<typename ClassType,typename ReturnType,typename Arg1Type>
	struct Member_Function_Signature<ClassType,ReturnType,Arg1Type,NPT,NPT,NPT>
	{
		typedef ReturnType(ClassType::* function_type)(Arg1Type);
		static const int function_id;
	};

	template<typename ClassType,typename ReturnType,typename Arg1Type>
	const int Member_Function_Signature<ClassType,ReturnType,Arg1Type,NPT,NPT,NPT>::function_id = ++__function_counter;

	template<typename ClassType,typename ReturnType>
	struct Member_Function_Signature<ClassType,ReturnType,NPT,NPT,NPT,NPT>
	{
		typedef ReturnType(ClassType::* function_type)(void);
		static const int function_id;
	};

	template<typename ClassType,typename ReturnType>
	const int Member_Function_Signature<ClassType,ReturnType,NPT,NPT,NPT,NPT>::function_id = ++__function_counter;

	template<typename ClassType>
	struct Member_Function_Signature<ClassType,NPT,NPT,NPT,NPT,NPT>
	{
		typedef void(ClassType::* function_type)(void);
		static const int function_id;
	};

	template<typename ClassType>
	const int Member_Function_Signature<ClassType,NPT,NPT,NPT,NPT,NPT>::function_id = ++__function_counter;

	template<>
	struct Member_Function_Signature<NPT,NPT,NPT,NPT,NPT,NPT>
	{
		typedef void(NULLTYPE::* function_type)(void);
		static const int function_id = 1;
	};

}