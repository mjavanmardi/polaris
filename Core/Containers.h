#pragma once
#include "Grouping.h"

///============================================================================
/// Input_Iterator - stl Input_Iterator
///============================================================================

template<typename IteratorType,typename ComponentType,typename CallerType=NULLTYPE,typename TargetValueType=typename ComponentType::value_type>
struct Input_Iterator:public IteratorType
{
	Input_Iterator():IteratorType(){};

	Input_Iterator(IteratorType val):IteratorType(val){}

	TargetValueType& operator*(){return (TargetValueType&)(((IteratorType*)this)->operator*());}

	TargetValueType* operator->(){return (TargetValueType*)(((IteratorType*)this)->operator->());}

	Input_Iterator& operator++(){return (Input_Iterator&)++(*((IteratorType*)this));}

	void operator++(int){(*((IteratorType*)this))++;}
};

///============================================================================
/// Back_Insertion_Sequence_Prototype - stl Back Insertion Sequence prototype
///============================================================================

template<typename ComponentType,typename CallerType=NULLTYPE,typename TargetValueType=typename ComponentType::value_type>
struct Back_Insertion_Sequence_Prototype
{
	typedef Input_Iterator<typename ComponentType::iterator,ComponentType,CallerType,TargetValueType> iterator;
	typedef typename ComponentType::size_type size_type;

	iterator begin(){return (iterator)((ComponentType*)this)->begin();}

	iterator end(){return (iterator)((ComponentType*)this)->end();}
	
	size_type size(){return ((ComponentType*)this)->size();}

	size_type max_size(){return ((ComponentType*)this)->size();}

	bool empty(){return ((ComponentType*)this)->empty();}

	TargetValueType front(){return (TargetValueType)(((ComponentType*)this)->front());}

	iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
	
	void insert(iterator p, size_type n, TargetValueType t){return ((ComponentType*)this)->insert(p,n);}

	void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

	iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
	iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

	void clear(){return ((ComponentType*)this)->clear();}

	void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
	void resize(size_type n, TargetValueType t){return ((ComponentType*)this)->resize(n,t);}

	TargetValueType back(){return (TargetValueType)(((ComponentType*)this)->back());}
	
	void push_back(TargetValueType& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}
	
	void push_back(TargetValueType&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}
	
	void pop_back(){((ComponentType*)this)->pop_back();}

	void pop_front(){((ComponentType*)this)->pop_front();}
};

///============================================================================
/// Random_Access_Sequence_Prototype - stl Random Access Sequence prototype
///============================================================================

template<typename ComponentType,typename CallerType=NULLTYPE,typename TargetValueType=typename ComponentType::value_type>
struct Random_Access_Sequence_Prototype
{
	typedef Input_Iterator<typename ComponentType::iterator,ComponentType,CallerType,TargetValueType> iterator;
	typedef typename ComponentType::size_type size_type;

	iterator begin(){return (iterator)((ComponentType*)this)->begin();}

	iterator end(){return (iterator)((ComponentType*)this)->end();}
	
	size_type size(){return ((ComponentType*)this)->size();}

	size_type max_size(){return ((ComponentType*)this)->size();}

	bool empty(){return ((ComponentType*)this)->empty();}

	TargetValueType front(){return (TargetValueType)(((ComponentType*)this)->front());}

	iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
	
	void insert(iterator p, size_type n, TargetValueType t){return ((ComponentType*)this)->insert(p,n);}

	void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

	iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
	iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

	void clear(){return ((ComponentType*)this)->clear();}

	void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
	void resize(size_type n, TargetValueType t){return ((ComponentType*)this)->resize(n,t);}

	TargetValueType back(){return (TargetValueType)(((ComponentType*)this)->back());}
	
	void push_back(TargetValueType& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}

	void push_back(TargetValueType&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}

	void pop_back(){((ComponentType*)this)->pop_back();}

	void pop_front(){((ComponentType*)this)->pop_front();}

	TargetValueType& operator [](int i){return (TargetValueType&)((*((ComponentType*)this))[i]);}
	
