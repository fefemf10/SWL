#pragma once
#include <vector>
#include "Vector.hpp"
template <typename T>
class Matrix
{
public:
	Matrix<T>(size_t row, size_t column);
	Matrix<T>(size_t row, size_t column, const T& value);
	Matrix<T>(const Vector<T>& vector, bool transpose);
	Matrix<T> transpose();
	Matrix<T>& operator =(const T& value);
	Matrix<T>& operator =(const Matrix<T>& other) = default;
	Matrix<T> operator *(const Matrix<T>& other) const;
	Matrix<T> operator +(const Matrix<T>& other) const;
	//Vector<T> operator *(const Vector<T>& other) const;
	template <typename Q>
	friend Vector<Q> operator *(const Matrix<Q> first, const Vector<Q>& second);
	template <typename Q>
	friend Vector<Q> operator *(const Vector<Q>& first, const Matrix<Q> second);
	Matrix<T> operator *(const T& other) const;
	T& operator [](const size_t index);
	const T& operator [](const size_t index) const;
	std::vector<T> mat;
	size_t row;
	size_t column;
};

template<typename T>
inline Matrix<T>::Matrix(size_t row, size_t column) : mat(row* column), row(row), column(column)
{
}

template<typename T>
inline Matrix<T>::Matrix(size_t row, size_t column, const T& value) : mat(row* column, value), row(row), column(column)
{
}

template<typename T>
inline Matrix<T>::Matrix(const Vector<T>& vector, bool transpose) : mat(vector.size), row(transpose ? 1 : vector.size), column(transpose ? vector.size : 1)
{

}

template<typename T>
inline Matrix<T> Matrix<T>::transpose()
{
	Matrix<T> m(column, row);
	for (size_t i = 0; i < row; ++i)
	{
		for (size_t j = 0; j < column; ++j)
		{
			m[i * row + j] = mat[j * column + i];
		}
	}
	return m;
}

template<typename T>
inline Matrix<T>& Matrix<T>::operator=(const T& value)
{
	mat.clear();
	mat.resize(row * column, value);
	return *this;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
	if (this->column != other.row)
		return *this;

	Matrix<T> c(this->row, other.column);
#pragma omp parallel
	for (long long i = 0; i < this->row; ++i)
	{
		T* rowc = &c[i * other.column];
		for (size_t k = 0; k < this->column; ++k)
		{
			float a = mat[i * this->column + k];
			const T* rowb = &other[k * other.column];
			for (size_t j = 0; j < other.column; ++j)
			{
				rowc[j] += a * rowb[j];
			}
		}
	}
	return c;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
{
	Matrix c(this->row, this->column);
#pragma omp parallel
	for (long long i = 0; i < this->row; ++i)
	{
		const T* rowa = &mat[i * this->column];
		const T* rowb = &other[i * this->column];
		T* rowc = &c[i * this->column];
		for (size_t j = 0; j < this->column; ++j)
		{
			rowc[j] = rowa[j] + rowb[j];
		}
	}
	return c;
}

//template<typename T>
//inline Vector<T> Matrix<T>::operator*(const Vector<T>& other) const
//{
//	Vector<T> c(this->column);
//	if (this->column == other.size)
//		return c;
//	for (size_t i = 0; i < this->row; ++i)
//	{
//		const T* rowa = &mat[i * this->column];
//		float tmp{};
//		for (size_t j = 0; j < this->column; ++j)
//		{
//			tmp += rowa[j] * other[j];
//		}
//		c[i] = tmp;
//	}
//	return c;
//}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const T& other) const
{
	Matrix<T> c(this->row, this->column);
#pragma omp parallel
	for (long long i = 0; i < this->row; ++i)
	{
		const T* rowa = &mat[i * this->column];
		T* rowc = &c[i * this->column];
		for (size_t j = 0; j < this->column; ++j)
		{
			rowc[j] = rowa[j] * other;
		}
	}
	return c;
}

template<typename T>
inline T& Matrix<T>::operator[](const size_t index)
{
	return mat[index];
}

template<typename T>
inline const T& Matrix<T>::operator[](const size_t index) const
{
	return mat[index];
}

template <typename Q>
inline Vector<Q> operator*(const Matrix<Q> first, const Vector<Q>& second)
{
	Vector<Q> c(first.row);
	if (first.column != second.size)
		return c;
#pragma omp parallel
	for (long long i = 0; i < first.row; ++i)
	{
		const Q* rowa = &first[i * first.column];
		for (size_t j = 0; j < first.column; ++j)
		{
			c[i] += rowa[j] * second[j];
		}
	}
	return c;
}

template <typename Q>
inline Vector<Q> operator*(const Vector<Q>& first, const Matrix<Q> second)
{
	Vector<Q> c(second.column);
	if (second.row != first.size)
		return c;
#pragma omp parallel
	for (long long j = 0; j < second.column; ++j)
	{
		for (size_t i = 0; i < second.row; ++i)
		{
			const Q rowa = second[j * second.row + i];
			c[j] += rowa * first[i];
		}
	}
	return c;
}
