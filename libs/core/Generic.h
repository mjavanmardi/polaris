#pragma once
///----------------------------------------------------------------------------------------------------
/// Generic.h - General purpose generic programming structures
///----------------------------------------------------------------------------------------------------

#include "Dependencies.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// General Definitions
	///----------------------------------------------------------------------------------------------------

	#define concat(...) __VA_ARGS__

	///----------------------------------------------------------------------------------------------------
	/// Null Type Definitions
	///----------------------------------------------------------------------------------------------------

	struct NULLTYPE{};

	typedef NULLTYPE NT;

	struct NULLCOMPONENT{typedef NULLTYPE Component_Type;};

	template<typename A> struct NULLTEMPLATE{};
	template<typename A,typename B> struct NULLTEMPLATE_2{};
	template<typename A,typename B,typename C> struct NULLTEMPLATE_3{};
	template<typename A,typename B,typename C,typename D> struct NULLTEMPLATE_4{};
	template<typename A,typename B,typename C,typename D,typename E> struct NULLTEMPLATE_5{};
	template<typename A,typename B,typename C,typename D,typename E,typename F> struct NULLTEMPLATE_6{};

	///----------------------------------------------------------------------------------------------------
	/// TypeList Definition
	///----------------------------------------------------------------------------------------------------

	template<class T,class U=NULLTYPE>
	struct TypeList
	{
		typedef T Head;
		typedef U Tail;
	};

	#define TYPELIST_1(T1) TypeList<T1,NULLTYPE>
	#define TYPELIST_2(T1,T2) TypeList<T1,TYPELIST_1(T2)>
	#define TYPELIST_3(T1,T2,T3) TypeList<T1,TYPELIST_2(T2,T3)>
	#define TYPELIST_4(T1,T2,T3,T4) TypeList<T1,TYPELIST_3(T2,T3,T4)>
	#define TYPELIST_5(T1,T2,T3,T4,T5) TypeList<T1,TYPELIST_4(T2,T3,T4,T5)>

	#define NULLTYPELIST TypeList<NULLTYPE,NULLTYPE>
	#define NTL TypeList<NULLTYPE,NULLTYPE>
	#define INHERIT(IMPLEMENTATION) typename Append<InheritanceList,IMPLEMENTATION<MasterType>>::Result
	#define PROTO_INHERIT(IMPLEMENTATION) typename Append<InheritanceList,IMPLEMENTATION>::Result

	///----------------------------------------------------------------------------------------------------
	/// IndexOf Implementation
	///----------------------------------------------------------------------------------------------------

	template<class TList, class T> struct IndexOf;

	template<class T> struct IndexOf<NULLTYPE,T>{enum{value = -1 };};

	template<class T, class Tail> struct IndexOf<TypeList<T, Tail>,T>{enum{value = 0 };};

	template<class Head, class Tail, class T> struct IndexOf<TypeList<Head, Tail>, T>{private:enum { temp = IndexOf<Tail, T>::value };public:enum { value = temp == -1 ? -1 : 1 + temp };};

	///----------------------------------------------------------------------------------------------------
	/// ValidIndex Implementation
	///----------------------------------------------------------------------------------------------------

	template<int Index>
	struct ValidIndex
	{
		typedef true_type type;
	};

	template<>
	struct ValidIndex<-1>
	{
		typedef false_type type;
	};

	///----------------------------------------------------------------------------------------------------
	/// TypeAt Implementation
	///----------------------------------------------------------------------------------------------------

	template<class TList, unsigned int index> struct TypeAt;

	template<class Head, class Tail>
	struct TypeAt<TypeList<Head, Tail>,0>
	{
		typedef Head Result;
	};

	template<class Head, class Tail, unsigned int i>
	struct TypeAt<TypeList<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail,i-1>::Result Result;
	};

	template<>
	struct TypeAt<NULLTYPE,0>
	{
		typedef NULLCOMPONENT Result;
	};

	///----------------------------------------------------------------------------------------------------
	/// Length Implementation
	///----------------------------------------------------------------------------------------------------

	template<class TList> struct _Length;template <> struct _Length<NULLTYPE>{enum { value = 0 };};

	template<class T, class U> struct _Length<TypeList<T, U>>{enum { value = 1 + _Length<U>::value };};

	///----------------------------------------------------------------------------------------------------
	/// Append Implementation
	///----------------------------------------------------------------------------------------------------

	template<class TList, class T> struct Append;

	template<> struct Append<NULLTYPE, NULLTYPE>{typedef NULLTYPE Result;};

	template<class T> struct Append<NULLTYPE, T>{typedef TypeList<T,NULLTYPE> Result;};

	template<class Head, class Tail> struct Append<NULLTYPE, TypeList<Head, Tail> >{typedef TypeList<Head, Tail> Result;};

	template<class Head, class Tail, class T> struct Append<TypeList<Head, Tail>, T>{typedef TypeList<Head,typename Append<Tail, T>::Result> Result;};

	///----------------------------------------------------------------------------------------------------
	/// Erase Implementation
	///----------------------------------------------------------------------------------------------------

	template <class TList, class T> struct Erase;

	template <class T>
	struct Erase<NULLTYPE, T>
	{
		typedef NULLTYPE Result;
	};

	template <class T, class Tail>
	struct Erase<TypeList<T, Tail>, T>
	{
		typedef Tail Result;
	};

	template <class Head, class Tail, class T>
	struct Erase<TypeList<Head, Tail>, T>
	{
		typedef TypeList<Head,typename Erase<Tail, T>::Result> Result;
	};

	///----------------------------------------------------------------------------------------------------
	/// Remove Duplicates Implementation
	///----------------------------------------------------------------------------------------------------

	template <class TList> struct RemoveDuplicates;

	template <> struct RemoveDuplicates<NULLTYPE>
	{
		typedef NULLTYPE Result;
	};

	template <class Head, class Tail>
	struct RemoveDuplicates< TypeList<Head, Tail> >
	{
	private:
		typedef typename RemoveDuplicates<Tail>::Result L1;
		typedef typename Erase<L1, Head>::Result L2;
	public:
		typedef TypeList<Head, L2> Result;
	};

	///----------------------------------------------------------------------------------------------------
	/// IsTrue Implementation
	///----------------------------------------------------------------------------------------------------

	template<class TList,unsigned int index=_Length<TList>::value-1> struct IsTrue;

	template<class Head,class Tail>
	struct IsTrue<TypeList<Head,Tail>,0>{ static const bool value=Head::value; };

	template<class Head,class Tail,unsigned int i>
	struct IsTrue<TypeList<Head,Tail>,i>{ static const bool value=Head::value && IsTrue<Tail,i-1>::value; };

