#pragma once
///----------------------------------------------------------------------------------------------------
/// Local_Concepts.h - Local concept checks
///----------------------------------------------------------------------------------------------------

#include "Concepts.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Concept Checks
	///----------------------------------------------------------------------------------------------------


	///----------------------------------------------------------------------------------------------------
	/// general_check - user-defined check
	///----------------------------------------------------------------------------------------------------

	#define local_general_check(CHECK_ALIAS,...)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>{ static const bool value = __VA_ARGS__; };\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_typedef_name - typedef of given name must exist
	///----------------------------------------------------------------------------------------------------

	#define local_check_typedef_name(CHECK_ALIAS,NAME)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename V::##NAME*);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(0))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_typedef_type - typedef of given name must exist and match given type
	///----------------------------------------------------------------------------------------------------

	#define local_check_typedef_type(CHECK_ALIAS,NAME,TYPE)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename V::##NAME*);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(0))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<typename V::##NAME,TYPE>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_name - given name must be a data member
	///----------------------------------------------------------------------------------------------------

	#define local_check_data_member_name(CHECK_ALIAS,NAME)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_type - given name must be a data member and match given type
	///----------------------------------------------------------------------------------------------------

	#define local_check_data_member_type(CHECK_ALIAS,NAME,TYPE)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<decltype(&V::NAME),TYPE V::*>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_data_member_meets_check - given name must be a data member and meet user defined compile time check
	///----------------------------------------------------------------------------------------------------

	#define local_check_data_member_meets_check(CHECK_ALIAS,NAME,...)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_object_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = __VA_ARGS__; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = data_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_method_name - given name must be a member method
	///----------------------------------------------------------------------------------------------------

	#define local_check_method_name(CHECK_ALIAS,NAME)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = true; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_method_type - given name must be a method member and match given signature
	///----------------------------------------------------------------------------------------------------

	#define local_check_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(typename is_member_function_pointer<decltype(&V::NAME)>::type);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(true_val))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const bool value = is_same<decltype(&V::NAME),FUNCTION_SIGNATURE::function_type>::value; };\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_accessor_name - given name must be a method template member
	///----------------------------------------------------------------------------------------------------

	#define local_check_template_method_name(CHECK_ALIAS,NAME)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::template NAME<NULLTYPE>);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool _P>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V>\
			struct form_check<V,true>{ static const int value = true;};\
			\
			static const bool performcheck = (sizeof(has_matching_named_member<U>(nullptr))==success);\
			static const bool value = form_check<U,performcheck>::value;\
		};\
		\
		static const bool value = function_check<T>::value;\
	};

	///----------------------------------------------------------------------------------------------------
	/// check_template_method_type - given name must be a method template member and match signature
	///----------------------------------------------------------------------------------------------------

	#define local_check_template_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE,...)\
	template<typename T>\
	struct CHECK_ALIAS\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct function_check{ static const bool value = true; };\
		template<typename U>\
		struct function_check<U,true>\
		{\
			template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<__VA_ARGS__>);\
			template<typename V> static large_type has_matching_named_member(...);\
			\
			template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
			struct form_check{ static const bool value = false; };\
			\
			template<typename V> static small_type has_matching_formed_member(typename FUNCTION_SIGNATURE::function_type);\
			template<typename V> static large_type has_matching_formed_member(...);\
			\
			template<typename V>\
			struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<__VA_ARGS__>))==success);};\
			\
			static const bool value = form_check<U>::value;\
		};\
		\
		static const bool value = function_check<T>::value;\
	};


}