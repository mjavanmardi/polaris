#pragma once
#include "Grouping.h"

///============================================================================
/// Input_Iterator - stl Input_Iterator
///============================================================================

namespace Containers
{
	namespace Concepts
	{
		concept struct Is_Associative
		{
			check_typename_state(Has_Associative_Type_Defined,Is_Associative_Type,true_type);
			define_default_check(Has_Associative_Type_Defined);
		};
		concept struct Is_Sequence
		{
			check_typename_state(Has_Sequence_Type_Defined,Is_Sequence_Type,true_type);
			define_default_check(Has_Sequence_Type_Defined);
		};

	}

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
	typedef ComponentType Component_Type;\
	typedef CallerType Caller_Type;\
	typedef true_type Is_Prototype;

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
	typedef ComponentType Component_Type;
	typedef CallerType Caller_Type;
	typedef true_type Is_Prototype;
	typedef true_type Is_Sequence_Type;

	typedef Input_Iterator<typename ComponentType::iterator,ComponentType,CallerType,TargetValueType> iterator;
	typedef Input_Iterator<typename ComponentType::reverse_iterator,ComponentType,CallerType,TargetValueType> reverse_iterator;
	typedef typename ComponentType::size_type size_type;

	iterator begin(){return (iterator)((ComponentType*)this)->begin();}

	iterator end(){return (iterator)((ComponentType*)this)->end();}

	reverse_iterator rbegin(){return (reverse_iterator)((ComponentType*)this)->rbegin();}

	reverse_iterator rend(){return (reverse_iterator)((ComponentType*)this)->rend();}
	
	size_type size(){return ((ComponentType*)this)->size();}

	size_type max_size(){return ((ComponentType*)this)->size();}

	bool empty(){return ((ComponentType*)this)->empty();}

	TargetValueType front(){return (TargetValueType)(((ComponentType*)this)->front());}

	iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
	
	void insert(iterator p, size_type n, TargetValueType t){return ((ComponentType*)this)->insert(p,n,t);}

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

template<typename ComponentType,typename CallerType=NULLTYPE,typename TargetValueType=typename ComponentType::mapped_type> 
struct Associative_Container_Prototype : public ComponentType
{
	typedef ComponentType Component_Type;
	typedef CallerType Caller_Type;
	typedef true_type Is_Prototype;
	typedef true_type Is_Associative_Type;

	typedef typename ComponentType::size_type size_type;
	typedef typename ComponentType::key_type key_type;
	//typedef typename ComponentType::key_compare key_compare;
	//typedef typename ComponentType::value_compare value_compare;
	typedef typename ComponentType::mapped_type Val_Type;

	typedef Input_Iterator<typename ComponentType::iterator,ComponentType,CallerType,pair<key_type,TargetValueType>> iterator;

	iterator begin(){return (iterator)((ComponentType*)this)->begin();}

	iterator end(){return (iterator)((ComponentType*)this)->end();}
	
	size_type size(){return ((ComponentType*)this)->size();}

	size_type max_size(){return ((ComponentType*)this)->size();}

	bool empty(){return ((ComponentType*)this)->empty();}

	pair<iterator,bool> insert(pair<key_type,TargetValueType>& p)
	{
		return ((ComponentType*)this)->insert(pair<key_type,typename Component_Type::mapped_type>(p.first,(typename Component_Type::mapped_type)(p.second)));
	}

	iterator insert(key_type key, TargetValueType& value)
	{	
		return ((ComponentType*)this)->insert(pair<key_type,typename Component_Type::mapped_type>(key,(typename Component_Type::mapped_type)value));
	}
	iterator insert(key_type key, TargetValueType&& value)
	{
		return ((ComponentType*)this)->insert(pair<key_type,typename Component_Type::mapped_type&&>(key,(typename Component_Type::mapped_type&&)value));
	}

	//iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
	//
	//void insert(iterator p, iterator i, TargetValueType t){return ((ComponentType*)this)->insert(p,i,t);}

	iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
	iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

	void clear(){return ((ComponentType*)this)->clear();}

	//key_compare key_comp() const { return ((ComponentType*)this)->key_comp();}

	//value_compare value_comp() const { return ((ComponentType*)this)->value_comp();}

	iterator find ( const key_type& x ) { return ((ComponentType*)this)->find(x);}

