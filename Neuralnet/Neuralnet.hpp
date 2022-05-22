#pragma once
#include <random>
#include <fstream>
#include "Neuron.hpp"

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
	void backPropagation(const std::vector<T>& rightAnswer, T learnRate = 0.1);
	//std::vector<std::vector<Neuron>> neurons; //[layers][neuron]
	std::vector<std::vector<T>> weights;
	std::vector<std::vector<T>> values;
	std::vector<std::vector<T>> errors;
	float error;
	uint8_t countLayers;
private:
	T activation(T value);
	T derivative(T value);
};

template<typename T>
inline Neuralnet<T>::Neuralnet(std::vector<uint16_t> countNeurons) : countLayers(static_cast<uint8_t>(countNeurons.size()))
{
	for (size_t i = 0; i < countLayers; ++i)
	{
		values.push_back(std::vector<T>(countNeurons[i]));
		errors.push_back(std::vector<T>(countNeurons[i]));
	}
	for (size_t i = 0; i < countLayers - 1; ++i)
	{
		weights.push_back(std::vector<T>(countNeurons[i] * countNeurons[i + 1]));
	}
}

template<typename T>
inline void Neuralnet<T>::fullReset()
{
	for (size_t i = 0; i < countLayers; ++i)
	{
		const size_t countNeurons = values[i].size();
		for (size_t j = 0; j < countNeurons; j++)
		{
			values[i][j] = 0.0;
			errors[i][j] = 0.0;
		}
	}
	for (size_t i = 0; i < countLayers - 1; ++i)
	{
		const size_t n = values[i].size();
		const size_t m = values[i + 1].size();
		for (size_t j = 0; j < n; j++)
		{
			for (size_t k = 0; k < m; k++)
			{
				weights[i][j * m + k] = 0.0;
			}
		}
	}
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
	for (size_t i = 0; i < countLayers - 1; ++i)
	{
		const size_t n = values[i].size();
		const size_t m = values[i + 1].size();
		for (size_t j = 0; j < n; ++j)
		{
			for (size_t k = 0; k < m; ++k)
			{
				weights[i][j * m + k] = distribution(gen);
			}
		}
	}
}

template<typename T>
inline void Neuralnet<T>::forwardFeed()
{
	for (int i = 0; i < countLayers - 1; ++i)
	{
		const size_t n = values[i].size();
		const size_t m = values[i + 1].size();
		for (size_t j = 0; j < m; j++)
		{
			values[i + 1][j] = 0.0;
		}
		for (size_t j = 0; j < n; j++)
		{
			for (size_t k = 0; k < m; k++)
			{
				values[i + 1][k] += (weights[i][j * m + k] * values[i][j]);
 			}
		}
		for (size_t k = 0; k < m; k++)
		{
			values[i + 1][k] = activation(values[i + 1][k]);
		}
	}
}

template<typename T>
inline void Neuralnet<T>::backPropagation(const std::vector<T>& rightAnswer, T learnRate)
{
	error = 0.0;
	size_t countNeurons = values[countLayers - 1].size();
	for (size_t i = 0; i < countNeurons; i++)
	{
		errors[countLayers - 1][i] = values[countLayers - 1][i] - rightAnswer[i];
		error += errors[countLayers - 1][i] * errors[countLayers - 1][i];
	}
	error /= countNeurons;
	for (int i = countLayers - 2; i >= 0; --i)
	{
		const size_t n = values[i].size();
		const size_t m = values[i+1].size();
		for (size_t k = 0; k < m; k++)
		{
			const T weightsDelta = errors[i + 1][k] * derivative(values[i + 1][k]);
			for (size_t j = 0; j < n; j++)
			{
				weights[i][j * m + k] -= values[i][j] * weightsDelta * learnRate;
				errors[i][j] = weights[i][j * m + k] * weightsDelta;
			}
		}
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
inline T Neuralnet<T>::derivative(T value)
{
	return value * (1.0 - value);
	//return 1.f / (1.f + std::powf(2.7182818284f, -value));
	//return value > 0.f ? 1.f : 0.f;
	//return 1.f / (std::logf(10) + value * std::logf(10));
}

template<typename T>
inline void Neuralnet<T>::loadWeights(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		file.read(reinterpret_cast<char*>(&weights[i][0]), values[i].size() * values[i+1].size()  * sizeof(T));
	}
	file.close();
}

template<typename T>
inline void Neuralnet<T>::saveWeights(const std::string& filepath)
{
	std::ofstream file(filepath, std::ios::binary);
	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
	{
		file.write(reinterpret_cast<const char*>(&weights[i][0]), values[i].size() * values[i + 1].size() * sizeof(T));
	}
	file.close();
}
