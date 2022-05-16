#pragma once
#include <vector>
#include <cmath>
struct Neuron
{
	Neuron() = default;
	Neuron(float value, uint16_t countWeights);
	void forwardFeed(const std::vector<Neuron>& neurons);
	std::vector<float> weights;
	float value = 0.f;
	float error = 0.f;
	uint16_t indexNeuron;
};