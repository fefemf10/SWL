#pragma once
#include <vector>
template <typename T>
class Vector
{
public:
	Vector<T>(size_t size);
	Vector<T>(size_t size, const T& value);
	Vector<T>(const std::vector<T>& values);
	Vector<T>& operator =(const T& value);
	Vector<T>& operator *(const T& value);
	Vector<T>& operator /(const T& value);
	Vector<T>& operator =(const std::vector<T>& values);
	Vector<T> operator -(const Vector<T>& other) const;
	Vector<T> operator +(const Vector<T>& other) const;
	Vector<T> operator *(const Vector<T>& other) const;
	Vector<T> operator /(const Vector<T>& other) const;
	T& operator [](const size_t index);
	const T& operator [](const size_t index) const;
	std::vector<T> vec;
	size_t size;
};

template<typename T>
inline Vector<T>::Vector(size_t size) : vec(size), size(size)
{
}

template<typename T>
inline Vector<T>::Vector(size_t size, const T& value) : vec(size, value), size(size)
{
}

template<typename T>
inline Vector<T>::Vector(const std::vector<T>& values) : vec(values), size(values.size())
{
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const T& value)
{
	vec.clear();
	vec.resize(size, value);
	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator*(const T& value)
{
#pragma omp parallel
	for (long long i = 0; i < size; ++i)
	{
		vec[i] *= value;
	}
	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator/(const T& value)
{
#pragma omp parallel
	for (long long i = 0; i < size; ++i)
	{
		vec[i] /= value;
	}
	return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const std::vector<T>& values)
{
	vec = values;
	size = values.size();
	return *this;
}

template<typename T>
inline Vector<T> Vector<T>::operator-(const Vector<T>& other) const
{
	if (this->size != other.size)
		return *this;
	Vector<T> a(this->size);
#pragma omp parallel
	for (long long i = 0; i < this->size; ++i)
	{
		a[i] = vec[i] - other[i];
	}
	return a;
}

template<typename T>
inline Vector<T> Vector<T>::operator+(const Vector<T>& other) const
{
	if (this->size != other.size)
		return *this;
	Vector<T> a(this->size);
#pragma omp parallel
	for (long long i = 0; i < this->size; ++i)
	{
		a[i] = vec[i] + other[i];
	}
	return a;
}

template<typename T>
inline Vector<T> Vector<T>::operator*(const Vector<T>& other) const
{
	if (this->size != other.size)
		return *this;
	Vector<T> a(this->size);
#pragma omp parallel
	for (long long i = 0; i < this->size; ++i)
	{
		a[i] = vec[i] * other[i];
	}
	return a;
}

template<typename T>
inline Vector<T> Vector<T>::operator/(const Vector<T>& other) const
{
	if (this->size != other.size)
		return *this;
	Vector<T> a(this->size);
#pragma omp parallel
	for (long long i = 0; i < this->size; ++i)
	{
		a[i] = vec[i] / other[i];
	}
	return a;
}

template<typename T>
inline T& Vector<T>::operator[](const size_t index)
{
	return vec[index];
}

template<typename T>
inline const T& Vector<T>::operator[](const size_t index) const
{
	return vec[index];
}
