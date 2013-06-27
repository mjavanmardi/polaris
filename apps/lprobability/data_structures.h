#pragma once
#include <vector>
#include <unordered_map>

namespace polaris
{
template <typename T>
class SparseMatrix
{
public:
	SparseMatrix() {sum = 0;}
	SparseMatrix (unsigned int n_, unsigned int m_) : n(n_), m(m_) {sum = 0;}
	SparseMatrix (unsigned int n_) : n(n_), m(n_) {sum = 0;}
	int sum;
	inline
	T& operator()(unsigned int i, unsigned int j)
	{
		return mat[i][j];
	}
	T operator()(unsigned int i, unsigned int j) const
	{
		if (mat.find(i) == mat.end())
		{
			return 0;
		}
		else if ((mat.at(i)).find(j) == mat.at(i).end())
		{
			return 0;
		}
		else
		{
			return mat.at(i).at(j);
		}
	}
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, T> >& M()
	{
		return mat;
	}
	std::unordered_map<unsigned int, T>& operator() (unsigned int i)
	{
		return mat[i];
	}
	int size()
	{
		return mat.size();
	}
private:
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, T> > mat;
	unsigned int n,m;	
};
template <typename T>
class ArrayMatrix
{
public:
		~ArrayMatrix()
		{
			delete [] mat;
		}
		ArrayMatrix (unsigned int n_, unsigned int m_) : n(n_), m(m_)
		{
			mat = new T[n*m];
		}
		inline
		T& operator()(unsigned int i, unsigned int j)
		{
			return mat[m*j + i];
		}
private:
	T* mat;
	unsigned int n,m;
};
template <typename T>
class VectorMatrix
{
public:
		VectorMatrix (unsigned int n_, unsigned int m_) : n(n_), m(m_)
		{
			mat.resize(n*m);
		}
		inline
		T& operator()(unsigned int i, unsigned int j)
		{
			return mat[m*j + i];
		}
private:
	std::vector<T> mat;
	unsigned int n,m;
};
typedef SparseMatrix<int> Matrix;
}