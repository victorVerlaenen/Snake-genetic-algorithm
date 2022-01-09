#include "pch.h"
#include "GeneticPlayer.h"
#include <algorithm>
#include <cmath>
#include <iostream>
using std::cout, std::endl;

GeneticPlayer::GeneticPlayer(int populationSize, int numOfGenerations, int numOfTrials, float inputSize, int hiddenSize, float boardSize, float mutationChance, float mutationSize)
	:m_PopulationSize{ populationSize }
	, m_NumOfGenerations{ numOfGenerations }
	, m_NumOfTrials{ numOfTrials }
	, m_InputSize{ inputSize }
	, m_HiddenSize{ hiddenSize }
	, m_BoardSize{ boardSize }
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


	/*for (std::vector<float> value : hiddenLayer1)
	{
		for (float value1 : value)
		{
			cout << value1 << ", ";
		}
		cout << endl;
	}
	cout << endl;*/



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

int GeneticPlayer::GetMove(std::vector<float> inputVector)
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


	/*for (float value1 : outputResult)
	{
		cout << value1 << ", ";
	}
	cout << endl;*/

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

std::vector<std::vector<std::vector<std::vector<float>>>> GeneticPlayer::Reproduce(const std::vector<std::vector<std::vector<std::vector<float>>>>& top10Brains)
{
	std::vector<std::vector<std::vector<std::vector<float>>>> newPopulation{};
	/*for (const std::vector<std::vector<std::vector<float>>>& brain : top2Brains)
	{
		newPopulation.push_back(brain);
		std::vector<std::vector<std::vector<float>>> newBrain{ Mutate(brain) };
		newPopulation.push_back(newBrain);
	}
	for (int i{}; i < m_PopulationSize / 2; ++i)
	{
		newPopulation.push_back(GenerateBrain(static_cast<int>(m_InputSize), static_cast<int>(m_HiddenSize), 4));
	}*/
	
	for (size_t i{}; i < top10Brains.size(); ++i)
	{
		newPopulation.push_back(top10Brains[i]);
	}
	while (int(newPopulation.size()) < m_PopulationSize)
	{
		int randomNumber1 = rand() % int(newPopulation.size());
		int randomNumber2 = rand() % int(newPopulation.size());
		newPopulation.push_back(CrossOver(newPopulation[randomNumber1], newPopulation[randomNumber2]));
		//newPopulation.push_back(GenerateBrain(static_cast<int>(m_InputSize), static_cast<int>(m_HiddenSize), 4));
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



	//cout << "fitness list: [";
	//for (float value : fitnesses)
	//{
	//	cout << value << ", ";
	//}
	//cout << "]" << endl;
	//cout << "sorted copy of fitness list: [";
	//for (float value : sortedCopy)
	//{
	//	cout << value << ", ";
	//}
	//cout << "]" << endl;



	std::vector<std::vector<std::vector<std::vector<float>>>> top10{};
	auto itr = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[0]);
	if (itr != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr)]);
	}
	auto itr2 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[1]);
	if (itr2 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr2)]);
	}
	auto itr3 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[2]);
	if (itr3 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr3)]);
	}
	auto itr4 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[3]);
	if (itr4 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr4)]);
	}
	auto itr5 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[4]);
	if (itr5 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr5)]);
	}
	auto itr6 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[5]);
	if (itr6 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr6)]);
	}
	auto itr7 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[6]);
	if (itr7 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr7)]);
	}
	auto itr8 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[7]);
	if (itr8 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr8)]);
	}
	auto itr9 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[8]);
	if (itr9 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr9)]);
	}
	auto itr10 = std::find(fitnesses.begin(), fitnesses.end(), sortedCopy[9]);
	if (itr10 != fitnesses.cend())
	{
		top10.push_back(m_Population[std::distance(fitnesses.begin(), itr10)]);
	}

	m_Population = Reproduce(top10);
	m_CurrentIndividual = 0;
}