	const TargetValueType& operator [](int i) const {return (TargetValueType&)((*((ComponentType*)this))[i]);}

	TargetValueType& at(int i){return (TargetValueType&)(((ComponentType*)this)->at(i));}

};

///============================================================================
/// Polaris_Container - stl Container implementation
///============================================================================

template<typename ContainerType>
struct Polaris_Container:public ContainerType
{
	typedef Polaris_Container This_Type;
	typedef ContainerType Container_Type;
	
	typedef typename remove_pointer<typename ContainerType::value_type>::type unqualified_value_type;
};
















///============================================================================
/// Polaris_Trivial_Iterator_Prototype - stl Trivial Iterator prototype
///============================================================================

//template<typename ComponentType,typename CallerType=NULLTYPE>
//struct Polaris_Trivial_Iterator_Prototype
//{
//	typename ComponentType::value_type& operator*(){return *((ComponentType*)this);}
//
//	typename ComponentType::value_type* operator->(){return ((ComponentType*)this);}
//};

///============================================================================
/// Polaris_Trivial_Iterator - stl Trivial Iterator implementation
///============================================================================

//template<typename TrivialIteratorType>
//struct Polaris_Trivial_Iterator:public TrivialIteratorType
//{
//	typedef TrivialIteratorType Implementation_Type;
//	typedef Polaris_Trivial_Iterator This_Type;
//
//	typedef typename TrivialIteratorType::value_type stored_type;	
//	typedef strip_modifiers(stored_type) type;
//
//	template<typename ComponentType=Polaris_Trivial_Iterator,typename CallerType=NULLTYPE>
//	struct Prototype_Type{typedef Polaris_Trivial_Iterator_Prototype<ComponentType,CallerType> type;};
//};



///============================================================================
/// Polaris_Input_Iterator - stl Input Iterator implementation
///============================================================================

//template<typename InputIteratorType>
//struct Polaris_Input_Iterator:public InputIteratorType
//{
//	typedef InputIteratorType Implementation_Type;
//	typedef Polaris_Input_Iterator This_Type;
//
//	typedef typename InputIteratorType::value_type stored_type;	
//	typedef strip_modifiers(stored_type) type;
//
//	template<typename ComponentType=Polaris_Input_Iterator,typename CallerType=NULLTYPE>
//	struct Prototype_Type{typedef Polaris_Input_Iterator_Prototype<ComponentType,CallerType> type;};
//};

///============================================================================
/// Polaris_Container_Prototype - stl Container prototype
///============================================================================

//template<typename ComponentType,typename CallerType=NULLTYPE>
//struct Polaris_Container_Prototype
//{
//	typename ComponentType::iterator begin(){return ((ComponentType*)this)->begin();}
//
//	typename ComponentType::iterator end(){return ((ComponentType*)this)->end();}
//	
//	typename ComponentType::size_type size(){return ((ComponentType*)this)->size();}
//
//	typename ComponentType::size_type max_size(){return ((ComponentType*)this)->size();}
//
//	bool empty(){return ((ComponentType*)this)->empty();}
//};

///============================================================================
/// Polaris_Forward_Container_Prototype - stl Forward Container prototype
///============================================================================

//template<typename ComponentType,typename CallerType=NULLTYPE>
//struct Polaris_Forward_Container_Prototype
//{
//	typename ComponentType::iterator begin(){return ((ComponentType*)this)->begin();}
//
//	typename ComponentType::iterator end(){return ((ComponentType*)this)->end();}
//	
//	typename ComponentType::size_type size(){return ((ComponentType*)this)->size();}
//
//	typename ComponentType::size_type max_size(){return ((ComponentType*)this)->size();}
//
//	bool empty(){return ((ComponentType*)this)->empty();}
//};

///============================================================================
/// Polaris_Sequence_Prototype - stl Sequence prototype
///============================================================================

