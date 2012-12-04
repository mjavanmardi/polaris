#pragma once

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <exception>

typedef unsigned int uint;

using namespace std;

//-------------------------------------------------------------
//	MULTIDIMENSIONAL ARRAY CLASS WITH INDETERMINATE DIMENSIONS
//-------------------------------------------------------------
template<class _Ty>
class m_array_iterator
{

	_Ty *m_Ptr;                                            // pointer to container value type
	vector<int>* _dimensions;								// dimensional sizes 
	vector<int> _cursor;
	vector<_Ty>* _data;
	int _fixed_dim;
	int _fixed_dim_val;


public:
       typedef std::random_access_iterator_tag	iterator_category;
       typedef _Ty								value_type;
       typedef value_type &						reference;
       typedef value_type   *					pointer;
       typedef int								difference_type;

      m_array_iterator()
       {      // default constructor
		   _fixed_dim = -1;
		   _fixed_dim_val = -1;
		   m_Ptr = NULL;
       }

      m_array_iterator(vector<_Ty>* data, vector<int>* dim_sizes, vector<int>* cursor)
       {	// copy constructor
			_fixed_dim = -1;
			_fixed_dim_val = -1;
			_dimensions = dim_sizes;
			for (int i = 0; i< cursor->size(); i++) _cursor.push_back((*cursor)[i]);
			_data = data;
			m_Ptr = &(*data)[get_index(_cursor)];
       }

	  m_array_iterator(vector<_Ty>* data, vector<int>* dim_sizes, vector<int>* cursor, int fixed_dimension_index, int fixed_dimension_value)
       {	// copy constructor
			_fixed_dim = fixed_dimension_index;
			_fixed_dim_val = fixed_dimension_value;
			_dimensions = dim_sizes;
			for (int i = 0; i< cursor->size(); i++) _cursor.push_back((*cursor)[i]);
			_cursor[_fixed_dim] = _fixed_dim_val;
			_data = data;
			m_Ptr = &(*data)[get_index(_cursor)];
       }

      reference operator*() const
       {  
		   // return designated object
           return (*m_Ptr);
       }

	   m_array_iterator& operator=(const m_array_iterator& obj)
       {      // predecrement
              m_Ptr = obj.m_Ptr;
			  _dimensions = obj._dimensions;
			  _data = obj._data;
			  _cursor = obj._cursor;
			  _fixed_dim = obj._fixed_dim;
			  _fixed_dim_val = obj._fixed_dim_val;

             return (*this);
       }

       m_array_iterator& operator++()
       {    // preincrement
			//++m_Ptr;
			//return (*this);

			for (int d = _dimensions->size()-1; d >= 0; d--)
			{
				if (d != _fixed_dim)
				{
					++(_cursor)[d];

					if ((_cursor)[d] >= (*_dimensions)[d])
					{
						(_cursor)[d] = 0;

						if (d == 0 || (d == 1 && _fixed_dim == 0))
						{
							return m_array_iterator();
						}
					}
					else break;
				}
			}
			m_Ptr = &(*_data)[get_index(_cursor)];

			return (*this);
       }

      m_array_iterator& operator--()
       {      // predecrement
              --m_Ptr;

             return (*this);
       }

      m_array_iterator operator++(int)
       {      // postincrement
              m_array_iterator _Tmp = *this;
              ++*this;
              return (_Tmp);
       }

      m_array_iterator operator--(int)

      {      // postdecrement
              m_array_iterator _Tmp = *this;
              --*this;
              return (_Tmp);
       }

      bool operator==(const m_array_iterator &_Right) const
       {      // test for iterator equality
              return (m_Ptr == _Right.m_Ptr);
       }

      bool operator!=(const m_array_iterator &_Right) const
       {      // test for iterator inequality
              return (!(m_Ptr == _Right.m_Ptr));
       }

	uint get_index(vector<int> index)
	{
		uint ind=0;

		if (index.size() != _dimensions->size()) throw new std::exception("Error, incorrect number of dimensions in index.");

		for (int i = 0; i< index.size(); i++)
		{
			if (index[i] >= (*_dimensions)[i]) throw new std::exception( string("Error, index outside of array bounds for dimension: %i", i).c_str());

			int multiplier = 1;
			for (int j=i+1; j< index.size(); j++)
			{
				multiplier = multiplier * (*_dimensions)[j];
			}

			ind += index[i] * multiplier;
		}
		return ind;
	}

 };



