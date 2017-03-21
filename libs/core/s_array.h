#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>
#include <limits>
#include <cfloat>

typedef unsigned int uint;

#define THROW_SARRAY_EXCEPTION(...) \
			{stringstream s;\
			s << "\nRUNTIME_ERROR: " << __FILE__ << " at " << __LINE__<< "\n\t" << __VA_ARGS__ << endl;\
			cerr << s.str();\
			throw new runtime_error(s.str().c_str());}

using namespace std;

//-------------------------------------------------------------
//	MULTIDIMENSIONAL ARRAY CLASS WITH INDETERMINATE DIMENSIONS
//-------------------------------------------------------------
template<class T>
class s_array_iterator
{

public:
    typedef std::random_access_iterator_tag	iterator_category;
    typedef T								value_type;
    typedef value_type &						reference;
    typedef value_type   *					pointer;
    typedef int								difference_type;
	typedef uint								size_type;

    s_array_iterator()
    {      // default constructor
		_fixed_dim = -1;
		_fixed_dim_val = -1;
		m_Ptr = NULL;
    }

    s_array_iterator(T* data, std::vector<size_type>* row_sizes, pair<size_type,size_type>* cursor)
    {	// copy constructor
		_fixed_dim = -1;
		_fixed_dim_val = -1;
		_row_sizes = (std::vector<size_type>*)row_sizes;
		_cursor = pair<size_type,size_type>(cursor->first, cursor->second);

		_data = data;
		m_Ptr = &_data[get_index(_cursor)];
    }

	s_array_iterator(T* data, std::vector<size_type>* row_sizes, pair<size_type,size_type>* cursor, size_type fixed_dim, size_type fixed_dim_val)
	{		
		// copy constructor
		_fixed_dim = fixed_dim;
		_fixed_dim_val = fixed_dim_val;
		_row_sizes = (std::vector<size_type>*)row_sizes;
		_cursor = pair<size_type,size_type>(cursor->first, cursor->second);

		m_Ptr = &data[get_index(_cursor)];
		_data = data;
	}

    reference operator*() const
    {  
		// return designated object
        return (*m_Ptr);
    }

	s_array_iterator& operator=(const s_array_iterator& obj)
    {      
            m_Ptr = obj.m_Ptr;
			_row_sizes = obj._row_sizes;
			_cursor = obj._cursor;
			_data = obj._data;
			_fixed_dim = obj._fixed_dim;
			_fixed_dim_val = obj._fixed_dim_val;

            return (*this);
    }

    s_array_iterator& operator++()
    {   
		_cursor.second++;

		if (_fixed_dim > 0 && _cursor.second >= (*_row_sizes)[_fixed_dim])
		{
			m_Ptr = NULL;
			return (*this);
		}

		if (_cursor.second >=  (*_row_sizes)[_cursor.first])
		{
			_cursor.first++;
			_cursor.second = 0;
		}

		if (_cursor.first >= _row_sizes->size())
		{
			m_Ptr = NULL;
			return (*this);
		}
		
		++m_Ptr;
		return (*this);
    }

    s_array_iterator& operator--()
    {
		_cursor.second--;

		if (_fixed_dim > 0 && _cursor.second == 0)
		{
			m_Ptr = NULL;
			return (*this);
		}

		if (_cursor.second == 0)
		{
			_cursor.first--;
			_cursor.second = (uint)_row_sizes[_cursor.first]-1;
		}

		if (_cursor.first == 0)
		{
			m_Ptr = NULL;
			return (*this);
		}
		
		--m_Ptr;
		return (*this);
    }

    bool operator==(const s_array_iterator &_Right) const
    {      // test for iterator equality
            return (m_Ptr == _Right.m_Ptr);
    }

    bool operator!=(const s_array_iterator &_Right) const
    {      // test for iterator inequality
            return (!(m_Ptr == _Right.m_Ptr));
    }

	const pair<size_type,size_type>& get_index()
	{
		return _cursor;
	}

private:
	T *m_Ptr;                                            // pointer to container value type
	std::vector<size_type>* _row_sizes;								// dimensional sizes 
	pair<size_type,size_type> _cursor;
	T * _data;
	int _fixed_dim;
	int _fixed_dim_val;
	uint get_index(pair<size_type,size_type>& index)
	{
		uint ind=0;

		if (index.first >= _row_sizes->size()) THROW_SARRAY_EXCEPTION("Error, row index points to a row outside of array bounds.");

		for (uint i = 0; i< index.first; i++) ind += (*_row_sizes)[i];

		if (index.second >= (*_row_sizes)[index.first]) THROW_SARRAY_EXCEPTION("Error, column index points to a column outside of array bounds.");
		ind += index.second;

		return _data[ind];
	}
 };

 template <class T>
