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

	struct NULLTYPE{typedef NULLTYPE Master_Type;};

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

}