template <class T>
class m_array : public vector<T>
{
public:

	typedef T							value_type;
	typedef value_type&					reference;
	typedef const value_type&			const_reference;
	typedef m_array_iterator<T>			iterator;
	typedef const m_array_iterator<T>	const_iterator;
	typedef int							difference_type;
	typedef int							size_type;

	iterator begin()
    {
		for (int i = 0; i< _dim_sizes.size(); i++) _cursor[i] = 0;
		return iterator(this, &_dim_sizes, &_cursor);
    }
	iterator begin(int fixed_dimension_index, int fixed_dimension_value)
    {
		for (int i = 0; i< _dim_sizes.size(); i++) _cursor[i] = 0;
		return iterator(this, &_dim_sizes, &_cursor, fixed_dimension_index, fixed_dimension_value);
    }
	iterator end()
    {
		return iterator();
    }
	iterator end(int fixed_dimension_index, int fixed_dimension_value)
    {
		for (int i = 0; i< _dim_sizes.size(); i++) _cursor[i] = _dim_sizes[i]-1;
		return iterator(this, &_dim_sizes, &_cursor, fixed_dimension_index, fixed_dimension_value);
    }

	// MArray constructors/destructor
	m_array (void) : vector<T>(){_allocated=false;};
	m_array (vector<int> &dim_sizes);
	m_array (vector<int> &dim_sizes, T init_val);
	m_array (const m_array& obj);
	void Init(vector<int> &dim_sizes);
	void Copy(const m_array& obj);
	m_array& operator=(const m_array& obj);

	// Array Looping functions
	bool iterate(void);	// loop over all cells
	bool iterate(int fix_dim, int at_index); // loop over all cells not in dimension=fix_dim.  

	// MArray operators overloads for data acces
	T& operator[](vector<int> index) // get data at given index
	{
		uint i = get_index(index);
		return vector<T>::operator[](i);
	} 
	T& operator()(void) // Get data at current cursor position
	{
		uint i = get_index(_cursor);
		return vector<T>::operator[](i);
	}
	vector<int> operator()(bool returnIndex) // get index of current cursor position in the form of a vector of ints for each dimension
	{
		vector<int> i;
		for (int j=0; j<_ndim; j++) i.push_back(_cursor[j]);
		return i;
	}

	uint get_index(vector<int> &index);
	
	// Property access members
	const int& dimensions() {return _dim_sizes.size();}

	const int& dimension_size(int dimension) { return _dim_sizes[dimension];}

	friend class m_array_iterator<T>;

protected:
	vector<int> _dim_sizes;
	vector<int> _cursor;
	bool _iterating;

	bool _allocated;
	void _init(vector<int> &dim_sizes);
	void _copy(const m_array& obj);

	
	bool _advance(void); // advance to next cell by shifting cursor
	bool _advance(int fixed_dim); // advance to next cell (not in the fixed dimension) by shifting cursor
	void _goToStart(void);
	T& getCurrentCell(void)
	{
		uint i = get_index(_cursor);
		return ((vector<T>*)this)->operator[][i];
	}

};