class s_array
{
public:

	typedef T									value_type;
	typedef value_type&							reference;
	typedef value_type*							pointer;
	typedef const value_type&					const_reference;
	typedef s_array_iterator<T>					iterator;
	typedef const s_array_iterator<T>			const_iterator;
	typedef int									difference_type;
	typedef uint								size_type;
	typedef const uint&							const_size_type;
	typedef pair<size_type,size_type>			index_type;
	typedef const  pair<size_type,size_type>&	const_index_type;
	typedef const std::vector<size_type>&			const_dimensional_type;
	typedef  std::vector<size_type>::iterator	dimensional_iterator;

	// Members added for STL compliance
	reference		at(const_index_type i){return _data[get_index(i)];}
	const_reference	at(const_index_type i) const {return _data[get_index(i)];}
	reference		front(){return _data[0];}
	const_reference	front() const {return _data[0];}
	reference		front(size_type row_index)
	{
		_cursor.first = row_index;
		_cursor.second = 0;
		return this->operator[](_cursor);
	}
	const_reference	front(size_type row_index) const 
	{
		_cursor.first = row_index;
		_cursor.second = 0;
		return this->operator[](_cursor);
	}
	reference		back(){return _data[_size-1];}
	const_reference	back() const {return _data[_size-1];}
	reference		back(size_type row_index)
	{
		_cursor.first = row_index;
		_cursor.second = _row_sizes[_cursor.first]-1;
		return this->operator[](_cursor);
	}
	const_reference	back(size_type row_index) const 
	{
		_cursor.first = row_index;
		_cursor.second = _row_sizes[_cursor.first]-1;
		return this->operator[](_cursor);
	}
	iterator		begin()
    {
		_cursor.first = 0;
		_cursor.second = 0;
		return iterator(this->_data, &_row_sizes, &_cursor);
    }
	iterator		begin(size_type row_index)
    {
		_cursor.first = 0;
		_cursor.second = 0;
		return iterator(this->_data, &_row_sizes, &_cursor, row_index);
    }
	iterator		end() {return iterator();}
	bool			empty(){ return (_size==0);}
	void			clear(){_cleanup();}
	void			resize(const_dimensional_type new_row_sizes)
	{
		this->resize(new_row_sizes,0);
	}
	void			resize(const_dimensional_type new_row_sizes, value_type value)
	{
		if (new_row_sizes.size() == 0) return;
		if (new_row_sizes.size() == 1 && new_row_sizes[0]==0) return;

		s_array<T> tmp = s_array<T>(*this);
		this->_cleanup();
		//this->_init(new_row_sizes);
		*this = s_array<T>(new_row_sizes, value);

		//cout<<", new data_ptr="<<this->_data<<", this_ptr="<<this;

		//iterator itr = this->begin();

		//cout<<", new data_ptr="<<this->_data<<", this_ptr="<<this;

		for (size_type i = 0; i < tmp.num_dimensions(); ++i)
		{
			for (size_type j=0; j < tmp.dimensions()[i]; ++j)
			{
				this->operator()(i,j) = tmp(i,j);
			}
		}

		/*for (itr; itr != this->end(); ++itr)
		{
			const_index_type index = itr.get_index();
			if (tmp.valid_index(index))
			{
				size_type i = tmp.get_index(index);
				(*itr) = tmp._data[i];
			}
			else (*itr) = value;		
		}*/
	}

	// MArray constructors/destructor
	s_array (void){_size = 0;_data=nullptr;_row_sizes.push_back(0);}
	s_array (const_dimensional_type row_sizes);
	s_array (const_dimensional_type row_sizes, T init_val);
	s_array (const s_array& obj);
	void Init(const_dimensional_type row_sizes);
	void Copy(const s_array& obj);
	s_array& operator=(const s_array& obj);
	~s_array (void);


	// MArray operators overloads for data acces
	reference operator[](const_index_type index) // get data at given index
	{
		size_type i = get_index(index);
		return _data[i];
	} 
	const_reference operator[](const_index_type index) const // get data at given index
	{
		size_type i = get_index(index);
		return _data[i];
	} 
	reference operator()(const_size_type row_index, const_size_type col_index)
	{
		size_type i = get_index(pair<size_type,size_type>(row_index,col_index));
		return _data[i];
	} 
	const_reference operator()(const_size_type row_index, const_size_type col_index) const // get data at given index
	{
		size_type i = get_index(pair<size_type,size_type>(row_index,col_index));
		return _data[i];
	} 
	reference operator()(const_size_type index)
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 
	const_reference operator()(const_size_type index) const // get data at given index
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 

