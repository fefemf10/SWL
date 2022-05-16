#include "Neuralnet.hpp"

//Neuralnet::Neuralnet(std::vector<uint16_t> countNeurons) : countLayers(static_cast<uint8_t>(countNeurons.size()))
//{
//	for (size_t i = 0; i < countLayers; ++i)
//	{
//		values.push_back(Vector<float>(countNeurons[i]));
//		errors.push_back(Vector<float>(countNeurons[i]));
//	}
//	for (size_t i = 0; i < countLayers - 1; ++i)
//	{
//		weights.push_back(Matrix<float>(countNeurons[i], countNeurons[i + 1]));
//	}
//}
//
//void Neuralnet::fullReset()
//{
//	for (size_t i = 0; i < countLayers - 1; ++i)
//	{
//		values[i] = 0.f;
//		errors[i] = 0.f;
//		weights[i] = 0.f;
//	}
//	values[countLayers - 1] = 0.f;
//	errors[countLayers - 1] = 0.f;
//}
//
//void Neuralnet::setInput(const std::vector<float>& input)
//{
//	values[0] = input;
//}
//
////void Neuralnet::setWeights(uint8_t indexLayer, uint16_t indexNeuron, const std::vector<float>& weights)
////{
////	memcpy(&this->weights[indexLayer][indexNeuron], weights.data(), sizeof(float) * weights.size());
////}
//
//void Neuralnet::setRandomWeights()
//{
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<float> distribution(0.f, 1.f);
//	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
//	{
//		for (size_t j = 0; j < weights[i].row; ++j)
//		{
//			for (size_t k = 0; k < weights[i].column; ++k)
//			{
//				weights[i][j * weights[i].column + k] = distribution(gen);
//			}
//		}
//	}
//}
//
//void Neuralnet::loadWeights(const std::string& filepath)
//{
//	std::ifstream file(filepath, std::ios::binary);
//	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
//	{
//		file.read(reinterpret_cast<char*>(&weights[i][0]), weights[i].row * weights[i].column * sizeof(float));
//	}
//	file.close();
//}
//
//void Neuralnet::saveWeights(const std::string& filepath)
//{
//	std::ofstream file(filepath, std::ios::binary);
//	for (size_t i = 0; i < countLayers - 1; ++i)//-1 last layer
//	{
//		file.write(reinterpret_cast<const char*>(&weights[i][0]), weights[i].row * weights[i].column * sizeof(float));
//	}
//	file.close();
//}
//
//void Neuralnet::forwardFeed()
//{
//	for (int i = 1; i < countLayers; ++i)
//	{
//		values[i] = activation(values[i - 1] * weights[i - 1]);
//	}
//}
//
//void Neuralnet::backPropagation(float learnRate)
//{
//	for (int i = 0; i < countLayers - 1; ++i)//-1 last layer
//	{
//		weights[i] = weights[i] + ((Matrix(errors[i + 1], false) * Matrix(values[i], true)) * -1.f * learnRate);
//	}
//}
//
//void Neuralnet::findError(const std::vector<float>& rightAnswer, float alpha)
//{
//	error = 0.f;
//	size_t countNeurons = values[countLayers - 1].size;
//	errors[countLayers - 1] = (Vector<float>(rightAnswer) - values[countLayers - 1]) * derivative(values[countLayers - 1]) * -2.f * alpha;
//	for (size_t i = 0; i < countNeurons; ++i)
//	{
//		error += std::powf(rightAnswer[i] - values[countLayers - 1][i], 2) / countNeurons;
//	}
//#pragma omp parallel for
//	for (int i = countLayers - 2; i >= 0; --i)
//	{
//		errors[i] = weights[i] * (errors[i + 1] * derivative(values[i]) * -2.f * alpha);
//	}
//}