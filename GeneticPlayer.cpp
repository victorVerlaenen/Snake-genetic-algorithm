#include "pch.h"
#include "GeneticPlayer.h"
#include <algorithm>
#include <cmath>
#include <iostream>
using std::cout, std::endl;

GeneticPlayer::GeneticPlayer(int populationSize, int numOfGenerations, int numOfTrials, float inputSize, int hiddenSize, float mutationChance, float mutationSize)
	:m_PopulationSize{ populationSize }
	, m_NumOfGenerations{ numOfGenerations }
	, m_NumOfTrials{ numOfTrials }
	, m_InputSize{ inputSize }
	, m_HiddenSize{ hiddenSize }
	, m_MutationChance{ mutationChance }
	, m_MutationSize{ mutationSize }
	, m_CurrentBrain{}
	, m_Population{}
{
	Initialize();
}

void GeneticPlayer::Initialize()
{
	for (int i{}; i < m_PopulationSize; ++i)
	{
		m_Population.push_back(GenerateBrain(static_cast<int>(m_InputSize), static_cast<int>(m_HiddenSize), 4));//output size is 4 cuz snake can only move left right up and down
	}
	m_CurrentBrain = m_Population[m_CurrentIndividual];
}

std::vector<std::vector<std::vector<float>>> GeneticPlayer::GenerateBrain(int inputSize, int hiddenSize, int outputSize)
{
	std::vector<std::vector<float>> hiddenLayer1;
	for (int i{}; i < hiddenSize; ++i)
	{
		hiddenLayer1.push_back(std::vector<float>{});
		for (int j{}; j < inputSize; ++j)
		{
			hiddenLayer1[i].push_back(((rand() % 201) - 100) / 100.f);
		}
	}

	std::vector<std::vector<float>> hiddenLayer2;
	for (int i{}; i < hiddenSize; ++i)
	{
		hiddenLayer2.push_back(std::vector<float>{});
		for (int j{}; j < hiddenSize; ++j)
		{
			hiddenLayer2[i].push_back(((rand() % 201) - 100) / 100.f);
		}
	}

	std::vector<std::vector<float>> outputLayer;
	for (int i{}; i < outputSize; ++i)
	{
		outputLayer.push_back(std::vector<float>{});
		for (int j{}; j < hiddenSize; ++j)
		{
			outputLayer[i].push_back(((rand() % 201) - 100) / 100.f);
		}
	}

	return std::vector<std::vector<std::vector<float>>>{hiddenLayer1, hiddenLayer2, outputLayer};
}

std::vector<float> GeneticPlayer::Dot(const std::vector<std::vector<float>>& lhs, const std::vector<float>& rhs)
{
	std::vector<float> result{};
	for (size_t i{}; i < lhs.size(); ++i)
	{
		float temp{ 0 };
		for (size_t j{}; j < rhs.size(); ++j)
		{
			temp += lhs[i][j] * rhs[j];
		}
		temp++;
		result.push_back(temp);
	}
	return result;
}

int GeneticPlayer::GetMove(const std::vector<float>& inputVector)
{
	std::vector<std::vector<float>> hiddenLayer1{ m_CurrentBrain[0] };
	std::vector<std::vector<float>> hiddenLayer2{ m_CurrentBrain[1] };
	std::vector<std::vector<float>> outputLayer{ m_CurrentBrain[2] };
	std::vector<float> hiddenResult1{ Dot(hiddenLayer1, inputVector) };
	for (float& value : hiddenResult1)
	{
		value = std::tanh(value);
	}

	std::vector<float> hiddenResult2{ Dot(hiddenLayer2, hiddenResult1) };
	for (float& value : hiddenResult2)
	{
		value = std::tanh(value);
	}

	std::vector<float> outputResult{ Dot(outputLayer, hiddenResult2) };

	//0 left
	//1 right
	//2 up
	//3 down
	int maxIndex{ 0 };
	for (size_t i{ 1 }; i < outputResult.size(); ++i)
	{
		if (outputResult[i] > outputResult[maxIndex])
			maxIndex = int(i);
	}
	return maxIndex;
}

