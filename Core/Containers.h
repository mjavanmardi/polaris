#pragma once
#include "Basic_Concepts.h"

///============================================================================
/// Polaris_Trivial_Iterator_Interface - stl Trivial Iterator interface
///============================================================================
template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Trivial_Iterator_Interface
{
	typename ThisType::value_type& operator*(){return *((ThisType*)this);}

	typename ThisType::value_type* operator->(){return ((ThisType*)this);}
};

///============================================================================
/// Polaris_Trivial_Iterator - stl Trivial Iterator base
///============================================================================
template<typename TrivialIteratorType>
struct Polaris_Trivial_Iterator:public TrivialIteratorType
{
	typedef TrivialIteratorType Base_Type;
	typedef Polaris_Trivial_Iterator This_Type;

	typedef typename TrivialIteratorType::value_type stored_type;	
	typedef strip_modifiers(stored_type) type;

	template<typename ThisType=Polaris_Trivial_Iterator,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Trivial_Iterator_Interface<ThisType,CallerType> type;};
};

///============================================================================
/// Polaris_Input_Iterator - stl Input Iterator base
///============================================================================

template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Input_Iterator_Interface
{
	typename ThisType::value_type& operator*(){return *((ThisType*)this);}

	typename ThisType::value_type* operator->(){return ((ThisType*)this);}

	typename ThisType::Iterator_Type& operator++(){return ++(*((ThisType*)this));}

	void operator++(int){return (*((ThisType*)this))++;}
};

///============================================================================
/// Polaris_Input_Iterator - stl Input Iterator base
///============================================================================
template<typename InputIteratorType>
struct Polaris_Input_Iterator:public InputIteratorType
{
	typedef InputIteratorType Base_Type;
	typedef Polaris_Input_Iterator This_Type;

	typedef typename InputIteratorType::value_type stored_type;	
	typedef strip_modifiers(stored_type) type;

	template<typename ThisType=Polaris_Input_Iterator,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Input_Iterator_Interface<ThisType,CallerType> type;};
};

///============================================================================
/// Polaris_Container_Interface - stl Container interface
///============================================================================
template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Container_Interface
{
	typename ThisType::iterator begin(){return ((ThisType*)this)->begin();}

	typename ThisType::iterator end(){return ((ThisType*)this)->end();}
	
	typename ThisType::size_type size(){return ((ThisType*)this)->size();}

	typename ThisType::size_type max_size(){return ((ThisType*)this)->size();}

	bool empty(){return ((ThisType*)this)->empty();}
};

///============================================================================
/// Polaris_Forward_Container_Interface - stl Forward Container interface
///============================================================================
template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Forward_Container_Interface
{
	typename ThisType::iterator begin(){return ((ThisType*)this)->begin();}

	typename ThisType::iterator end(){return ((ThisType*)this)->end();}
	
	typename ThisType::size_type size(){return ((ThisType*)this)->size();}

	typename ThisType::size_type max_size(){return ((ThisType*)this)->size();}

	bool empty(){return ((ThisType*)this)->empty();}
};

///============================================================================
/// Polaris_Sequence_Interface - stl Sequence interface
///============================================================================
template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Sequence_Interface
{
	typename ThisType::iterator begin(){return ((ThisType*)this)->begin();}

	typename ThisType::iterator end(){return ((ThisType*)this)->end();}
	
	typename ThisType::size_type size(){return ((ThisType*)this)->size();}

	typename ThisType::size_type max_size(){return ((ThisType*)this)->size();}

	bool empty(){return ((ThisType*)this)->empty();}

	typename ThisType::reference front(){return ((ThisType*)this)->front();}

	typename ThisType::iterator insert(typename ThisType::iterator p, typename ThisType::value_type t){return ((ThisType*)this)->insert(p,t);}
	
	void insert(typename ThisType::iterator p, typename ThisType::size_type n, typename ThisType::value_type t){return ((ThisType*)this)->insert(p,n);}

	void insert(typename ThisType::iterator p, typename ThisType::iterator i, typename ThisType::iterator j){return ((ThisType*)this)->insert(p,i,j);}

	typename ThisType::iterator erase(typename ThisType::iterator p){return ((ThisType*)this)->erase(p);}

	typename ThisType::iterator erase(typename ThisType::iterator p, typename ThisType::iterator q){return ((ThisType*)this)->erase(p,q);}

	void clear(){return ((ThisType*)this)->clear(p);}

	void resize(typename ThisType::size_type n){return ((ThisType*)this)->resize(n);}

	void resize(typename ThisType::size_type n,typename ThisType::value_type t){return ((ThisType*)this)->resize(n,t);}
};

