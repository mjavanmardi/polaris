#pragma once
///----------------------------------------------------------------------------------------------------
/// Container_Forms.h - POLARIS Container Forms Library
///----------------------------------------------------------------------------------------------------

#include "Forms.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// General Macros
	///----------------------------------------------------------------------------------------------------

	#define get_component_type(CONTAINERTYPE) typename remove_pointer<typename CONTAINERTYPE::value_type>::type::Component_Type
	#define get_mapped_component_type(CONTAINERTYPE) typename remove_pointer<typename CONTAINERTYPE::value_type::second_type>::type::Component_Type

	#define get_component_type_test(NAME) NAME##_accessible_value_type

	#define get_value_type(CONTAINERTYPE) typename CONTAINERTYPE::value_type
	#define get_mapped_value_type(CONTAINERTYPE) typename CONTAINERTYPE::value_type::second_type
	//#define get_mapped_type(CONTAINERTYPE) typename remove_pointer<typename CONTAINERTYPE::mapped_type>::type::Component_Type

	///----------------------------------------------------------------------------------------------------
	/// Input_Iterator - stl Input_Iterator
	///----------------------------------------------------------------------------------------------------

	//template<typename IteratorType>
	//struct Input_Iterator
	//{
	//	Input_Iterator():IteratorType(){};

	//	//Input_Iterator(IteratorType val):IteratorType(val){}

	//	typename IteratorType::value_type& operator*(){return (IteratorType::value_type&)(((IteratorType*)this)->operator*());}

	//	typename IteratorType::value_type* operator->(){return (IteratorType::value_type*)(((IteratorType*)this)->operator->());}

	//	Input_Iterator& operator++(){return (Input_Iterator&)++(*((IteratorType*)this));}
	//
	//	bool operator!=(const Input_Iterator& val){return (*((IteratorType*)this))!=(*((IteratorType*)&val));}

	//	/*void operator++(int){(*((IteratorType*)this))++;}*/

	//	Input_Iterator operator++(int)
	//	{
	//		Input_Iterator a = (*((IteratorType*)this));
	//		(*((IteratorType*)this))++;
	//		return a;
	//	}
	//};

	///----------------------------------------------------------------------------------------------------
	/// Sequence - stl Sequence prototype
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,typename T>
	struct Sequence
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		//typedef Input_Iterator<typename ComponentType::iterator> iterator;
		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::size_type size_type;
		typedef T value_type;

		iterator begin(){return ((ComponentType*)this)->begin();}

		iterator end(){return ((ComponentType*)this)->end();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T&)(((ComponentType*)this)->front());}

		iterator insert(iterator p, T& t){return ((ComponentType*)this)->insert(p,(typename ComponentType::value_type&)t);}

		void insert(iterator p, size_type n, T& t){return ((ComponentType*)this)->insert(p,n);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, T& t){return ((ComponentType*)this)->resize(n,t);}

		void pop_front(){((ComponentType*)this)->pop_front();}
	};

	///----------------------------------------------------------------------------------------------------
	/// Prototype_Sequence - stl Sequence for POLARIS prototypes
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,template<typename T> class value_prototype>
	struct Prototype_Sequence
	{
		static_assert(is_pointer<typename ComponentType::value_type>::value,"Container must hold pointer types");
		//requires_typedef_type(contains_prototype,Is_Prototype,true_type);

		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		//typedef Input_Iterator<typename ComponentType::iterator> iterator;
		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::size_type size_type;

		typedef value_prototype<typename remove_pointer<typename ComponentType::value_type>::type::Component_Type>* value_type;
		typedef value_type T;

		iterator begin(){return ((ComponentType*)this)->begin();}

		iterator end(){return ((ComponentType*)this)->end();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T&)(((ComponentType*)this)->front());}

		iterator insert(iterator p, T t){return ((ComponentType*)this)->insert(p,t);}

		void insert(iterator p, size_type n, T& t){return ((ComponentType*)this)->insert(p,n);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, T& t){return ((ComponentType*)this)->resize(n,t);}

		void pop_front(){((ComponentType*)this)->pop_front();}
	};

	///----------------------------------------------------------------------------------------------------
	/// Back_Insertion_Sequence - stl Back Insertion Sequence prototype
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,typename T = typename ComponentType::value_type>
	struct Back_Insertion_Sequence
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		//typedef Input_Iterator<typename ComponentType::iterator> iterator;
		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::size_type size_type;
		typedef T value_type;

		iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		iterator end(){return (iterator)((ComponentType*)this)->end();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T&)(((ComponentType*)this)->front());}

		//iterator insert(iterator p, T t){return ((ComponentType*)this)->insert(p,t);}
		iterator insert(iterator p, T& t){return ((ComponentType*)this)->insert(p,(typename ComponentType::value_type&)t);}

		void insert(iterator p, size_type n, T& t){return ((ComponentType*)this)->insert(p,n);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, T& t){return ((ComponentType*)this)->resize(n,t);}

		T& back(){return (T&)(((ComponentType*)this)->back());}
	
		void push_back(T& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}
	
		void push_back(T&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}
	
		void pop_back(){((ComponentType*)this)->pop_back();}

		void pop_front(){((ComponentType*)this)->pop_front();}

		void sort(){((ComponentType*)this)->sort();}
	};
	
	///----------------------------------------------------------------------------------------------------
	/// Prototype_Back_Insertion_Sequence - stl Back Insertion Sequence prototype
	///----------------------------------------------------------------------------------------------------
	
	template<typename ComponentType,template<typename T> class value_prototype>
	struct Prototype_Back_Insertion_Sequence
	{
		static_assert(is_pointer<typename ComponentType::value_type>::value,"Container must hold pointer types");
		//requires_typedef_type(contains_prototype,Is_Prototype,true_type);

		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		//typedef Input_Iterator<typename ComponentType::iterator> iterator;
		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::size_type size_type;
		typedef value_prototype<typename remove_pointer<typename ComponentType::value_type>::type::Component_Type>* value_type;
		typedef value_type T;

		iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		iterator end(){return (iterator)((ComponentType*)this)->end();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T)(((ComponentType*)this)->front());}

		//iterator insert(iterator p, T t){return ((ComponentType*)this)->insert(p,t);}
		iterator insert(iterator p, T& t){return ((ComponentType*)this)->insert(p,(typename ComponentType::value_type&)t);}

		void insert(iterator p, size_type n, T t){return ((ComponentType*)this)->insert(p,n);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, T t){return ((ComponentType*)this)->resize(n,t);}

		T back(){return (T)(((ComponentType*)this)->back());}
	
		void push_back(T& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}
	
		void push_back(T&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}
	
		void pop_back(){((ComponentType*)this)->pop_back();}

		void pop_front(){((ComponentType*)this)->pop_front();}

		void sort(){((ComponentType*)this)->sort();}
	};

	///----------------------------------------------------------------------------------------------------
	/// Random_Access_Sequence - stl Random Access Sequence prototype
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,typename T = typename ComponentType::value_type>
	struct Random_Access_Sequence ADD_DEBUG_INFO
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::reverse_iterator reverse_iterator;
	
		typedef typename ComponentType::size_type size_type;
		typedef T value_type;

		iterator begin(){return (iterator)(static_cast<ComponentType*>(this))->begin();}

		iterator end(){return (iterator)(static_cast<ComponentType*>(this))->end();}

		reverse_iterator rbegin(){return (reverse_iterator)((ComponentType*)this)->rbegin();}

		reverse_iterator rend(){return (reverse_iterator)((ComponentType*)this)->rend();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T&)(((ComponentType*)this)->front());}

		iterator insert(iterator p, T& t){return ((ComponentType*)this)->insert(p,t);}
	
		void insert(iterator p, size_type n, T& t){return ((ComponentType*)this)->insert(p,n,t);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, const T& t){return ((ComponentType*)this)->resize(n,t);}

		T& back(){return (T&)(((ComponentType*)this)->back());}
	
		void push_back(T& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}

		void push_back(T&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}

		void pop_back(){((ComponentType*)this)->pop_back();}

		void pop_front(){((ComponentType*)this)->pop_front();}

		T& operator [](int i){return (T&)((*((ComponentType*)this))[i]);}
	
		const T& operator [](int i) const {return (T&)((*((ComponentType*)this))[i]);}

		T& at(int i){return (T&)(((ComponentType*)this)->at(i));}
	};

	///----------------------------------------------------------------------------------------------------
	/// Prototype_Random_Access_Sequence - stl Random Access Sequence prototype
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,template<typename T> class value_prototype>
	struct Prototype_Random_Access_Sequence
	{
		static_assert(is_pointer<typename ComponentType::value_type>::value,"Container must hold pointer types");

		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		typedef typename ComponentType::iterator iterator;
		typedef typename ComponentType::reverse_iterator reverse_iterator;
	
		typedef typename ComponentType::size_type size_type;
		typedef value_prototype<typename remove_pointer<typename ComponentType::value_type>::type::Component_Type>* value_type;
		typedef value_type T;

		iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		iterator end(){return (iterator)((ComponentType*)this)->end();}

		reverse_iterator rbegin(){return (reverse_iterator)((ComponentType*)this)->rbegin();}

		reverse_iterator rend(){return (reverse_iterator)((ComponentType*)this)->rend();}
	
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		T& front(){return (T&)(((ComponentType*)this)->front());}

		iterator insert(iterator p, T& t){return ((ComponentType*)this)->insert(p,t);}
	
		void insert(iterator p, size_type n, T& t){return ((ComponentType*)this)->insert(p,n,t);}

		void insert(iterator p, iterator i, iterator j){return ((ComponentType*)this)->insert(p,i,j);}

		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
	
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void resize(size_type n){return ((ComponentType*)this)->resize(n);}
	
		void resize(size_type n, T& t){return ((ComponentType*)this)->resize(n,t);}

		T& back(){return (T&)(((ComponentType*)this)->back());}
	
		void push_back(T& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&)t);}

		void push_back(T&& t){return ((ComponentType*)this)->push_back((typename ComponentType::value_type&&)t);}

		void pop_back(){((ComponentType*)this)->pop_back();}

		void pop_front(){((ComponentType*)this)->pop_front();}

		T& operator [](int i){return (T&)((*((ComponentType*)this))[i]);}
	
		const T& operator [](int i) const {return (T&)((*((ComponentType*)this))[i]);}

		T& at(int i){return (T&)(((ComponentType*)this)->at(i));}
	};

	template<typename ComponentType,typename T = typename ComponentType::value_type> 
	struct Multidimensional_Random_Access_Array ADD_DEBUG_INFO
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;
		typedef true_type Is_Sequence_Type;

		typedef typename ComponentType::iterator iterator;

		typedef typename ComponentType::size_type size_type;
		typedef typename ComponentType::const_size_type const_size_type;
		typedef typename ComponentType::index_type index_type;
		typedef typename ComponentType::const_index_type const_index_type;
		typedef typename ComponentType::const_dimensional_type const_dimensional_type;
		typedef typename ComponentType::dimensional_iterator dimensional_iterator;
		typedef T value_type;

		Multidimensional_Random_Access_Array() 
		{
			ComponentType();
		}

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

		T& front(){return (T&)(((ComponentType*)this)->front());}
		T& front(size_type dimension){return (T&)(((ComponentType*)this)->front(dimension));}
		T& front(size_type dimension, size_type index){return (T&)(((ComponentType*)this)->front(dimension, index));}

		void clear(){return ((ComponentType*)this)->clear();}

		void scale(const T& value){((ComponentType*)this)->scale(value);} 

		void Copy(const Multidimensional_Random_Access_Array<ComponentType,T>& obj)
		{
			((ComponentType*)this)->Copy(*((typename Multidimensional_Random_Access_Array<ComponentType,T>::Component_Type*)&obj));
		}
		void Copy(const_index_type dim_sizes, T* data)
		{
			((ComponentType*)this)->Copy(dim_sizes, data);
		}
		size_type get_index(const_index_type index)
		{
			return ((ComponentType*)this)->get_index(index);
		}
		index_type get_index(size_type index)
		{
			return ((ComponentType*)this)->get_index(index);
		}

		void write(ostream& s){return ((ComponentType*)this)->print(s);}

		void resize(const_dimensional_type n){return ((ComponentType*)this)->resize(n);}

		void resize(const_dimensional_type n, T t){return ((ComponentType*)this)->resize(n,t);}
		T sum(){return (((ComponentType*)this)->sum());}
		T sum(size_type dim, size_type index){return (((ComponentType*)this)->sum(dim,index));}
		T& back(){return (T&)(((ComponentType*)this)->back());}
		T& back(size_type dimension){return (T&)(((ComponentType*)this)->back(dimension));}
		T& back(size_type dimension, size_type index){return (T&)(((ComponentType*)this)->back(dimension, index));}

		T& operator [](const_index_type i){return (T&)((*((ComponentType*)this))[i]);}
		const T& operator [](const_index_type i) const {return (T&)((*((ComponentType*)this))[i]);}
		T& operator [](size_type i) {return (T&)((*((ComponentType*)this))[i]);}
		const T& operator [](size_type i) const {return (T&)((*((ComponentType*)this))[i]);}
		T& operator ()(size_type i, size_type j) {return (T&)((*((ComponentType*)this))(i,j));}
		const T& operator ()(size_type i, size_type j) const {return (T&)((*((ComponentType*)this))(i,j));}

		T& at(const_index_type i){return (T&)(((ComponentType*)this)->at(i));}

		T* get_data_pointer(){return((ComponentType*)this)->get_data_pointer();}

		Multidimensional_Random_Access_Array& operator+(Multidimensional_Random_Access_Array& obj)
		{
			((ComponentType*)this)->operator+(*(ComponentType*)&obj);
			return *this;
		}
		Multidimensional_Random_Access_Array& operator-(Multidimensional_Random_Access_Array& obj)
		{
			((ComponentType*)this)->operator-(*(ComponentType*)&obj);
			return *this;
		}

		// basic feature tags
		typedef true_type begin_feature_tag;
		typedef true_type end_feature_tag;
		typedef true_type insert_feature_tag;
		// back_insert feature tags
		typedef true_type push_back_feature_tag;
		typedef true_type pop_back_feature_tag;
		typedef true_type front_feature_tag;
		typedef true_type back_feature_tag;
		// Random access feature tags
		typedef true_type at_feature_tag;
		// Multidimensional feature tags
		typedef true_type dimensions_feature_tag;
	};

	///----------------------------------------------------------------------------------------------------
	/// Simple_Associative_Container - stl Simple Associative Container
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,typename K = typename ComponentType::key_type> 
	struct Simple_Associative_Container ADD_DEBUG_INFO
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;

		typedef K key_type;

		typedef K value_type;

		//typedef Input_Iterator<typename ComponentType::iterator,ComponentType,pair<key_type,TargetValueType>> iterator;

		//iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		//iterator end(){return (iterator)((ComponentType*)this)->end();}
		//
		//size_type size(){return ((ComponentType*)this)->size();}

		//size_type max_size(){return ((ComponentType*)this)->size();}

		//bool empty(){return ((ComponentType*)this)->empty();}

		//pair<iterator,bool> insert(pair<key_type,TargetValueType>& p)
		//{
		//	pair<key_type,Val_Type> t = pair<key_type,Val_Type>(p.first,(Val_Type)(p.second));
		//	return ((ComponentType*)this)->insert(t);
		//}

		//pair<iterator,bool> insert(pair<key_type,TargetValueType>&& p)
		//{
		//	pair<key_type,Val_Type> t = pair<key_type,Val_Type>(p.first,(Val_Type)(p.second));
		//	return ((ComponentType*)this)->insert(t);
		//}

		//iterator insert(key_type key, TargetValueType& value)
		//{	
		//	return ((ComponentType*)this)->insert(pair<key_type,typename Component_Type::mapped_type>(key,(typename Component_Type::mapped_type)value));
		//}
		//iterator insert(key_type key, TargetValueType&& value)
		//{
		//	return ((ComponentType*)this)->insert(pair<key_type,typename Component_Type::mapped_type&&>(key,(typename Component_Type::mapped_type&&)value));
		//}

		////iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
		////
		////void insert(iterator p, iterator i, TargetValueType t){return ((ComponentType*)this)->insert(p,i,t);}

		//size_type erase (key_type key){return ((ComponentType*)this)->erase(key);}
		//iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
		//
		//iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		//void clear(){return ((ComponentType*)this)->clear();}

		//void set_empty_key(key_type key){((ComponentType*)this)->set_empty_key(key);}

		//void set_deleted_key(key_type key){((ComponentType*)this)->set_deleted_key(key);}

		////key_compare key_comp() const { return ((ComponentType*)this)->key_comp();}

		////value_compare value_comp() const { return ((ComponentType*)this)->value_comp();}

		//iterator find ( const key_type& x ) { return ((ComponentType*)this)->find(x);} 

		//pair<iterator,iterator>  equal_range ( const key_type& x ) { return ((ComponentType*)this)->equal_range(x);}
	};

	///----------------------------------------------------------------------------------------------------
	/// Pair_Associative_Container - stl Pair Associative Container
	///----------------------------------------------------------------------------------------------------

	template<typename ComponentType,typename K = typename ComponentType::key_type,typename T = typename ComponentType::value_type::second_type> 
	struct Pair_Associative_Container ADD_DEBUG_INFO
	{
		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;
		typedef true_type Is_Associative_Type;

		typedef typename ComponentType::size_type size_type;

		typedef K key_type;
		typedef T data_type;

		typedef pair<const key_type, data_type> value_type;

		typedef typename ComponentType::iterator iterator;

		iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		iterator end(){return (iterator)((ComponentType*)this)->end();}
		
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		pair<iterator,bool> insert(pair<key_type,data_type>& p)
		{
			pair<key_type,data_type> t = pair<key_type,data_type>(p.first,(data_type)(p.second));
			return ((ComponentType*)this)->insert(t);
		}

		pair<iterator,bool> insert(pair<key_type,data_type>&& p)
		{
			pair<key_type,data_type> t = pair<key_type,data_type>(p.first,(data_type)(p.second));
			return ((ComponentType*)this)->insert(t);
		}

		//iterator insert(key_type& key, typename remove_pointer<data_type>::type* value)
		//{	
		//	return ((ComponentType*)this)->insert(pair<key_type,data_type>(key,value));
		//}
		iterator insert(key_type& key, data_type& value)
		{	
			return ((ComponentType*)this)->insert(pair<key_type,data_type>(key,value));
		}
		iterator insert(key_type& key, data_type&& value)
		{
			return ((ComponentType*)this)->insert(pair<key_type,data_type&&>(key,(data_type&&)value));
		}

		//iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
		//
		//void insert(iterator p, iterator i, TargetValueType t){return ((ComponentType*)this)->insert(p,i,t);}

		size_type erase (key_type& key){return ((ComponentType*)this)->erase(key);}
		void erase(iterator p){((ComponentType*)this)->erase(p);}
		
		void erase(iterator p, iterator q){((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void set_empty_key(key_type& key){((ComponentType*)this)->set_empty_key(key);}

		void set_deleted_key(key_type& key){((ComponentType*)this)->set_deleted_key(key);}

		//key_compare key_comp() const { return ((ComponentType*)this)->key_comp();}

		//value_compare value_comp() const { return ((ComponentType*)this)->value_comp();}

		iterator find ( const key_type& x ) { return ((ComponentType*)this)->find(x);} 

		pair<iterator,iterator>  equal_range ( const key_type& x ) { return ((ComponentType*)this)->equal_range(x);}



	};

	template<typename ComponentType,typename K = typename ComponentType::key_type, template<typename T=NULLTYPE> class value_prototype=NULLTEMPLATE>
	struct Prototype_Pair_Associative_Container ADD_DEBUG_INFO
	{
		static_assert(is_pointer<typename ComponentType::mapped_type>::value,"Container must hold pointer types");

		typedef ComponentType Component_Type;
		typedef true_type Is_Prototype;
		typedef true_type Is_Associative_Type;

		typedef typename ComponentType::size_type size_type;
	
		typedef K key_type;
		typedef value_prototype<typename remove_pointer<typename ComponentType::mapped_type>::type::Component_Type>* data_type;

		typedef data_type T;

		typedef pair<const key_type, data_type> value_type;

		typedef typename ComponentType::iterator iterator;

		iterator begin(){return (iterator)((ComponentType*)this)->begin();}

		iterator end(){return (iterator)((ComponentType*)this)->end();}
		
		size_type size(){return ((ComponentType*)this)->size();}

		size_type max_size(){return ((ComponentType*)this)->size();}

		bool empty(){return ((ComponentType*)this)->empty();}

		pair<iterator,bool> insert(pair<key_type,data_type>& p)
		{
			pair<key_type,data_type> t = pair<key_type,data_type>(p.first,(data_type)(p.second));
			return ((ComponentType*)this)->insert(t);
		}

		pair<iterator,bool> insert(pair<key_type,data_type>&& p)
		{
			pair<key_type,data_type> t = pair<key_type,data_type>(p.first,(data_type)(p.second));
			return ((ComponentType*)this)->insert(t);
		}

		//iterator insert(key_type& key, typename remove_pointer<data_type>::type* value)
		//{	
		//	return ((ComponentType*)this)->insert(pair<key_type,data_type>(key,value));
		//}
		iterator insert(key_type& key, data_type& value)
		{	
			return ((ComponentType*)this)->insert(pair<key_type,data_type>(key,value));
		}
		iterator insert(key_type& key, data_type&& value)
		{
			return ((ComponentType*)this)->insert(pair<key_type,data_type&&>(key,(data_type&&)value));
		}

		//iterator insert(iterator p, TargetValueType t){return ((ComponentType*)this)->insert(p,t);}
		//
		//void insert(iterator p, iterator i, TargetValueType t){return ((ComponentType*)this)->insert(p,i,t);}

		size_type erase (key_type& key){return ((ComponentType*)this)->erase(key);}
		iterator erase(iterator p){return ((ComponentType*)this)->erase(p);}
		
		iterator erase(iterator p, iterator q){return ((ComponentType*)this)->erase(p,q);}

		void clear(){return ((ComponentType*)this)->clear();}

		void set_empty_key(key_type& key){((ComponentType*)this)->set_empty_key(key);}

		void set_deleted_key(key_type& key){((ComponentType*)this)->set_deleted_key(key);}

		//key_compare key_comp() const { return ((ComponentType*)this)->key_comp();}

		//value_compare value_comp() const { return ((ComponentType*)this)->value_comp();}

		iterator find ( const key_type& x ) { return ((ComponentType*)this)->find(x);} 

		pair<iterator,iterator>  equal_range ( const key_type& x ) { return ((ComponentType*)this)->equal_range(x);}



	};

	template<typename KeyType, typename ValueType> bool Pair_Comparer(std::pair<KeyType,ValueType> &p1, std::pair<KeyType,ValueType> &p2)
	{
		if (p1.first < p2.first) return true;
		else return false;
	}

	template<typename T> bool Ptr_Comparer(T p1, T p2)
	{
		if (*p1 < *p2) return true;
		else return false;
	}

	///----------------------------------------------------------------------------------------------------
	/// container_accessor - implements the standard get / set accessors for a container
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define container_accessor(CONTAINER_PROTOTYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>* set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>* set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>* NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>>();\
			}\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::value_type>* NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\


	///----------------------------------------------------------------------------------------------------
	/// pair_container_accessor - implements the standard get / set accessors for a pair container
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define pair_container_accessor(CONTAINER_PROTOTYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>* set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>* set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>* NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>>();\
			}\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,typename TargetType::data_type>* NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\



	///----------------------------------------------------------------------------------------------------
	/// prototype_container_accessor - implements the standard get / set accessors for a prototype container
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define prototype_container_accessor(CONTAINER_PROTOTYPE,VALUE_PROTOTYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>* set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>*>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>* set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>* NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>*>();\
			}\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,VALUE_PROTOTYPE>* NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\


	///----------------------------------------------------------------------------------------------------
	/// prototype_pair_container_accessor - implements the get / set accessors for a prototype pair container
	///		includes a tagless check on whether the implementation has corresponding accessors
	///----------------------------------------------------------------------------------------------------

	#define prototype_pair_container_accessor(CONTAINER_PROTOTYPE,VALUE_PROTOTYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		public:\
			template<typename TypeChecked>\
			struct NAME##_set_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(void (V::* arg)(NT,char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( void (V::* arg)(NT,char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>* set_value,requires(TargetType,check(ComponentType,NAME##_set_check) && (SETTER_REQUIREMENTS)))\
			{\
				this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>*>(set_value);\
			}\
			template<typename TargetType>\
			void NAME(CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>* set_value,requires(TargetType,!check(ComponentType,NAME##_set_check) || !(SETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_set_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a setter for " #NAME " exists ---------]\n\n");\
				static_assert(SETTER_REQUIREMENTS,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");\
			}\
			\
			template<typename TypeChecked>\
			struct NAME##_get_check\
			{\
				template<typename U,bool Perform_Check = !is_same<U,NULLTYPE>::value>\
				struct function_check{ static const bool value = true; };\
				template<typename U>\
				struct function_check<U,true>\
				{\
					template<typename V> static small_type has_matching_named_member(NT (V::* arg)(char(*)[1]) = &V::NAME<NT>);\
					template<typename V> static large_type has_matching_named_member(...);\
					\
					template<typename V,bool Perform_Check = (sizeof(has_matching_named_member<U>(nullptr))==success)>\
					struct form_check{ static const bool value = false; };\
					\
					template<typename V> static small_type has_matching_formed_member( NT (V::* arg)(char(*)[1]) );\
					template<typename V> static large_type has_matching_formed_member(...);\
					\
					template<typename V>\
					struct form_check<V,true>{ static const int value = (sizeof(has_matching_formed_member<V>(&V::NAME<NT>))==success);};\
					\
					static const bool value = form_check<U>::value;\
				};\
				\
				static const bool value = function_check<TypeChecked>::value;\
			};\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>* NAME(requires(TargetType,check(ComponentType,NAME##_get_check) && (GETTER_REQUIREMENTS)))\
			{\
				return this_component()->template NAME<CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>*>();\
			}\
			template<typename TargetType>\
			CONTAINER_PROTOTYPE<TargetType,typename TargetType::key_type,VALUE_PROTOTYPE<get_mapped_component_type(TargetType)>*>* NAME(requires(TargetType,!check(ComponentType,NAME##_get_check) || !(GETTER_REQUIREMENTS)))\
			{\
				static_assert(NAME##_get_check<ComponentType>::value,"\n\n\n[--------- Can't guarantee that a getter for " #NAME " exists ---------]\n\n");\
				static_assert(GETTER_REQUIREMENTS,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");\
			}\


	///----------------------------------------------------------------------------------------------------
	/// m_container � member creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------
#define m_container(CONTAINER_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			CONTAINER_TYPE _##NAME; \
		public:\
			typedef CONTAINER_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)dereference(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(*value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((CONTAINER_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);

#define m_static_container(CONTAINER_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			static CONTAINER_TYPE _##NAME;\
		public:\
			typedef CONTAINER_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)dereference(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(*value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((CONTAINER_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\
			tag_getter_as_available(NAME);\
			tag_setter_as_available(NAME);

	///----------------------------------------------------------------------------------------------------
	/// m_prototype_container � member creator, type-definition and basic accessors
	///----------------------------------------------------------------------------------------------------

	#define m_prototype_container(CONTAINER_TYPE,COMPONENT_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			CONTAINER_TYPE _##NAME;\
		public:\
			typedef CONTAINER_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			typedef COMPONENT_TYPE NAME##_accessible_value_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)dereference(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(*value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((CONTAINER_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\

	#define m_static_prototype_container(CONTAINER_TYPE,COMPONENT_TYPE,NAME,GETTER_REQUIREMENTS,SETTER_REQUIREMENTS)\
		protected:\
			CONTAINER_TYPE _##NAME;\
		public:\
			typedef CONTAINER_TYPE NAME##_type;\
			typedef NAME##_type NAME##_accessible_type;\
			typedef COMPONENT_TYPE NAME##_accessible_value_type;\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(&_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)dereference(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (GETTER_REQUIREMENTS)       ))\
			{return (TargetType)(_##NAME);}\
			template<typename TargetType>\
			TargetType NAME(requires(TargetType,!(GETTER_REQUIREMENTS)))\
			{static_assert((GETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more getter requirements for \"" #NAME"\" could not be satisfied: { "#GETTER_REQUIREMENTS" } ---------]\n\n");}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && !check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(*value);}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (!check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=((CONTAINER_TYPE)(&value));}\
			template<typename TargetType>\
			void NAME(TargetType value,requires(TargetType,      (check(TargetType,is_pointer) && check(concat(CONTAINER_TYPE),is_pointer)) && (SETTER_REQUIREMENTS)       ))\
			{_##NAME=(CONTAINER_TYPE)(value);}\
			template<typename TargetType>\
			void NAME(TargetType value, requires(TargetType,!(SETTER_REQUIREMENTS)))\
			{static_assert((SETTER_REQUIREMENTS) && True_Concept<TargetType>::value,"\n\n\n[--------- One or more setter requirements for \"" #NAME"\" could not be satisfied: { "#SETTER_REQUIREMENTS" } ---------]\n\n");}\

}
