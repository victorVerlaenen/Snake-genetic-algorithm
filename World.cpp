#include "pch.h"
#include "Food.h"
#include "World.h"
#include "Texture.h"
#include <iostream>
using std::cout, std::endl;

float World::m_Score{ 0 };
float World::m_MaxScore{ 0 };
void World::ResetScore()
{
	m_Score = 0;
}void World::ResetMaxScore()
{
	m_MaxScore = 0;
}
void World::IncreaseScore()
{
	m_Score += 1;
	if (m_Score > m_MaxScore)
	{
		m_MaxScore = m_Score;
		cout << "Max Score: " << m_MaxScore << endl;
	}
}

World::World(const Rectf& bounds, const int nrOfcolsAndRows)
	:m_Bounds{bounds}
	,m_NrOfColsAndRows{nrOfcolsAndRows}
{
	cout << "Max Score: " << m_MaxScore << endl;
}