//template<typename ComponentType,typename CallerType=NULLTYPE>
//struct Polaris_Sequence_Prototype
//{
//	typename ComponentType::iterator begin(){return ((ComponentType*)this)->begin();}
//
//	typename ComponentType::iterator end(){return ((ComponentType*)this)->end();}
//	
//	typename ComponentType::size_type size(){return ((ComponentType*)this)->size();}
//
//	typename ComponentType::size_type max_size(){return ((ComponentType*)this)->size();}
//
//	bool empty(){return ((ComponentType*)this)->empty();}
//
//	typename ComponentType::reference front(){return ((ComponentType*)this)->front();}
//
//	typename ComponentType::iterator insert(typename ComponentType::iterator p, typename ComponentType::value_type t){return ((ComponentType*)this)->insert(p,t);}
//	
//	void insert(typename ComponentType::iterator p, typename ComponentType::size_type n, typename ComponentType::value_type t){return ((ComponentType*)this)->insert(p,n);}
//
//	void insert(typename ComponentType::iterator p, typename ComponentType::iterator i, typename ComponentType::iterator j){return ((ComponentType*)this)->insert(p,i,j);}
//
//	typename ComponentType::iterator erase(typename ComponentType::iterator p){return ((ComponentType*)this)->erase(p);}
//
//	typename ComponentType::iterator erase(typename ComponentType::iterator p, typename ComponentType::iterator q){return ((ComponentType*)this)->erase(p,q);}
//
//	void clear(){return ((ComponentType*)this)->clear(p);}
//
//	void resize(typename ComponentType::size_type n){return ((ComponentType*)this)->resize(n);}
//
//	void resize(typename ComponentType::size_type n,typename ComponentType::value_type t){return ((ComponentType*)this)->resize(n,t);}
//};









///============================================================================
/// Polaris_Forward_Container - stl Forward Container implementation
///============================================================================
//
//template<typename ForwardContainerType>
//struct Polaris_Forward_Container:public ForwardContainerType
//{
//	typedef Polaris_Input_Iterator<typename ForwardContainerType::iterator> Iterator_Type;
//
//	typedef ForwardContainerType Implementation_Type;
//	typedef Polaris_Forward_Container This_Type;
//
//	typedef typename Implementation_Type::value_type element_stored_type;	
//	typedef strip_modifiers(element_stored_type) element_type;
//
//	template<typename ComponentType=Polaris_Forward_Container,typename CallerType=NULLTYPE>
//	struct Prototype_Type{typedef Polaris_Forward_Container_Prototype<ComponentType,CallerType> type;};
//};
//
/////============================================================================
///// Polaris_Sequence - stl Sequence implementation
/////============================================================================
//
//template<typename SequenceType>
//struct Polaris_Sequence:public SequenceType
//{
//	typedef Polaris_Input_Iterator<typename SequenceType::iterator> Iterator_Type;
//
//	typedef SequenceType Implementation_Type;
//	typedef Polaris_Sequence This_Type;
//	
//	typedef typename Implementation_Type::value_type element_stored_type;	
//	typedef strip_modifiers(element_stored_type) element_type;
//
//	template<typename ComponentType=Polaris_Sequence,typename CallerType=NULLTYPE>
//	struct Prototype_Type{typedef Polaris_Sequence_Prototype<ComponentType,CallerType> type;};
//};
//
/////============================================================================
///// Polaris_Back_Insertion_Sequence - stl Back Insertion Sequence implementation
/////============================================================================
//
//template<typename BackInsertionSequenceType>
//struct Polaris_Back_Insertion_Sequence:public BackInsertionSequenceType
//{
//	typedef Polaris_Input_Iterator<typename BackInsertionSequenceType::iterator> Iterator_Type;
//
//	typedef BackInsertionSequenceType Implementation_Type;
//	typedef Polaris_Back_Insertion_Sequence This_Type;
//
//	typedef typename Implementation_Type::value_type element_stored_type;	
//	typedef strip_modifiers(element_stored_type) element_type;
//
//	template<typename ComponentType=Polaris_Back_Insertion_Sequence,typename CallerType=NULLTYPE>
//	struct Prototype_Type{typedef Polaris_Back_Insertion_Sequence_Prototype<ComponentType,CallerType> type;};
//};
