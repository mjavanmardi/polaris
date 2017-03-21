#ifndef FORMS_H
#define FORMS_H

///----------------------------------------------------------------------------------------------------
/// General Definitions
///----------------------------------------------------------------------------------------------------

#define concept template<typename T=NULLTYPE,typename V=NULLTYPE>

typedef char small_type;

static true_type true_val;

struct large_type{ small_type data[2]; };

static const int success = sizeof(small_type);

///----------------------------------------------------------------------------------------------------
/// Null Type Definitions
///----------------------------------------------------------------------------------------------------

struct NULLTYPE{};

typedef NULLTYPE NT;

static void* judge;


#define requires(T,...) typename enable_if<__VA_ARGS__ && True_Concept<T>::value>::type* = nullptr

#define accessor(NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
    public:\
        template<typename TypeChecked>\
        struct NAME##_set_check\
        {\
            template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
            struct function_check{ static const bool value = true; };\
            template<typename U>\
            struct function_check<U,true>\
            {\
                template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NT>);\
                template<typename V> static large_type has_matching_named_member(...);\
                \
                template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
                struct form_check{ static const bool value = false; };\
                template<typename V>\
                struct form_check<V,true>\
                {\
                    template<typename Z> static small_type has_matching_formed_member( decltype( ((Z*)judge)->state<NT>(NT()) ) value = nullptr );\
                    template<typename Z> static large_type has_matching_formed_member(...);\
                    static const int value = (sizeof(has_matching_formed_member<V>())==success);\
                };\
                \
                static const bool value = form_check<U>::value; \
            };\
            \
            static const bool value = function_check<TypeChecked>::value;\
        };\
        template<typename TargetType>\
        void NAME(TargetType set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
        {\
            this_component()->template NAME<TargetType>(set_value);\
        }\
        template<typename TargetType>\
        void NAME(TargetType set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
        {\
            static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
            static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
        }\
        static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
        \
        template<typename TypeChecked>\
        struct NAME##_get_check\
        {\
            template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
            struct function_check{ static const bool value = true; };\
            template<typename U>\
            struct function_check<U,true>\
            {\
                template<typename V> static small_type has_matching_named_member(void (V::* arg)() = &V::NAME<NT>);\
                template<typename V> static large_type has_matching_named_member(...);\
                \
                template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
                struct form_check{ static const bool value = false; };\
                template<typename V>\
                struct form_check<V,true>\
                {\
                    template<typename Z> static small_type has_matching_formed_member( decltype( ((Z*)judge)->state<NT>() ) value );\
                    template<typename Z> static large_type has_matching_formed_member(...);\
                    static const int value = (sizeof(has_matching_formed_member<V>(NT()))==success);\
                };\
                \
                static const bool value = form_check<U>::value;\
            };\
            \
            static const bool value = function_check<TypeChecked>::value;\
        };\
        template<typename TargetType>\
        TargetType NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
        {\
            return this_component()->template NAME<TargetType>();\
        }\
        template<typename TargetType>\
        TargetType NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
        {\
            static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
            static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
        }\
        static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\

#define m_data(DATA_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
                DATA_TYPE _##NAME;\
        public:\
                typedef DATA_TYPE NAME##_type;\
                typedef NAME##_type NAME##_accessible_type;\
                template<typename TargetType>\
                TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
                {return (TargetType)(_##NAME);}\
                template<typename TargetType>\
                TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
                {return (TargetType)(&_##NAME);}\
                template<typename TargetType>\
                TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
                {return (TargetType)(dereference(_##NAME));}\
                template<typename TargetType>\
                TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
                {return (TargetType)(_##NAME);}\
                template<typename TargetType>\
                TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
                {static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
                template<typename TargetType>\
                void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
                {_##NAME=(DATA_TYPE)(value);}\
                template<typename TargetType>\
                void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
                {_##NAME=(DATA_TYPE)(*value);}\
                template<typename TargetType>\
                void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
                {_##NAME=((DATA_TYPE)(&value));}\
                template<typename TargetType>\
                void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(DATA_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
                {_##NAME=(DATA_TYPE)(value);}\
                template<typename TargetType>\
                void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
                {static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\

#endif // FORMS_H
