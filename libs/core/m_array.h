#pragma once

#include "boost/container/vector.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>

#define THROW_MATRIX_EXCEPTION(...) \
			{stringstream s;\
			s << "\nRUNTIME_ERROR: " << __FILE__ << " at " << __LINE__<< "\n\t" << __VA_ARGS__ << endl;\
            cerr << s.str();\
			throw new runtime_error(s.str().c_str());}

typedef unsigned int uint;

using namespace std;

//-------------------------------------------------------------
//	MULTIDIMENSIONAL ARRAY CLASS WITH INDETERMINATE DIMENSIONS
//-------------------------------------------------------------
template<class T>
class m_array_iterator
{

public:
       typedef std::random_access_iterator_tag	iterator_category;
       typedef T								value_type;
       typedef value_type &						reference;
       typedef value_type   *					pointer;
       typedef int								difference_type;
	   typedef uint								size_type;

      m_array_iterator()
       {      // default constructor
		   _fixed_dim = -1;
		   _fixed_dim_val = -1;
		   m_Ptr = NULL;
       }

      m_array_iterator(T* data, std::vector<size_type>* dim_sizes, std::vector<size_type>* cursor)
       {	// copy constructor
			_fixed_dim = -1;
			_fixed_dim_val = -1;
			_dimensions = (std::vector<size_type>*)dim_sizes;
			for (size_type i = 0; i< cursor->size(); i++) _cursor.push_back((*cursor)[i]);
			m_Ptr = &data[get_index(_cursor)];
			_data = data;
       }

	  m_array_iterator(T* data, std::vector<size_type>* dim_sizes, std::vector<size_type>* cursor, size_type fixed_dimension_index, size_type fixed_dimension_value)
       {	// copy constructor
			_fixed_dim = fixed_dimension_index;
			_fixed_dim_val = fixed_dimension_value;
			_dimensions = dim_sizes;
			for (size_type i = 0; i< cursor->size(); i++) _cursor.push_back((*cursor)[i]);
			_cursor[_fixed_dim] = _fixed_dim_val;
			m_Ptr = &data[get_index(_cursor)];
			_data = data;
       }

      reference operator*() const
       {  
		   // return designated object
           return (*m_Ptr);
       }

	   m_array_iterator& operator=(const m_array_iterator& obj)
       {      
              m_Ptr = obj.m_Ptr;
			  _dimensions = obj._dimensions;
			  _cursor = obj._cursor;
			  _data = obj._data;
			  _fixed_dim = obj._fixed_dim;
			  _fixed_dim_val = obj._fixed_dim_val;

             return (*this);
       }

