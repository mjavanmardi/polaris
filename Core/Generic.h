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

bool operator==(const Type_Info& A,const Type_Info& B){return (*A.pInfo_)==(*B.pInfo_);}

///============================================================================
/// Basic Type Traits
///============================================================================

template<const bool T> struct test_condition{typedef false_type type;};
template<> struct test_condition<true>{typedef true_type type;};

///============================================================================
/// TypeList Definition
///============================================================================

template<class T,class U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

#define TYPELIST_1(T1) Typelist<T1,NULLTYPE>

///============================================================================
/// IndexOf Implementation
///============================================================================

template<class TList, class T> struct IndexOf;

template<class T> struct IndexOf<NULLTYPE,T>{enum{value = -1 };};

template<class T, class Tail> struct IndexOf<Typelist<T, Tail>,T>{enum{value = 0 };};

template<class Head, class Tail, class T> struct IndexOf<Typelist<Head, Tail>, T>{private:enum { temp = IndexOf<Tail, T>::value };public:enum { value = temp == -1 ? -1 : 1 + temp };};

///============================================================================
/// Length Implementation
///============================================================================

template<class TList> struct Length;template <> struct Length<NULLTYPE>{enum { value = 0 };};

template<class T, class U> struct Length<Typelist<T, U>>{enum { value = 1 + Length<U>::value };};

///============================================================================
/// Append Implementation
///============================================================================

template<class TList, class T> struct Append;

template<> struct Append<NULLTYPE, NULLTYPE>{typedef NULLTYPE Result;};

template<class T> struct Append<NULLTYPE, T>{typedef TYPELIST_1(T) Result;};

template<class Head, class Tail> struct Append<NULLTYPE, Typelist<Head, Tail> >{typedef Typelist<Head, Tail> Result;};

template<class Head, class Tail, class T> struct Append<Typelist<Head, Tail>, T>{typedef Typelist<Head,typename Append<Tail, T>::Result> Result;};

///============================================================================
/// IsTrue Implementation
///============================================================================

template<class TList,unsigned int index=Length<TList>::value-1> struct IsTrue;

template<class Head,class Tail>
struct IsTrue<Typelist<Head,Tail>,0>{static const bool value=Head::value;};

template<class Head,class Tail,unsigned int i>
struct IsTrue<Typelist<Head,Tail>,i>{static const bool value=Head::value && IsTrue<Tail,i-1>::value;};
