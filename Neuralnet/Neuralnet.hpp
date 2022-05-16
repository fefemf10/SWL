#pragma once
#include <random>
#include <fstream>
#include "Neuron.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

template <typename T>
class Neuralnet
{
public:
	Neuralnet(std::vector<uint16_t> countNeurons);
	void fullReset();
	void setInput(const std::vector<T>& input);
	//void setWeights(uint8_t indexLayer, const std::vector<float>& weights);
	void setRandomWeights();
	void loadWeights(const std::string& filepath);
	void saveWeights(const std::string& filepath);
	void forwardFeed();
	void backPropagation(T learnRate = 0.1);
	void findError(const std::vector<T>& rightAnswer, T alpha = 1.0);
	//std::vector<std::vector<Neuron>> neurons; //[layers][neuron]
	std::vector<Matrix<T>> weights;
	std::vector<Vector<T>> values;
	std::vector<Vector<T>> errors;
	float error;
	uint8_t countLayers;
private:
	T activation(T value);
	Vector<T> activation(const Vector<T>& values);
	T derivative(T value);
	Vector<T> derivative(const Vector<T>& values);
};

template<typename T>
inline Neuralnet<T>::Neuralnet(std::vector<uint16_t> countNeurons) : countLayers(static_cast<uint8_t>(countNeurons.size()))
{
	for (size_t i = 0; i < countLayers; ++i)
	{
		values.push_back(Vector<T>(countNeurons[i]));
		errors.push_back(Vector<T>(countNeurons[i]));
	}
	for (size_t i = 0; i < countLayers - 1; ++i)
	{
		weights.push_back(Matrix<T>(countNeurons[i], countNeurons[i + 1]));
	}
}

template<typename T>
inline void Neuralnet<T>::fullReset()
{
	for (size_t i = 0; i < countLayers - 1; ++i)
	{
		values[i] = 0.0;
		errors[i] = 0.0;
		weights[i] = 0.0;
	}
	values[countLayers - 1] = 0.0;
	errors[countLayers - 1] = 0.0;
}

template<typename T>
inline void Neuralnet<T>::setInput(const std::vector<T>& input)
{
	values[0] = input;
}

template<typename T>
inline void Neuralnet<T>::setRandomWeights()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<T> distribution(0.0, 1.0);
	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		for (size_t j = 0; j < weights[i].row; ++j)
		{
			for (size_t k = 0; k < weights[i].column; ++k)
			{
				weights[i][j * weights[i].column + k] = distribution(gen);
			}
		}
	}
}

template<typename T>
inline void Neuralnet<T>::forwardFeed()
{
	for (int i = 1; i < countLayers; ++i)
	{
		values[i] = activation(values[i - 1] * weights[i - 1]);
	}
}

template<typename T>
inline void Neuralnet<T>::backPropagation(T learnRate)
{
	for (int i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		weights[i] = weights[i] + ((Matrix(errors[i + 1], false) * Matrix(values[i], true)) * -learnRate);
	}
}

template<typename T>
inline void Neuralnet<T>::findError(const std::vector<T>& rightAnswer, T alpha)
{
	error = 0.0;
	size_t countNeurons = values[countLayers - 1].size;
	errors[countLayers - 1] = (Vector<T>(rightAnswer) - values[countLayers - 1]) * derivative(values[countLayers - 1]);
	for (size_t i = 0; i < countNeurons; ++i)
	{
		error += std::pow(rightAnswer[i] - values[countLayers - 1][i], 2) / countNeurons;
	}
	for (int i = countLayers - 2; i >= 0; --i)
	{
		errors[i] = (weights[i] * errors[i+1]) * derivative(values[i]);
	}
}

template<typename T>
inline T Neuralnet<T>::activation(T value)
{
	return 1.0 / (1.0 + std::exp(-value));
	//return std::logf(1.f + std::powf(2.7182818284f, value));
	//return std::max(0.f, value);
	//return std::log10f(value + 1.f);
}

template<typename T>
inline Vector<T> Neuralnet<T>::activation(const Vector<T>& values)
{
	Vector<T> output(values.size);
	for (size_t i = 0; i < values.size; ++i)
	{
		output[i] = activation(values[i]);
	}
	return output;
}

template<typename T>
inline T Neuralnet<T>::derivative(T value)
{
	return value * (1.0 - value);
	//return 1.f / (1.f + std::powf(2.7182818284f, -value));
	//return value > 0.f ? 1.f : 0.f;
	//return 1.f / (std::logf(10) + value * std::logf(10));
}

template<typename T>
inline Vector<T> Neuralnet<T>::derivative(const Vector<T>& values)
{
	Vector<T> deriv(values.size);
	for (size_t i = 0; i < values.size; ++i)
	{
		deriv[i] = derivative(values[i]);
	}
	return deriv;
}

template<typename T>
inline void Neuralnet<T>::loadWeights(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		file.read(reinterpret_cast<char*>(&weights[i][0]), weights[i].row * weights[i].column * sizeof(T));
	}
	file.close();
}

template<typename T>
inline void Neuralnet<T>::saveWeights(const std::string& filepath)
{
	std::ofstream file(filepath, std::ios::binary);
	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		file.write(reinterpret_cast<const char*>(&weights[i][0]), weights[i].row * weights[i].column * sizeof(T));
	}
	file.close();
}