std::vector<std::vector<std::vector<std::vector<float>>>> GeneticPlayer::Reproduce(const std::vector<std::vector<std::vector<std::vector<float>>>>& top50Brains)
{
	std::vector<std::vector<std::vector<std::vector<float>>>> newPopulation{};

	for (size_t i{}; i < top50Brains.size(); ++i)
	{
		newPopulation.push_back(top50Brains[i]);
	}
	while (int(newPopulation.size()) < m_PopulationSize)
	{
		int randomNumber1 = rand() % int(newPopulation.size());
		int randomNumber2 = rand() % int(newPopulation.size());
		newPopulation.push_back(CrossOver(newPopulation[randomNumber1], newPopulation[randomNumber2]));
		//m_Population.push_back(GenerateBrain(static_cast<int>(m_InputSize), static_cast<int>(m_HiddenSize), 4));
	}
	for (std::vector<std::vector<std::vector<float>>>& brain : newPopulation)
	{
		Mutate(brain);
	}
	return newPopulation;
}

std::vector<std::vector<std::vector<float>>> GeneticPlayer::CrossOver(const std::vector<std::vector<std::vector<float>>>& father, const std::vector<std::vector<std::vector<float>>>& mother)
{
	std::vector<std::vector<std::vector<float>>> newBrain{ father };
	for (size_t i{}; i < father.size(); ++i)
	{
		for (size_t j{}; j < father.size(); ++j)
		{
			float random{ ((rand() % 201) - 100) / 100.f };
			if (random > 0.5f)
				newBrain[i][j] = father[i][j];
			else
				newBrain[i][j] = mother[i][j];
		}
	}
	return newBrain;
}

std::vector<std::vector<std::vector<float>>> GeneticPlayer::Mutate(const std::vector<std::vector<std::vector<float>>>& brain)
{
	std::vector<std::vector<std::vector<float>>> newBrain{};
	for (const std::vector<std::vector<float>>& layer : brain)
	{
		std::vector<std::vector<float>> newLayer{ layer };
		for (size_t i{}; i < newLayer.size(); ++i)
		{
			for (size_t j{}; j < newLayer[i].size(); ++j)
			{
				if (((rand() % 201) - 100) / 100.f < m_MutationChance)
				{
					newLayer[i][j] = ((rand() % 201) - 100) / 100.f;
				}
			}
		}
		newBrain.push_back(newLayer);
	}
	return newBrain;
}

void GeneticPlayer::SetIndividual(int individual)
{
	m_CurrentIndividual = individual;
	m_CurrentBrain = m_Population[m_CurrentIndividual];
}

void GeneticPlayer::NextGeneration(const std::vector<float>& fitnesses)
{
	std::vector<float> sortedCopy{ fitnesses };
	std::sort(sortedCopy.begin(), sortedCopy.end(), std::greater<float>());

	float totalOfFitnesses{};
	std::vector<float> modifiedCopy{ sortedCopy };
	for (float& value : modifiedCopy)//making sure there are no negative fitness values
	{
		value += modifiedCopy[modifiedCopy.size() - 1];
		totalOfFitnesses += value;
	}
	for (float& value : modifiedCopy)//making sure there are no negative fitness values
	{
		value = value / totalOfFitnesses;
	}

	std::vector<std::vector<std::vector<std::vector<float>>>> selected50Percent{};
	for (int i{}; i < int(fitnesses.size())/4; ++i)
	{
		float randomNmbr{ (rand() % 1001) / 1000.f };
		for (int j{}; j < sortedCopy.size(); ++j)
		{
			if (randomNmbr >= modifiedCopy[j])
			{
				auto itr = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[j]);
				if (itr != fitnesses.cend())
				{
					selected50Percent.push_back(m_Population[std::distance(fitnesses.begin(), itr)]);
					//selected50Percent.push_back(GenerateBrain(static_cast<int>(m_InputSize), static_cast<int>(m_HiddenSize), 4));
					break;
				}
			}
		}
		
	}
	m_Population = Reproduce(selected50Percent);
	m_CurrentIndividual = 0;
}