	// Property access members
	const_size_type size() {return _size;}
	const_size_type size(size_type row_index) {return _row_sizes[row_index];}
	const_dimensional_type dimensions(){return _row_sizes;}
	size_type num_dimensions(){return _row_sizes.size();}

	// display member
	void print(ostream& stream);
	void print_address()
	{
		cout <<"ADDR: "<<_data<<endl;
	}

protected:
	std::vector<size_type> _row_sizes;
	index_type _cursor;
	size_type _size;
	pointer _data;

	void _init(const_dimensional_type row_sizes);
	void _copy(const s_array& obj);
	void _cleanup()
	{
		if (_size > 0) delete[] _data;
		_size=0;
		_row_sizes.clear();
		_cursor.first=0; _cursor.second=0;
	}

	uint get_index(const_index_type index)
	{
		uint ind=0;

		if (index.first >= _row_sizes.size()) THROW_SARRAY_EXCEPTION("Error, row index points to a row outside of array bounds.");

		for (uint i = 0; i< index.first; i++) ind += _row_sizes[i];

		if (index.second >= _row_sizes[index.first]) THROW_SARRAY_EXCEPTION("Error, column index points to a column outside of array bounds.");
		ind += index.second;

		return ind;
	}
	bool valid_index(const_index_type index)
	{
		if (index.first >= _row_sizes.size()) return false;
		if (index.second >= _row_sizes[index.first]) return false;
		return true;
	}
	void _cursor_start()
	{
		_cursor.first = 0;
		_cursor.second = 0;
	}
	void _cursor_end()
	{
		_cursor.first = _row_sizes.size()-1;
		_cursor.second = _row_sizes[_cursor.first];
	}
};

// Multi-dim Array Constructors, copiers, assignment, etc.
template <class T>
s_array<T>::s_array(const_dimensional_type row_sizes)
{
	_init(row_sizes);
}
template <class T>
s_array<T>::s_array(const_dimensional_type row_sizes, T init_val)
{
	_init(row_sizes);
	for (size_type i=0; i<_size; i++) _data[i]=init_val;
}
template <class T>
s_array<T>::s_array(const s_array<T>& obj)
{
	_copy(obj);
}
template <class T>
void s_array<T>::Copy(const s_array<T>& obj)
{
	if (this==&obj) return;
	_cleanup();
	_copy(obj);
}
template <class T>
void s_array<T>::_copy(const s_array<T>& obj)
{
	_row_sizes.clear();
	_cursor.first = 0;
	_cursor.second = 0;
	_size = obj._size;

	if (obj._size == 0) return;

	for (size_type i=0; i<obj._row_sizes.size(); i++)
	{
		_row_sizes.push_back(obj._row_sizes[i]);
	}

	_data = new T[_size];

	for (size_type i=0; i<_size; i++) _data[i] = obj._data[i];
}
template <class T>
void s_array<T>::Init(const_dimensional_type row_sizes)
{
	_cleanup();
	_init(row_sizes);
}
template <class T>
void s_array<T>::_init(const_dimensional_type row_sizes)
{
	_size = 0;
	_cursor.first = 0;
	_cursor.second = 0;

	for (size_type i=0; i<row_sizes.size(); i++)
	{
		if (row_sizes[i] <= 0) THROW_SARRAY_EXCEPTION("ERROR: cannot have a dimensions with size less than or equal to 0.");
		_row_sizes.push_back(row_sizes[i]);
		_size +=row_sizes[i];
	}

	_data = new value_type[_size];
}
template <class T>
s_array<T>& s_array<T>::operator=(const s_array<T>& obj)
{
	if (this != &obj)
	{
		_cleanup();
		_copy(obj);
	}
	return *this;
}


// M_array destructor
template <class T>
s_array<T>::~s_array(void)
{
	_cleanup();
}

// display member functions
template <class T>
void s_array<T>::print(ostream& stream)
{
	if (this->_size == 0) return;

	this->begin();
	// print 2d matrix of last 2 dimensions
	for (uint i=0; i<_row_sizes.size(); i++)
	{
		for (uint j=0; j<_row_sizes[i]; j++)
		{
			T& val = this->operator[](index_type(i,j));
			if ((float)val < FLT_MIN) val = 0.0;
			stream<<val<<"\t";
		}
		stream<<endl;
	}
	
}



