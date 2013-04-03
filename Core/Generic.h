#pragma once
#include "Core_Includes.h"

///============================================================================
/// General Definitions
///============================================================================

typedef char small_type;

struct large_type{small_type dummy[2];};

template<typename Type,typename Tag>
struct member_function_ptr_types
{
	typedef void (Type::* type)(void);
};

/*
///============================================================================
/// Type_Info Wrapper Class
///============================================================================

class Type_Info
{
public:
	Type_Info(){};
	Type_Info(const type_info& val){pInfo_=&val;}
	//Type_Info(const Type_Info& val){pInfo_=val.pInfo_;}
	//Type_Info& operator=(const Type_Info& val){*this=val; return *this;}
	//bool before(const Type_Info&) const;
	const char* name() const{return pInfo_->name();}
//private:
	const type_info* pInfo_;
};

static bool operator==(const Type_Info& A,const Type_Info& B){return (*A.pInfo_)==(*B.pInfo_);}
*/


struct NULLCOMPONENT{typedef NULLTYPE Component_Type;};


///============================================================================
/// Basic Type Traits
///============================================================================

template<const bool T> struct test_condition{typedef false_type type;};
template<> struct test_condition<true>{typedef true_type type;};

///============================================================================
/// TypeList Definition
///============================================================================

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

#define NULLTYPELIST TypeList<NULLTYPE,NULLTYPE>
#define APPEND_CHILD(IMPLEMENTATION) typename Append<InheritanceList,IMPLEMENTATION<MasterType,ParentType>>::Result

///============================================================================
/// IndexOf Implementation
///============================================================================

template<class TList, class T> struct IndexOf;

template<class T> struct IndexOf<NULLTYPE,T>{enum{value = -1 };};

template<class T, class Tail> struct IndexOf<TypeList<T, Tail>,T>{enum{value = 0 };};

template<class Head, class Tail, class T> struct IndexOf<TypeList<Head, Tail>, T>{private:enum { temp = IndexOf<Tail, T>::value };public:enum { value = temp == -1 ? -1 : 1 + temp };};

///============================================================================
/// ValidIndex Implementation
///============================================================================

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

///============================================================================
/// TypeAt Implementation
///============================================================================

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

///============================================================================
/// Length Implementation
///============================================================================

template<class TList> struct _Length;template <> struct _Length<NULLTYPE>{enum { value = 0 };};

template<class T, class U> struct _Length<TypeList<T, U>>{enum { value = 1 + _Length<U>::value };};

///============================================================================
/// Append Implementation
///============================================================================

template<class TList, class T> struct Append;

template<> struct Append<NULLTYPE, NULLTYPE>{typedef NULLTYPE Result;};

template<class T> struct Append<NULLTYPE, T>{typedef TypeList<T,NULLTYPE> Result;};

template<class Head, class Tail> struct Append<NULLTYPE, TypeList<Head, Tail> >{typedef TypeList<Head, Tail> Result;};

template<class Head, class Tail, class T> struct Append<TypeList<Head, Tail>, T>{typedef TypeList<Head,typename Append<Tail, T>::Result> Result;};

///============================================================================
/// Erase Implementation
///============================================================================

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

///============================================================================
/// Remove Duplicates Implementation
///============================================================================

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

///============================================================================
/// IsTrue Implementation
///============================================================================

template<class TList,unsigned int index=_Length<TList>::value-1> struct IsTrue;

template<class Head,class Tail>
struct IsTrue<TypeList<Head,Tail>,0>{static const bool value=Head::value;};

template<class Head,class Tail,unsigned int i>
struct IsTrue<TypeList<Head,Tail>,i>{static const bool value=Head::value && IsTrue<Tail,i-1>::value;};