       m_array_iterator& operator++()
       {    
			for (size_type d = (size_type)_dimensions->size()-1; d >= 0; d--)
			{
				if (d != _fixed_dim)
				{
					++(_cursor)[d];

					if ((_cursor)[d] >= (*_dimensions)[d])
					{
						(_cursor)[d] = 0;

						if (d == 0 || (d == 1 && _fixed_dim == 0))
						{
							m_Ptr = NULL;
							return (*this);
						}
					}
					else break;
				}
			}
			m_Ptr = &_data[get_index(_cursor)];

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

	const std::vector<size_type>& get_index()
	{
		return _cursor;
	}

private:
	T *m_Ptr;                                            // pointer to container value type
	std::vector<size_type>* _dimensions;								// dimensional sizes 
	std::vector<size_type> _cursor;
	T * _data;
	int _fixed_dim;
	int _fixed_dim_val;
	uint get_index(std::vector<size_type>& index)
	{
		uint ind=0;

		if (index.size() != _dimensions->size()) THROW_MATRIX_EXCEPTION("Error, incorrect number of dimensions in index.");

		for (int i = 0; i< index.size(); i++)
		{
			if (index[i] >= (*_dimensions)[i]) THROW_MATRIX_EXCEPTION("Error, index outside of array bounds for dimension: " << i);

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
class m_array
{
public:
	typedef T							value_type;
	typedef value_type&					reference;
	typedef value_type*					pointer;
	typedef const value_type&			const_reference;
	typedef m_array_iterator<T>			iterator;
	typedef const m_array_iterator<T>	const_iterator;
	typedef int							difference_type;
	typedef uint						size_type;
	typedef const uint					const_size_type;
	typedef std::vector<size_type>			index_type;
	typedef const std::vector<size_type>&	const_index_type;
	typedef const std::vector<size_type>&	const_dimensional_type;
	typedef const std::vector<size_type>::iterator	dimensional_iterator;

	// Members added for STL compliance
	reference		at(const_index_type i){return _data[get_index(i)];}
	const_reference	at(const_index_type i) const {return _data[get_index(i)];}
	reference		front(){return _data[0];}
	const_reference	front() const {return _data[0];}
	reference		front(size_type fixed_dimension_index, size_type fixed_dimension_value)
	{
		_cursor_start();
		_cursor[fixed_dimension_index] = fixed_dimension_value;
		return this->operator[](_cursor);
	}
	const_reference	front(size_type fixed_dimension_index, size_type fixed_dimension_value) const 
	{
		_cursor_start();
		_cursor[fixed_dimension_index] = fixed_dimension_value;
		return this->operator[](_cursor);
	}
	reference		back(){return _data[_size-1];}
	const_reference	back() const {return _data[_size-1];}
	reference		back(size_type fixed_dimension_index, size_type fixed_dimension_value)
	{
		_cursor_end();
		_cursor[fixed_dimension_index] = fixed_dimension_value;
		return this->operator[](_cursor);
	}
	const_reference	back(size_type fixed_dimension_index, size_type fixed_dimension_value) const 
	{
		_cursor_end();
		_cursor[fixed_dimension_index] = fixed_dimension_value;
		return this->operator[](_cursor);
	}
	iterator		begin()
    {
		for (size_type i = 0; i< _dim_sizes.size(); i++) _cursor[i] = 0;
		return iterator(this->_data, &_dim_sizes, &_cursor);
    }
	iterator		begin(size_type fixed_dimension_index, size_type fixed_dimension_value)
    {
		for (size_type i = 0; i< _dim_sizes.size(); i++) _cursor[i] = 0;
		return iterator(this->_data, &_dim_sizes, &_cursor, fixed_dimension_index, fixed_dimension_value);
    }
	iterator		end() {return iterator();}
	bool			empty(){ return (_size==0);}
	void			clear(){_cleanup();}
	void			resize(std::vector<size_type> new_dimensions)
	{
		if (new_dimensions.size() == 0) return;

		m_array<T> tmp = m_array<T>(*this);
		this->_cleanup();
		this->_init(new_dimensions);
		*this = m_array<T>(new_dimensions);

		iterator itr = this->begin();

		for (itr; itr != this->end(); ++itr)
		{
			const std::vector<size_type>& index = itr.get_index();
			if (tmp.valid_index(index))
			{
				size_type i = tmp.get_index(index);
				(*itr) = tmp._data[i];
			}
			else (*itr) = 0;
			
		}
	}
	void			resize(std::vector<size_type> new_dimensions, value_type value)
	{
		if (new_dimensions.size() == 0) return;

		m_array<T> tmp = m_array<T>(*this);
		this->_cleanup();
		this->_init(new_dimensions);
		*this = m_array<T>(new_dimensions, value);

		iterator itr = this->begin();

		for (itr; itr != this->end(); ++itr)
		{
			const index_type index = (index_type)itr.get_index();
			if (tmp.valid_index(index))
			{
				size_type i = tmp.get_index(index);
				(*itr) = tmp._data[i];
			}
			else
			{
				(*itr) = value;
			}
			
		}
	}

	// MArray constructors/destructor
	m_array (void);
	m_array (const_index_type dim_sizes);
	m_array (const_index_type dim_sizes, T init_val);
	m_array (const m_array& obj);
	void Init(const_index_type dim_sizes);
	void Copy(const m_array& obj);
	m_array& operator=(const m_array& obj);
	~m_array (void);


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
	reference operator[](size_type index) // get data at given index
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 
	const_reference operator[](const_size_type index) const // get data at given index
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 
	
	// Property access members
	const_size_type size() {return _size;}
	const_size_type size(size_type dimension) {return _dim_sizes[dimension];}
	const_index_type dimensions(){return _dim_sizes;}
	size_type num_dimensions() {return _dim_sizes.size();}
	value_type sum()
	{
		value_type _sum = 0;
		for (size_type i=0; i<_size; i++)
		{
			_sum += _data[i];
		}
		return _sum;
	}
	value_type sum(size_type dimension, size_type index)
	{
		value_type _sum = 0;

		for (m_array<T>::iterator itr = this->begin(dimension,index); itr != this->end(); ++itr)
		{
			_sum += *itr;
		}
		return _sum;
	}

	// display member
	void print(ostream& stream);
	void scale(const T&);

	// Arithmetic members
	m_array& operator+(const m_array& obj);
	m_array& operator-(const m_array& obj);
	m_array& operator*(const m_array& obj);
	m_array& operator/(const m_array& obj);

	size_type get_index(const_index_type index)
	{
		size_type ind=0;

		if (index.size() != _dim_sizes.size())
		{
			THROW_MATRIX_EXCEPTION("Error, incorrect number of dimensions in index.");
		}
		
		for (size_type i = 0; i< index.size(); i++)
		{
			if (index[i] >= _dim_sizes[i]) 
			{
				THROW_MATRIX_EXCEPTION("Error, index outside of array bounds for dimension: " << i);
			}
			size_type multiplier = 1;
			for (size_type j=i+1; j< index.size(); j++)
			{
				multiplier = multiplier * _dim_sizes[j];
			}

			ind += index[i] * multiplier;
		}
		return ind;
	}
	index_type get_index(size_type index)
	{
		index_type ind;

		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, 1D index outside of array bounds, can not convert to index vector.");
		}
		size_type remain = index;
		for (size_type i = 0; i< _dim_sizes.size(); i++)
		{
			size_type product = 1;
			for (size_type j = i+1; j< _dim_sizes.size(); j++)
			{
				product *= _dim_sizes[j];
			}
			ind.push_back(remain / product);
			remain = remain % product;
		
		}
		return ind;
	}

	pointer get_data_pointer()
	{
		return _data;
	}
protected:
	index_type _dim_sizes;
	index_type _cursor;
	size_type _size;
	pointer _data;

	void _init(const_index_type dim_sizes);
	void _copy(const m_array& obj);
	void _cleanup()
	{
		//cout <<endl<< "cleanup 1, ";
		if (_size > 0) delete[] _data;
		_size=0;
		//cout << "cleanup 2, ";
		if (_dim_sizes.size() > 0) _dim_sizes.clear();
		//cout << "cleanup 3, ";
		if (_cursor.size() > 0) _cursor.clear();
		//cout << "cleanup done"<<endl;
	}


	bool valid_index(const_index_type index)
	{
		if (index.size() != _dim_sizes.size())
		{
			return false;
		}
		for (size_type i = 0; i< index.size(); i++)
		{
			if (index[i] >= _dim_sizes[i]) return false;
		}
		return true;
	}
	void _cursor_start()
	{
		for (int i=0; i<_cursor.size(); i++) _cursor[i] = 0;
	}
	void _cursor_end()
	{
		for (int i=0; i<_cursor.size(); i++) _cursor[i] = _dim_sizes[i]-1;
	}

	void print(ostream& stream, int n);

};

// Multi-dim Array Constructors, copiers, assignment, etc.
template <class T>
m_array<T>::m_array (void)
{
	_dim_sizes.clear();
	_cursor.clear();
	_size=0;
	_data=nullptr;
	//index_type dims;
	//dims.push_back(1);
	//_init(dims);
}
template <class T>
m_array<T>::m_array(const_index_type dim_sizes)
{
	_init(dim_sizes);
}
template <class T>
m_array<T>::m_array(const_index_type dim_sizes, T init_val)
{
	_init(dim_sizes);
	for (size_type i=0; i<_size; i++)
	{
		_data[i]=init_val;
	}
}
template <class T>
m_array<T>::m_array(const m_array<T>& obj)
{
	_copy(obj);
}
template <class T>
void m_array<T>::Copy(const m_array<T>& obj)
{
	//cout <<"Copy .... this, obj: " << (long long*)this->_data << ", " << (long long*)&obj._data<<endl;
	if (this==&obj) return;
	_cleanup();
	_copy(obj);
}
template <class T>
void m_array<T>::_copy(const m_array<T>& obj)
{
	//cout <<endl<< "copy 1, ";
	_dim_sizes.clear();
	_cursor.clear();
	_size = obj._size;

	//cout << "copy 2, ";

	for (size_type i=0; i<obj._dim_sizes.size(); i++)
	{
		_dim_sizes.push_back(obj._dim_sizes[i]);
		_cursor.push_back(0);
	}
	//cout << "copy 3, ";

	if (_size>0) _data = new T[_size];
	else _data = nullptr;

	for (size_type i=0; i<_size; i++) _data[i] = obj._data[i];

	//cout << "copy done."<<endl;
}
template <class T>
void m_array<T>::Init(const_index_type dim_sizes)
{
	_cleanup();
	_init(dim_sizes);
}
template <class T>
void m_array<T>::_init(const_index_type dim_sizes)
{
	//_ndim = (size_type)dim_sizes.size();
	_size = 1;

	for (size_type i=0; i<dim_sizes.size(); i++)
	{
		_dim_sizes.push_back(dim_sizes[i]);
		_cursor.push_back(0);
		_size = _size*dim_sizes[i];
	}

	_data = new value_type[_size];
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

// arithmetic functions
template <class T>
m_array<T>& m_array<T>::operator+(const m_array<T>& obj)
{
	if (_size != obj._size)
	{
		THROW_MATRIX_EXCEPTION("Error, cannot add two arrays of different sizes.");
	}
	else
	{
		for (int i=0; i<(int)_size; i++) _data[i]+=obj._data[i];
	}
	return *this;
}
template <class T>
m_array<T>& m_array<T>::operator-(const m_array<T>& obj)
{
	if (_size != &obj.size())
	{
		THROW_MATRIX_EXCEPTION("Error, cannot add two arrays of different sizes.");
	}
	else
	{
		for (int i=0; i<_size; i++) _data[i]-=obj._data[i];
	}
	return *this;
}


template <class T>
void m_array<T>::scale(const T& value)
{
	for (size_type i=0; i<_size; i++) _data[i] *= value;
}

// M_array destructor
template <class T>
m_array<T>::~m_array(void)
{
	_cleanup();
}

// display member functions
template <class T>
void m_array<T>::print(ostream& stream)
{
	if (this->_size == 0) return;
	this->begin();
	print(stream, 0);
}
template <class T>
void m_array<T>::print(ostream& stream, int n)
{
	if (n == _dim_sizes.size()-2)
	{	
		// print header for higher dimensions
		if(_dim_sizes.size()>2)
		{
			stream<<"Higher Dimensions: ";
			for (uint k=0; k<_dim_sizes.size()-2; k++) stream<<"D"<<k<<"="<<_cursor[k]<<",";
			stream<<endl;
		}

		// print header for columns (last dimension in matrix)
		stream<<"\t";
		for (uint k=0; k<_dim_sizes[n+1]; k++) 
		{
			stream<</*setw(6)<<*/"D"<<n+1;
			//cout<<n+1;
			stream<<"=";
			stream<<k;
			stream<<"\t";
		}
		stream<<endl;

		// print 2d matrix of last 2 dimensions
		for (uint i=0; i<_dim_sizes[n]; i++)
		{
			_cursor[n] = i;
			stream<<"D"<<n<<"="<<i<<"\t";
			for (uint j=0; j<_dim_sizes[n+1]; j++)
			{
				_cursor[n+1]=j;

				stream<</*setw(10)<<*/this->operator[](this->_cursor)<<"\t";
			}
			stream<<endl;
		}
		stream<<endl<<endl;
		return;
	}

	for (uint i=0; i<_dim_sizes[n]; i++)
	{
		_cursor[n] = i;
		print(stream, n+1);
	}
}




//-------------------------------------------------------------
//	MATRIX CLASS WITH TWO DIMENSIONS
//-------------------------------------------------------------
template<class T>
class matrix_iterator
{

public:
       typedef std::random_access_iterator_tag	iterator_category;
       typedef T								value_type;
       typedef value_type &						reference;
       typedef value_type *						pointer;
       typedef int								difference_type;
	   typedef uint								size_type;

      matrix_iterator()
       {      // default constructor
		   m_Ptr = NULL;
       }

      matrix_iterator(T* data, size_type index)
       {
			m_Ptr = &data[index];
			_data = data;
       }

	   reference operator*() const
       {  
		   // return designated object
           return (*m_Ptr);
       }

	   matrix_iterator& operator=(const matrix_iterator& obj)
       {      
              m_Ptr = obj.m_Ptr;
			  _data = obj._data;
             return (*this);
       }

       matrix_iterator& operator++()
       {   
		   m_Ptr++;
			return (*this);
       }

	   matrix_iterator& operator--()
       {      // predecrement
              --m_Ptr;
             return (*this);
       }

      matrix_iterator operator++(int)
       {      // postincrement
              m_array_iterator<T> _Tmp = *this;
              ++*this;
              return (_Tmp);
       }

      matrix_iterator operator--(int)

      {      // postdecrement
              m_array_iterator<T> _Tmp = *this;
              --*this;
              return (_Tmp);
       }

      bool operator==(const matrix_iterator &_Right) const
       {      // test for iterator equality
              return (m_Ptr == _Right.m_Ptr);
       }

      bool operator!=(const matrix_iterator &_Right) const
       {      // test for iterator inequality
              return (!(m_Ptr == _Right.m_Ptr));
       }

private:
	T *m_Ptr;                                            // pointer to container value type
	T * _data;
 };

 template <class T>
class matrix
{
public:

	typedef T							value_type;
	typedef value_type&					reference;
	typedef value_type*					pointer;
	typedef const value_type&			const_reference;
	typedef matrix_iterator<T>			iterator;
	typedef const matrix_iterator<T>	const_iterator;
	typedef int							difference_type;
	typedef uint						size_type;
	typedef const uint					const_size_type;
	typedef pair<size_type,size_type>	index_type;
	typedef const index_type&			const_index_type;
	typedef const index_type&			const_dimensional_type;
	typedef const std::vector<size_type>::iterator	dimensional_iterator;

	// Members added for STL compliance
	reference		at(const_index_type i){return _data[i];}
	const_reference	at(const_index_type i) const {return _data[i];}
	reference		front(){return _data[0];}
	const_reference	front() const {return _data[0];}
	reference		back(){return _data[_size-1];}
	iterator		begin()
    {
		_cursor_start();
		return iterator(this->_data, 0);
    }
	iterator		end() {return iterator();}
	bool			empty(){ return (_size==0);}
	void			clear(){_cleanup();}
	void			resize(size_type rows, size_type cols, value_type value)
	{ 
		pair<size_type,size_type> new_dimensions = pair<size_type,size_type>(rows,cols);
		matrix<T> tmp = matrix<T>(*this);
		this->_cleanup();
		//this->_init(new_dimensions);
		*this = matrix<T>(new_dimensions, value);
//		this->_nrow = rows; this->_ncol = cols;

		iterator itr = this->begin();

		for (uint i = 0; i < tmp.num_rows(); ++i)
		{
			for (uint j = 0; j < tmp.num_cols(); ++j)
			{
				this->operator()(i,j) = tmp(i,j);
			}
		}
	}
	void			resize(index_type new_dimensions, value_type value)
	{
		matrix<T> tmp = matrix<T>(*this);
		this->_cleanup();
		*this = matrix<T>(new_dimensions, value);
		for (size_type i=0; i<_size; i++) _data[i]=value;
	}


	// MArray constructors/destructor
	matrix (void){_size = 0;_dim_sizes.first=0; _dim_sizes.second=0; _data=nullptr;}
	matrix (const_index_type dim_sizes);
	matrix (const_index_type dim_sizes, T init_val);
	matrix (const matrix& obj);
	void Init(const_index_type dim_sizes);
	void Copy(const matrix& obj);
	void Copy(const_index_type dim_sizes, T* mem_to_copy);
	matrix& operator=(const matrix& obj);
	~matrix (void);


	// MArray operators overloads for data acces
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
	reference operator[](size_type index) // get data at given index
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 
	const_reference operator[](const_size_type index) const // get data at given index
	{
		if (index >= _size)
		{
			THROW_MATRIX_EXCEPTION("Error, index '"<< index <<"' outside of array bounds (size="<< _size <<").");
		}
		return _data[index];
	} 
	pointer get_data_pointer(){return _data;}
	
	// Property access members
	const size_type& size() const {return _size;}
	const size_type& size(size_type dimension) const 
	{
		if (dimension==0) return _dim_sizes.first;
		if (dimension==1) return _dim_sizes.second;
		THROW_MATRIX_EXCEPTION("Error, can not request dimension size for dimension higher than 1");
	}
	const_index_type dimensions() const {return _dim_sizes;}
	const size_type& num_dimensions() {return 2;}
	const size_type& num_rows() const {return _dim_sizes.first;}
	const size_type& num_cols() const {return _dim_sizes.second;}

	// display member
	void print(ostream& stream);

	// Arithmetic members
	void cholesky(matrix& LU);
	matrix operator*(const matrix& obj);
	matrix& operator*(const T& value);
	void scale(const T& value);
	void transpose();

	size_type get_index(const_index_type index) const
	{
		if (index.first >= _dim_sizes.first || index.second >= _dim_sizes.second) THROW_MATRIX_EXCEPTION("Error, index ("<<index.first<<","<<index.second<<") was outside of matrix bounds {"<<_dim_sizes.first<<","<<_dim_sizes.second<<"}" << endl);
		return index.first *_dim_sizes.second + index.second;
	}

protected:
	index_type _dim_sizes;
	index_type _cursor;
	size_type _size;
	pointer _data;

	void _init(const_index_type dim_sizes);
	void _copy(const matrix& obj);
	void _cleanup()
	{
		if (_size > 0) delete[] _data;
		_size=0;
		_dim_sizes.first=0; _dim_sizes.second=0;
		_cursor_start();
	}


	bool valid_index(const_index_type index)
	{
		if (index.first < _dim_sizes.first && index.second << _dim_sizes.second) return true;
		return false;
	}
	void _cursor_start()
	{
		_cursor.first = 0; _cursor.second = 0;
	}
	void _cursor_end()
	{
		_cursor.first = _dim_sizes.first-1;
		_cursor.second = _dim_sizes.second-1;
	}

	void print(ostream& stream, int n);

};

// Multi-dim Array Constructors, copiers, assignment, etc.
template <class T>
matrix<T>::matrix(const_index_type dim_sizes)
{
	_init(dim_sizes);
}
template <class T>
matrix<T>::matrix(const_index_type dim_sizes, T init_val)
{
	_init(dim_sizes);
	for (size_type i=0; i<_size; i++) _data[i]=init_val;
}
template <class T>
matrix<T>::matrix(const matrix<T>& obj)
{
	_copy(obj);
}
template <class T>
void matrix<T>::Copy(const matrix<T>& obj)
{
	if (this==&obj) return;
	_cleanup();
	_copy(obj);
}
template <class T>
void matrix<T>::Copy(const_index_type dim_sizes, T* mem_to_copy)
{
	_cleanup();
	_dim_sizes.first = dim_sizes.first; _dim_sizes.second = dim_sizes.second;
	_size = dim_sizes.first * dim_sizes.second;
	_data = mem_to_copy;
}
template <class T>
void matrix<T>::_copy(const matrix<T>& obj)
{
	this->_cursor_start();
	this->_dim_sizes.first = obj._dim_sizes.first; this->_dim_sizes.second=obj._dim_sizes.second;
	_size = obj._size;
	_data = new T[_size];
	for (size_type i=0; i<_size; i++) _data[i] = obj._data[i];
}
template <class T>
void matrix<T>::Init(const_index_type dim_sizes)
{
	_cleanup();
	_init(dim_sizes);
}
template <class T>
void matrix<T>::_init(const_index_type dim_sizes)
{
	_dim_sizes.first = dim_sizes.first; _dim_sizes.second = dim_sizes.second;
	_size = dim_sizes.first * dim_sizes.second;
	_data = new value_type[_size];
}
template <class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& obj)
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
matrix<T>::~matrix(void)
{
	_cleanup();
}

// display member functions
template <class T>
void matrix<T>::print(ostream& stream)
{
	this->begin();
	print(stream, 0);
}
template <class T>
void matrix<T>::print(ostream& stream, int n)
{
	stream<<endl;

	// print header
	for (uint j=0; j<_dim_sizes.second; j++)
	{
		stream << '\t' << j <<'|';
	}
	stream << endl;
	// print 2d matrix of last 2 dimensions
	for (uint i=0; i<_dim_sizes.first; i++)
	{
		stream << i <<'|'<<'\t';
		for (uint j=0; j<_dim_sizes.second; j++)
		{
			stream<<this->operator[](pair<size_type, size_type>(i,j))<<"\t";
		}
		stream<<endl;
	}
	stream<<endl<<endl;
}


// Arithmetic functions
template <class T>
matrix<T> matrix<T>::operator*(const matrix<T>& obj)
{
	// check appropriate conditions
	if (this->num_cols() != obj.num_rows()) THROW_MATRIX_EXCEPTION("ERROR: matrix rows != matrix columns in multiplication.");

	matrix<T> m = matrix<T>(matrix<T>::index_type(this->num_rows(), obj.num_cols()),0);

	for (uint i = 0; i < this->num_rows(); i++)
	{
		for (uint j = 0; j < obj.num_cols(); j++)
		{
			for (uint k = 0; k < this->num_cols(); k++) m(i,j) += (*this)(i,k) * obj(k,j);		
		}
	}
	return m;
}
template <class T>
matrix<T>& matrix<T>::operator*(const T& value)
{
	for (uint i = 0; i < this->_nrow; i++)
	{
		for (uint j = 0; j < this->_ncol; j++)
		{
			(*this)(i,j) = (*this)(i,j) * value;		
		}
	}
	return *this;
}
template <class T>
void matrix<T>::scale(const T& value)
{
	for (uint i = 0; i < this->num_rows(); i++)
	{
		for (uint j = 0; j < this->num_cols(); j++)
		{
			(*this)(i,j) = (*this)(i,j) * value;		
		}
	}
}
template <class T>
void matrix<T>::transpose()
{
	size_type ncol=_dim_sizes.second;
	_dim_sizes.second = _dim_sizes.first;
	_dim_sizes.first = ncol;
	
}
template <class T>
void matrix<T>::cholesky(matrix<T>& LU)
{
	assert(_dim_sizes.first == _dim_sizes.second);
	
	// Initialize the sqrt matrix
	LU.resize(_dim_sizes.first,_dim_sizes.second, 0.0);

	int d = this->_dim_sizes.first;
	double diff;

	for(int k=0; k<d; ++k)
	{
		double sum=0.;
		for(int p=0; p<k; ++p) sum += LU._data[k*d+p] * LU._data[k*d+p];

		diff = sqrt(this->_data[k*d+k]-sum);
		if (diff < 0) THROW_MATRIX_EXCEPTION("ERROR: matrix must be positive semi-definite to use cholesky decomposition.");
		LU._data[k*d+k] = diff;

		for(int i=k+1;i<d;++i)
		{
			double sum=0.;
			for(int p=0; p<k; ++p) sum += LU._data[i*d+p] * LU._data[k*d+p];

			diff = LU._data[k*d+k];
			if (diff == 0) THROW_MATRIX_EXCEPTION("ERROR: matrix must be positive semi-definite to use cholesky decomposition.");
			LU._data[i*d+k] = (this->_data[i*d+k]-sum) / diff;
		}
	}
}