//	template<typename T>
//	struct ToStringTester
//	{
//		template<typename U>
//		constexpr static bool ExistsInForm(
//				typename std::enable_if<
//						std::is_member_function_pointer<
//								decltype(static_cast<std::string(U::*)()const>(&U::toString))>::value>::type* = nullptr)
//		{}
//
//		template<typename U>
//		constexpr static bool ExistsInForm(...){ return Exists<U>(0); }
//
//		template<typename U>
//		constexpr static bool Exists(typename std::enable_if<std::is_member_function_pointer<decltype(&U::toString) >::value>::type* = nullptr)
//		{}
//
//		template<typename U>
//		constexpr static bool Exists(...)
//		{}
//
//		static const bool value = (ExistsInForm<T>(0) == 1);
//	};

	///----------------------------------------------------------------------------------------------------
	/// dereference Implementation
	///----------------------------------------------------------------------------------------------------
	template<typename T>
	T& dereference(T* val){return *val;}
	template<typename T>
	T& dereference(T& val){return val;}

	////-----------------------------------------------------------------
	//// Feature Dispatcher
	////
	//// Pass control to a specified feature of a type from a list of types from an anonymous pointer to a feature
	//#define define_feature_dispatcher(FEATURE_NAME, DISPATCH_ALIAS)\
	//template <class TList> struct DISPATCH_ALIAS;\
	//template <>\
	//struct DISPATCH_ALIAS<NULLTYPE>\
	//{\
	//	template<class HeadComponentType, typename TargetType>\
	//	static inline typename TargetType::ReturnType Start_Dispatch(void* obj)\
	//	{\
	//		THROW_EXCEPTION("ERROR: type id: " << ((generic_implementation<NT>*)obj)->Identify() << " not found in typelist");\
	//	}\
	//};\
	//template <class Head, class Tail>\
	//struct DISPATCH_ALIAS<TypeList<Head, Tail> >\
	//{\
	//	define_feature_exists_check(FEATURE_NAME, FEATURE_NAME##_exists)\
	//	template<class HeadType_As_Given, typename TargetType>\
	//	static inline typename TargetType::ReturnType Start_Dispatch(void* obj, requires(HeadType_As_Given,check(HeadType_As_Given, Is_Polaris_Prototype) || check(HeadType_As_Given, Is_Polaris_Component)))\
	//	{\
	//		return DISPATCH_ALIAS<TypeList<HeadType_As_Given, Tail> >::template Dispatch<typename HeadType_As_Given::Component_Type,HeadType_As_Given, TargetType>(obj);\
	//	}\
	//	template<class HeadType_As_Given, typename TargetType>\
	//	static inline typename TargetType::ReturnType Start_Dispatch(void* obj, requires(HeadType_As_Given,!check(HeadType_As_Given, Is_Polaris_Prototype) && !check(HeadType_As_Given, Is_Polaris_Component)))\
	//	{\
	//		assert_check(HeadType_As_Given, Is_Polaris_Prototype, "Type is not a valid polaris prototype or component." );\
	//	}\
	//	template<class HeadComponentType, class HeadType_As_Given, class TargetType>\
	//	static inline typename TargetType::ReturnType Dispatch(void* obj, requires(HeadType_As_Given,check(HeadType_As_Given, Is_Polaris_Prototype) && check(HeadComponentType,FEATURE_NAME##_exists)))\
	//	{\
	//		if(((generic_implementation<NT>*)obj)->Identify() == Head::Component_Type::component_index)\
	//		{\
	//			return (typename TargetType::ReturnType)((Head*)obj)->template FEATURE_NAME<typename TargetType::ControlType>();\
	//		}\
	//		else\
	//		{\
	//			return DISPATCH_ALIAS<Tail>::template Start_Dispatch<typename TypeAt<Tail,0>::Result, TargetType>(obj);\
	//		}\
	//	}\
	//	template<class HeadComponentType, class HeadType_As_Given, class TargetType>\
	//	static inline typename TargetType::ReturnType Dispatch(void* obj, requires(HeadType_As_Given,check(HeadType_As_Given, Is_Polaris_Component) && check(HeadComponentType,FEATURE_NAME##_exists)))\
	//	{\
	//		if(((generic_implementation<NT>*)obj)->Identify() == Head::Component_Type::component_index)\
	//		{\
	//			return (typename TargetType::ReturnType)((Head*)obj)->template FEATURE_NAME<HeadType_As_Given, HeadType_As_Given, typename TargetType::ControlType>();\
	//		}\
	//		else\
	//		{\
	//			return DISPATCH_ALIAS<Tail>::template Start_Dispatch<typename TypeAt<Tail,0>::Result, TargetType>(obj);\
	//		}\
	//	}\
	//	template<class HeadComponentType, class HeadType_As_Given, class TargetType>\
	//	static inline typename TargetType::ReturnType  Dispatch(void* obj, requires( (!check(HeadType_As_Given, Is_Polaris_Component) && !check(HeadType_As_Given, Is_Polaris_Prototype))))\
	//	{\
	//		assert_check(HeadType_As_Given, Is_Polaris_Prototype, "Type is not a valid polaris prototype or component." );\
	//	}\
	//	template<class HeadComponentType, class HeadType_As_Given, class TargetType>\
	//	static inline typename TargetType::ReturnType  Dispatch(void* obj, requires( (check(HeadType_As_Given, Is_Polaris_Component) || check(HeadType_As_Given, Is_Polaris_Prototype)) && !check(HeadComponentType,FEATURE_NAME##_exists)))\
	//	{\
	//		assert_check(HeadComponentType, FEATURE_NAME##_exists, "" #FEATURE_NAME "does not exist.  Make sure to 'tag_feature_as_available' if the feature does exist in the component." );\
	//	}\
	//};
	//#define dispatch_to_feature(DISPATCHER_ALIAS, TYPELIST, OBJECT, TARGETTYPE_STRUCT_WITH_CONTROLTYPE_FOR_DISPATCH_TARGET_AND_RETURNTYPE_DEFINED, ...)\
	//	DISPATCHER_ALIAS<TYPELIST>::template Start_Dispatch<typename TypeAt<TYPELIST,0>::Result, concat(TARGETTYPE_STRUCT_WITH_CONTROLTYPE_FOR_DISPATCH_TARGET_AND_RETURNTYPE_DEFINED,__VA_ARGS__)>(OBJECT);





	#define define_static_typelist_loop(FEATURE_NAME, DISPATCH_ALIAS)\
		template <class TList> struct DISPATCH_ALIAS;\
		template <>\
		struct DISPATCH_ALIAS<NULLTYPE>\
		{\
			template<class HeadComponentType, typename TargetType>\
			static inline void Start_Dispatch(void* obj)\
			{\
			}\
		};\
		template <class Head, class Tail>\
		struct DISPATCH_ALIAS<TypeList<Head, Tail> >\
		{\
			template<class HeadType_As_Given, typename TargetType>\
			static inline void Start_Dispatch(void* obj)\
			{\
				DISPATCH_ALIAS<TypeList<HeadType_As_Given, Tail> >::template Dispatch<HeadType_As_Given::Component_Type,HeadType_As_Given, TargetType>(obj);\
			}\
			template<class HeadComponentType, class HeadType_As_Given, class TargetType>\
			static inline void Dispatch(void* obj)\
			{\
				Head::template FEATURE_NAME<HeadType_As_Given, HeadType_As_Given, NT>(obj);\
				DISPATCH_ALIAS<Tail>::template Start_Dispatch<TypeAt<Tail,0>::Result, TargetType>(obj);\
			}\
		};

	#define execute_static_typelist_loop(DISPATCHER_ALIAS, TYPELIST, OBJECT_PTR)\
			DISPATCHER_ALIAS<TYPELIST>::template Start_Dispatch<TypeAt<TYPELIST,0>::Result, NT>(OBJECT_PTR);






	//#define feature_method_void(FEATURE_NAME, REQUIREMENTS)\
	//	public:\
	//		define_feature_exists_check(FEATURE_NAME, FEATURE_NAME##_exists);\
	//		template<typename ReturnValueType>\
	//		ReturnValueType FEATURE_NAME(requires_getter(check(ComponentType,FEATURE_NAME##_exists) && (REQUIREMENTS)))\
	//		{\
	//			return (ReturnValueType)this_component()->template FEATURE_NAME<ComponentType,CallerType,ReturnValueType>();\
	//		}\
	//		template<typename ReturnValueType>\
	//		ReturnValueType FEATURE_NAME(requires_getter(!check(ComponentType,FEATURE_NAME##_exists) || !(REQUIREMENTS)))\
	//		{\
	//			static_assert(FEATURE_NAME##_exists<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a feature_implementation for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_feature_as_available\"? ---------]\n\n");\
	//			static_assert(REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#REQUIREMENTS" } ---------]\n\n");\
	//		}

	//#define feature_method_1_arg(FEATURE_NAME, ARG, REQUIREMENTS)\
	//	public:\
	//		define_feature_exists_check(FEATURE_NAME##_1args, FEATURE_NAME##_1args_exists);\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG,requires( check(TargetType,Is_Target_Type_Struct) && check(ComponentType,FEATURE_NAME##_1args_exists) && (REQUIREMENTS) ) )\
	//		{\
	//			return (typename TargetType::ReturnType)this_component()->template FEATURE_NAME<ComponentType,CallerType,TargetType>(ARG);\
	//		}\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG,requires(!check(TargetType,Is_Target_Type_Struct)  || !check(ComponentType,FEATURE_NAME##_1args_exists) || !(REQUIREMENTS)))\
	//		{\
	//			static_assert(FEATURE_NAME##_1args_exists<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a feature_implementation for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_feature_as_available\"? ---------]\n\n");\
	//			assert_check(TargetType, Is_Target_Type_Struct, "Must use a Target_Type struct for the target type when using the feature_method macro_1.");\
	//			assert_sub_check(TargetType, Is_Target_Type_Struct, has_ReturnType, "Check1 fail");\
	//			assert_sub_check(TargetType, Is_Target_Type_Struct, has_ParamType, "Check2 fail");\
	//			assert_sub_check(TargetType, Is_Target_Type_Struct, has_Param2Type, "Check3 fail");\
	//			static_assert(REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#REQUIREMENTS" } ---------]\n\n");\
	//		}
	//	
	//#define feature_method_2_arg(FEATURE_NAME, ARG1, ARG2, REQUIREMENTS)\
	//	public:\
	//		define_feature_exists_check(FEATURE_NAME##_2args, FEATURE_NAME##_2args_exists);\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2,requires(check(ComponentType,FEATURE_NAME##_2args_exists) && check(TargetType,Is_Target_Type_Struct) && (REQUIREMENTS) ) )\
	//		{\
	//			return (typename TargetType::ReturnType)this_component()->template FEATURE_NAME<ComponentType,CallerType,TargetType>(ARG1,ARG2);\
	//		}\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2,requires(!check(ComponentType,FEATURE_NAME##_2args_exists) || !check(TargetType,Is_Target_Type_Struct)  || !(REQUIREMENTS)))\
	//		{\
	//			static_assert(FEATURE_NAME##_2args_exists<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a feature_implementation for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_feature_signature_as_available\"? ---------]\n\n");\
	//			static_assert(Is_Target_Type_Struct<TargetType>::value,"\n\n\n[--------- Must use a Target_Type struct for the target type when using the feature_method macro_1.---------]\n\n");\
	//			static_assert(REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#REQUIREMENTS" } ---------]\n\n");\
	//		}

	//#define feature_method_3_arg(FEATURE_NAME, ARG1, ARG2, ARG3, REQUIREMENTS)\
	//	public:\
	//		define_feature_exists_check(FEATURE_NAME##_3args, FEATURE_NAME##_3args_exists);\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2, typename TargetType::Param3Type ARG3,requires(check(ComponentType,FEATURE_NAME##_3args_exists) && check(TargetType,Is_Target_Type_Struct) && (REQUIREMENTS) ) )\
	//		{\
	//			return (typename TargetType::ReturnType)this_component()->template FEATURE_NAME<ComponentType,CallerType,TargetType>(ARG1,ARG2,ARG3);\
	//		}\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2, typename TargetType::Param3Type ARG3,requires(!check(ComponentType,FEATURE_NAME##_3args_exists) || !check(TargetType,Is_Target_Type_Struct)  || !(REQUIREMENTS)))\
	//		{\
	//			static_assert(FEATURE_NAME##_3args_exists<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a feature_implementation for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_feature_signature_as_available\"? ---------]\n\n");\
	//			static_assert(Is_Target_Type_Struct<TargetType>::value,"\n\n\n[--------- Must use a Target_Type struct for the target type when using the feature_method macro_1.---------]\n\n");\
	//			static_assert(REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#REQUIREMENTS" } ---------]\n\n");\
	//		}

	//#define feature_method_4_arg(FEATURE_NAME, ARG1, ARG2, ARG3, ARG4, REQUIREMENTS)\
	//	public:\
	//		define_feature_exists_check(FEATURE_NAME##_4args, FEATURE_NAME##_4args_exists);\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2,typename TargetType::Param3Type ARG3, typename TargetType::Param4Type ARG4,requires(check(ComponentType,FEATURE_NAME##_4args_exists) && check(TargetType,Is_Target_Type_Struct) && (REQUIREMENTS) ) )\
	//		{\
	//			return (typename TargetType::ReturnType)this_component()->template FEATURE_NAME<ComponentType,CallerType,TargetType>(ARG1,ARG2,ARG3,ARG4);\
	//		}\
	//		template<typename TargetType>\
	//		typename TargetType::ReturnType FEATURE_NAME(typename TargetType::ParamType ARG1, typename TargetType::Param2Type ARG2,typename TargetType::Param3Type ARG3, typename TargetType::Param4Type ARG4,requires(!check(ComponentType,FEATURE_NAME##_4args_exists) || !check(TargetType,Is_Target_Type_Struct)  || !(REQUIREMENTS)))\
	//		{\
	//			static_assert(FEATURE_NAME##_4args_exists<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a feature_implementation for " #FEATURE_NAME " exists, did you remember to use the macro \"tag_feature_as_available\"? ---------]\n\n");\
	//			static_assert(Is_Target_Type_Struct<TargetType>::value,"\n\n\n[--------- Must use a Target_Type struct for the target type when using the feature_method macro_1.---------]\n\n");\
	//			static_assert(REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #FEATURE_NAME"\" could not be satisfied: { "#REQUIREMENTS" } ---------]\n\n");\
	//		}
	//#define tag_feature_signature_as_available(FEATURE_NAME,NUM_ARGS) typedef true_type FEATURE_NAME##_##NUM_ARGS##args_feature_tag;

}