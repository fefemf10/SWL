#include "Neuron.hpp"

Neuron::Neuron(float value, uint16_t countWeights) : value(value), weights(countWeights), error()
{
}

void Neuron::forwardFeed(const std::vector<Neuron>& neurons)
{
	float sum{};
	for (size_t i = 0; i < neurons.size(); ++i)
	{
		sum += neurons[i].value * neurons[i].weights[indexNeuron];
	}
	value = 1.f / (1.f + std::powf(2.7182818284f, -sum));
}