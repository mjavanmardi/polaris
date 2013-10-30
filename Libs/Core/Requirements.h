#pragma once
///----------------------------------------------------------------------------------------------------
/// Requirements.h - Class Level Requirement Checking Library on "ComponentType"
///----------------------------------------------------------------------------------------------------

#include "Local_Concepts.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// requires_general_check - name must meet user-defined compile time check
	///----------------------------------------------------------------------------------------------------

	#define requires_general_check(CHECK_ALIAS,NAME,...)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_General_Check\
	{\
		template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
		struct data_check{ static const bool value = true; };\
		\
		template<typename U>\
		struct data_check<U,true>{ static const bool value = __VA_ARGS__; };\
		\
		static const bool value = data_check<T>::value;\
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_General_Check<ComponentType>::value,"\n\n\n[--------- Requirement Named:   \"" #NAME "\"  Did Not Meet the Check:   \"" #__VA_ARGS__ "\"! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_typedef_name - typedef of given name must exist
	///----------------------------------------------------------------------------------------------------

	#define requires_typedef_name(CHECK_ALIAS,NAME)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Type_Defined\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Type_Defined<ComponentType>::value,"\n\n\n[--------- Typename Requirement Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_typedef_type - typedef of given name must exist and match given type
	///----------------------------------------------------------------------------------------------------

	#define requires_typedef_type(CHECK_ALIAS,NAME,TYPE)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Type\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Type<ComponentType>::value,"\n\n\n[--------- Typename Requirement of Type:   " #TYPE "    Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_data_member_name - given name must be a data member
	///----------------------------------------------------------------------------------------------------

	#define requires_data_member_name(CHECK_ALIAS,NAME)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Named_Data\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Named_Data<ComponentType>::value,"\n\n\n[--------- Data Requirement Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_data_member_type - given name must be a data member and match given type
	///----------------------------------------------------------------------------------------------------

	#define requires_data_member_type(CHECK_ALIAS,NAME,TYPE)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Typed_Data\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Typed_Data<ComponentType>::value,"\n\n\n[--------- Data Requirement of Type:   " #TYPE "    Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_data_member_meets_check - given name must be a data member and meet user defined compile time check
	///----------------------------------------------------------------------------------------------------

	#define requires_data_member_meets_check(CHECK_ALIAS,NAME,...)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Data_Meets_Check\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Require_Data_Meets_Check<ComponentType>::value,"\n\n\n[--------- Data Requirement Named:   \"" #NAME "\"  Either Does Not Exist or Did Not Meet the Check:   \"" #__VA_ARGS__ "\"! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_method_name - given name must be a member method
	///----------------------------------------------------------------------------------------------------

	#define requires_method_name(CHECK_ALIAS,NAME)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Named_Method_Checker\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Named_Method_Checker<ComponentType>::value,"\n\n\n[--------- Method Requirement Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_method_type - given name must be a method member and match given signature
	///----------------------------------------------------------------------------------------------------

	#define requires_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE)\
	union\
	{\
	template<typename T>\
	struct CHECK_ALIAS##_Requires_Typed_Method_Checker\
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
	};\
	\
	static_assert(CHECK_ALIAS##_Requires_Typed_Method_Checker<ComponentType>::value,"\n\n\n[--------- Method Requirement of Type:   " #FUNCTION_SIGNATURE "    Named:   \"" #NAME "\"   Was Not Found! ---------]\n\n");\
	};

	///----------------------------------------------------------------------------------------------------
	/// requires_template_method_name - given name must be a method template member
	///----------------------------------------------------------------------------------------------------

	#define requires_template_method_name(CHECK_ALIAS,NAME)\
		union\
		{\
			template<typename T>\
			struct CHECK_ALIAS##_Requires_Typed_Method_Checker\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NULLTYPE>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = true;};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<T>::value;\
			};\
			\
			static_assert(CHECK_ALIAS##_Requires_Typed_Method_Checker<ComponentType>::value,"\n\n\n[--------- Template Method Requirement Named:   \"" #NAME "\" Was Not Found! ---------]\n\n");\
		};\

	///----------------------------------------------------------------------------------------------------
	/// requires_template_method_type - given name must be a method template member and match signature
	///----------------------------------------------------------------------------------------------------

	#define requires_template_method_type(CHECK_ALIAS,NAME,FUNCTION_SIGNATURE,...)\
		union\
		{\
			template<typename T>\
			struct CHECK_ALIAS##_Requires_Typed_Method_Checker\
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
			};\
			\
			static_assert(CHECK_ALIAS##_Requires_Typed_Method_Checker<ComponentType>::value,"\n\n\n[--------- Template Method Requirement of Type:   " #FUNCTION_SIGNATURE "    Named:   \"" #NAME "\"   With Arguments \"" #__VA_ARGS__ "\" Was Not Found! ---------]\n\n");\
		};\


}