// Multi-dim Array Constructors, copiers, assignment, etc.
template <class T>
m_array<T>::m_array(vector<int> &dim_sizes) : vector<T>()
{
	_init(dim_sizes);
}
template <class T>
m_array<T>::m_array(vector<int> &dim_sizes, T init_val) : vector<T>()
{
	_init(dim_sizes);
	vector<T>::iterator itr = ((vector<T>*)this)->begin();
	for (;itr!= ((vector<T>*)this)->end(); itr++) *itr = init_val;
	//for (int i=0; i<_size; i++) _data[i]=init_val;
}
template <class T>
m_array<T>::m_array(const m_array<T>& obj)
{
	_copy(obj);
}
template <class T>
void m_array<T>::Copy(const m_array<T>& obj)
{
	if (this==&obj) return;
	_cleanup();
	_copy(obj);
}
template <class T>
void m_array<T>::_copy(const m_array<T>& obj)
{
	_dim_sizes.clear();
	_cursor.clear();
	_ndim = obj._ndim;
	_size = obj._size;
	_iterating = false;

	for (int i=0; i<obj._dim_sizes.size(); i++)
	{
		_dim_sizes.push_back(obj._dim_sizes[i]);
		_cursor.push_back(0);
	}

	_data = new T[_size];

	for (int i=0; i<_size; i++) _data[i] = obj._data[i];
	_allocated=true;
}
template <class T>
void m_array<T>::Init(vector<int> &dim_sizes)
{
	_cleanup();
	_init(dim_sizes);
}
template <class T>
void m_array<T>::_init(vector<int> &dim_sizes)
{
	_iterating = false;
	int size = 1;

	for (int i=0; i<dim_sizes.size(); i++)
	{
		_dim_sizes.push_back(dim_sizes[i]);
		_cursor.push_back(0);
		size = size * dim_sizes[i];
	}
	
	for (int i = 0; i<size; i++)	
	{
		vector<T>::push_back((T)0);
	}
	_allocated=true;
}
template <class T>
m_array<T>& m_array<T>::operator=(const m_array<T>& obj)
{
	if (this != &obj)
	{
		_cleanup();
		_copy(obj);
	}
	return *this;
}



template <class T>
uint m_array<T>::get_index(vector<int> &index)
{
	uint ind=0;

	if (index.size() != _dim_sizes.size()) throw new std::exception("Error, incorrect number of dimensions in index.");
	//{
	//	//const char* mess = "Error, incorrect number of dimensions in index.";
	//	//throw new exception(mess);
	//	////THROW_EXCEPTION(mess);
	//}
	for (int i = 0; i< index.size(); i++)
	{
		if (index[i] >= _dim_sizes[i]) throw new std::exception( string("Error, index outside of array bounds for dimension: %i", i).c_str());
		//{
		//	//const char* mess = string("Error, index outside of array bounds for dimension: %i", i).c_str();
		//	//throw new exception(mess);
		//	////THROW_EXCEPTION(mess);
		//}
		int multiplier = 1;
		for (int j=i+1; j< index.size(); j++)
		{
			multiplier = multiplier * _dim_sizes[j];
		}

		ind += index[i] * multiplier;
	}
	return ind;
}


// array functions used during iteration
template <class T>
bool m_array<T>::iterate(void)
{
	if (_iterating) 
	{
		return _advance();
	}
	else
	{
		_goToStart();
		_iterating = true;
		return true;
	}
}
template <class T>
bool m_array<T>::iterate(int fix_dim, int at_index)
{
	if (_iterating) 
	{
		return _advance(fix_dim);
	}
	else
	{
		_goToStart();
		_cursor[fix_dim] = at_index;
		_iterating = true;
		return true;
	}
}
template <class T>
bool m_array<T>::_advance(void)
{
	_cursor[_ndim-1]++;
	for (int d= _ndim-1; d>=0; d--)
	{
		if(_cursor[d]==_dim_sizes[d])
		{
			if (d>0)
			{
				_cursor[d]=0;
				_cursor[d-1]++;
			}
			else
			{
				_iterating=false;
				return false;
			}
		}
	}
	return true;
}
template <class T>
bool m_array<T>::_advance(int fixed_dim)
{
	if (fixed_dim == _ndim-1) _cursor[_ndim-2]++;
	else _cursor[_ndim-1]++;

	for (int d = _ndim-1; d>=0; d--)
	{
		if (d== fixed_dim) d--;
		if (d<0) break;

		if(_cursor[d]==_dim_sizes[d])
		{
			if (d>0)
			{
				_cursor[d]=0;
				if (d-1 == fixed_dim) 
				{
					if (d-2 < 0)
					{
						_iterating = false;
						return false;
					}
					else
					{
						_cursor[d-2]++;
					}
				}
				else _cursor[d-1]++;
			}
			else
			{
				_iterating=false;
				return false;
			}
		}
	}
	return true;
}
template <class T>
void m_array<T>::_goToStart(void)
{
	for (int d=0; d<_ndim; d++) _cursor[d]=0;
}