	pair<iterator,iterator>  equal_range ( const key_type& x ) { return ((ComponentType*)this)->equal_range(x);}

};

///============================================================================
/// Multidimensional_Random_Access_Sequence_Array_Prototype - custom (note, not a sequence as insertion does not work with multiple dimensions
///============================================================================

template<typename ComponentType,typename CallerType=NULLTYPE,typename TargetValueType=typename ComponentType::value_type> 
struct Multidimensional_Random_Access_Array_Prototype : public ComponentType
{
	typedef ComponentType Component_Type;
	typedef CallerType Caller_Type;
	typedef true_type Is_Prototype;
	typedef true_type Is_Sequence_Type;

	typedef Input_Iterator<typename ComponentType::iterator,ComponentType,CallerType,TargetValueType> iterator;
	typedef typename ComponentType::size_type size_type;
	typedef typename ComponentType::index_type index_type;
	typedef typename ComponentType::const_index_type const_index_type;
	typedef typename ComponentType::const_dimensional_type const_dimensional_type;

	iterator begin(){return (iterator)((ComponentType*)this)->begin();}
	iterator begin(size_type dim){return (iterator)((ComponentType*)this)->begin(dim);}
	iterator begin(size_type dim, size_type index){return (iterator)((ComponentType*)this)->begin(dim,index);}

	iterator end(){return (iterator)((ComponentType*)this)->end();}
	
	size_type size(){return ((ComponentType*)this)->size();}

	size_type size(size_type dimension){return ((ComponentType*)this)->size(dimension);}

	size_type num_dimensions(){return ((ComponentType*)this)->num_dimensions();}

	const_dimensional_type dimensions(){return ((ComponentType*)this)->dimensions();}

	size_type max_size(){return ((ComponentType*)this)->size();}

	bool empty(){return ((ComponentType*)this)->empty();}

	TargetValueType& front(){return (TargetValueType&)(((ComponentType*)this)->front());}
	TargetValueType& front(size_type dimension){return (TargetValueType&)(((ComponentType*)this)->front(dimension));}
	TargetValueType& front(size_type dimension, size_type index){return (TargetValueType&)(((ComponentType*)this)->front(dimension, index));}

	void clear(){return ((ComponentType*)this)->clear();}

	void Copy(const Multidimensional_Random_Access_Array_Prototype<ComponentType,CallerType,TargetValueType>& obj)
	{
		((ComponentType*)this)->Copy(*((typename Multidimensional_Random_Access_Array_Prototype<ComponentType,CallerType,TargetValueType>::Component_Type::Container_Type*)&obj));
	}

	void write(ostream& s){return ((ComponentType*)this)->print(s);}

	void resize(const_dimensional_type n){return ((ComponentType*)this)->resize(n);}

	void resize(const_dimensional_type n, TargetValueType t){return ((ComponentType*)this)->resize(n,t);}

	TargetValueType& back(){return (TargetValueType&)(((ComponentType*)this)->back());}
	TargetValueType& back(size_type dimension){return (TargetValueType&)(((ComponentType*)this)->back(dimension));}
	TargetValueType& back(size_type dimension, size_type index){return (TargetValueType&)(((ComponentType*)this)->back(dimension, index));}

	TargetValueType& operator [](const_index_type i){return (TargetValueType&)((*((ComponentType*)this))[i]);}
	const TargetValueType& operator [](const_index_type i) const {return (TargetValueType&)((*((ComponentType*)this))[i]);}
	TargetValueType& operator [](size_type i) {return (TargetValueType&)((*((ComponentType*)this))[i]);}
	const TargetValueType& operator [](size_type i) const {return (TargetValueType&)((*((ComponentType*)this))[i]);}

	TargetValueType& at(const_index_type i){return (TargetValueType&)(((ComponentType*)this)->at(i));}

};

///============================================================================
/// Polaris_Container - stl Container implementation
///============================================================================

//template<typename ContainerType, typename QualifiedValueType = typename ContainerType::value_type>
//struct Polaris_Container:public ContainerType
//{
//	typedef Polaris_Container This_Type;
//	typedef ContainerType Container_Type;
//	typedef typename remove_pointer<QualifiedValueType>::type unqualified_value_type;
//};

template<typename ContainerType>
struct Polaris_Container:public ContainerType
{
	typedef Polaris_Container This_Type;
	typedef ContainerType Container_Type;
	typedef typename remove_pointer<typename ContainerType::value_type>::type unqualified_value_type;
};

template<typename ContainerType>
struct Polaris_Associative_Container:public ContainerType
{
	typedef Polaris_Associative_Container This_Type;
	typedef ContainerType Container_Type;
	typedef typename remove_pointer<typename ContainerType::mapped_type>::type unqualified_value_type;
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

}

using namespace Containers;