///============================================================================
/// Polaris_Back_Insertion_Sequence_Interface - stl Back Insertion Sequence interface
///============================================================================
template<typename ThisType,typename CallerType=NULLTYPE>
struct Polaris_Back_Insertion_Sequence_Interface
{
	typename ThisType::iterator begin(){return ((ThisType*)this)->begin();}

	typename ThisType::iterator end(){return ((ThisType*)this)->end();}
	
	typename ThisType::size_type size(){return ((ThisType*)this)->size();}

	typename ThisType::size_type max_size(){return ((ThisType*)this)->size();}

	bool empty(){return ((ThisType*)this)->empty();}

	typename ThisType::reference front(){return ((ThisType*)this)->front();}

	typename ThisType::iterator insert(typename ThisType::iterator p, typename ThisType::value_type t){return ((ThisType*)this)->insert(p,t);}
	
	void insert(typename ThisType::iterator p, typename ThisType::size_type n, typename ThisType::value_type t){return ((ThisType*)this)->insert(p,n);}

	void insert(typename ThisType::iterator p, typename ThisType::iterator i, typename ThisType::iterator j){return ((ThisType*)this)->insert(p,i,j);}

	typename ThisType::iterator erase(typename ThisType::iterator p){return ((ThisType*)this)->erase(p);}
	
	typename ThisType::iterator erase(typename ThisType::iterator p, typename ThisType::iterator q){return ((ThisType*)this)->erase(p,q);}

	void clear(){return ((ThisType*)this)->clear(p);}

	void resize(typename ThisType::size_type n){return ((ThisType*)this)->resize(n);}
	
	void resize(typename ThisType::size_type n,typename ThisType::value_type t){return ((ThisType*)this)->resize(n,t);}

	typename ThisType::reference back(){return ((ThisType*)this)->back();}

	void push_back(typename ThisType::value_type t){return ((ThisType*)this)->push_back(t);}

	void pop_back(){return ((ThisType*)this)->pop_back();}
};

///============================================================================
/// Polaris_Container - stl Container base
///============================================================================
template<typename ContainerType>
struct Polaris_Container:public ContainerType
{
	typedef Polaris_Input_Iterator<typename ContainerType::iterator> Iterator_Type;

	typedef ContainerType Base_Type;
	typedef Polaris_Container This_Type;

	typedef typename Base_Type::value_type element_stored_type;	
	typedef strip_modifiers(element_stored_type) element_type;

	template<typename ThisType=Polaris_Container,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Container_Interface<ThisType,CallerType> type;};
};

///============================================================================
/// Polaris_Forward_Container - stl Forward Container base
///============================================================================
template<typename ForwardContainerType>
struct Polaris_Forward_Container:public ForwardContainerType
{
	typedef Polaris_Input_Iterator<typename ForwardContainerType::iterator> Iterator_Type;

	typedef ForwardContainerType Base_Type;
	typedef Polaris_Forward_Container This_Type;

	typedef typename Base_Type::value_type element_stored_type;	
	typedef strip_modifiers(element_stored_type) element_type;

	template<typename ThisType=Polaris_Forward_Container,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Forward_Container_Interface<ThisType,CallerType> type;};
};

///============================================================================
/// Polaris_Sequence - stl Sequence base
///============================================================================
template<typename SequenceType>
struct Polaris_Sequence:public SequenceType
{
	typedef Polaris_Input_Iterator<typename SequenceType::iterator> Iterator_Type;

	typedef SequenceType Base_Type;
	typedef Polaris_Sequence This_Type;
	
	typedef typename Base_Type::value_type element_stored_type;	
	typedef strip_modifiers(element_stored_type) element_type;

	template<typename ThisType=Polaris_Sequence,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Sequence_Interface<ThisType,CallerType> type;};
};

///============================================================================
/// Polaris_Back_Insertion_Sequence - stl Back Insertion Sequence base
///============================================================================
template<typename BackInsertionSequenceType>
struct Polaris_Back_Insertion_Sequence:public BackInsertionSequenceType
{
	typedef Polaris_Input_Iterator<typename BackInsertionSequenceType::iterator> Iterator_Type;

	typedef BackInsertionSequenceType Base_Type;
	typedef Polaris_Back_Insertion_Sequence This_Type;

	typedef typename Base_Type::value_type element_stored_type;	
	typedef strip_modifiers(element_stored_type) element_type;

	template<typename ThisType=Polaris_Back_Insertion_Sequence,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef Polaris_Back_Insertion_Sequence_Interface<ThisType,CallerType> type;};
};

