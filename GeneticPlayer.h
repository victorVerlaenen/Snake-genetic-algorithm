#pragma once
#include <vector>
#include "Snake.h"

class GeneticPlayer
{
public:
	//numOfTrials is the number of games each individual is gonna play
	//hiddenSize is the size of the brain
	//mutationChance is the probability that a mutation is introduced when spawning a new generation
	//mutationSize is the magnitude that those mutations occur
	GeneticPlayer(int populationSize, int numOfGenerations, int numOfTrials, float inputSize, int hiddenSize, float boardSize, float mutationChance = 0.1f, float mutationSize = 0.1f);
	void SetIndividual(int individual);
	void NextGeneration(const std::vector<float>& fitnesses);
	int GetMove(std::vector<float> inputVector);
	std::vector<std::vector<std::vector<std::vector<float>>>> Reproduce(const std::vector<std::vector<std::vector<std::vector<float>>>>& top2Brains);
private:
	void Initialize();
	std::vector<std::vector<std::vector<float>>> CrossOver(const std::vector<std::vector<std::vector<float>>>& father, const std::vector<std::vector<std::vector<float>>>& mother);
	std::vector<float> Dot(const std::vector<std::vector<float>>& lhs, const std::vector<float>& rhs);
	std::vector<std::vector<std::vector<float>>> GenerateBrain(int inputSize, int hiddenSize, int outputSize);
	std::vector<std::vector<std::vector<float>>> Mutate(const std::vector<std::vector<std::vector<float>>>& brain);

	std::vector<std::vector<std::vector<float>>> m_CurrentBrain;
	std::vector<std::vector<std::vector<std::vector<float>>>> m_Population;
	const int m_PopulationSize;
	const int m_NumOfGenerations;
	const int m_NumOfTrials;
	const float m_InputSize;
	const int m_HiddenSize;
	const float m_BoardSize;
	const float m_MutationChance;
	const float m_MutationSize;
	int m_CurrentIndividual{ 0 };
};

