#include "pch.h"
#include "Food.h"
#include <iostream>
#include "utils.h"

using std::cout, std::cin, std::endl;

Food::Food(const Rectf& worldBounds, int nrColsandRows, const std::vector<Rectf>& snakeSegments)
	:m_WorldBounds{worldBounds}
	, m_NrOfColsAndRows{nrColsandRows}
{
	GiveRandomPosition(snakeSegments);
	m_Food.width = m_WorldBounds.width / m_NrOfColsAndRows;
	m_Food.height = m_WorldBounds.height / m_NrOfColsAndRows;
}

void Food::Draw() const
{
	//utils::SetColor(Color4f{ 0.8f, 0.2f, 0.2f, 1.0f });
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.0f });
	utils::FillRect(m_Food.left + m_Food.width/3, m_Food.bottom, m_Food.width/3, m_Food.width / 3);
	utils::FillRect(m_Food.left, m_Food.bottom + m_Food.height/3, m_Food.width / 3, m_Food.width / 3);
	utils::FillRect(m_Food.left + m_Food.width / 3, m_Food.bottom + m_Food.height / 3 * 2, m_Food.width / 3, m_Food.width / 3);
	utils::FillRect(m_Food.left + m_Food.width / 3*2, m_Food.bottom + m_Food.height / 3, m_Food.width / 3, m_Food.width / 3);
}

void Food::GiveRandomPosition(const std::vector<Rectf>& snakeSegments)
{
	m_Food.left = float(rand() % m_NrOfColsAndRows);
	m_Food.bottom = float(rand() % m_NrOfColsAndRows);

	m_Food.left *= (m_WorldBounds.width / m_NrOfColsAndRows);
	m_Food.bottom *= (m_WorldBounds.height / m_NrOfColsAndRows);

	m_Food.left += m_WorldBounds.left;
	m_Food.bottom += m_WorldBounds.bottom;
	for (const Rectf& segment : snakeSegments)
	{
		if (utils::IsOverlapping(m_Food, segment))
		{
			GiveRandomPosition(snakeSegments);
			return;
		}
	}
}

bool Food::CheckIfHit(const std::vector<Rectf>& snakeSegments)
{
	//had to give the check a little padding to make sure the check didnt succeed on all the neigboring cells of the food
	if (utils::IsOverlapping(Rectf{ m_Food.left + 1, m_Food.bottom + 1, m_Food.width - 2, m_Food.height -2}, snakeSegments[0]))
	{
		GiveRandomPosition(snakeSegments);
		return true;
	}
	return false;
}