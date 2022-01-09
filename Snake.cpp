#include "pch.h"
#include "Snake.h"
#include <iostream>
#include "utils.h"
#include "World.h"
#include "Food.h"
#include <math.h>
#include "Game.h"

using std::cout, std::cin, std::endl;

Snake::Snake(const Rectf& worldBounds, float segmentSize)
	:m_MovingDirection{ Snake::MovingDirection::up }
	, m_WorldBounds{ worldBounds }
	, m_SegmentSize{ segmentSize }
{
	m_Segments.push_back(Rectf{ (worldBounds.left) + (worldBounds.width / 2.f), worldBounds.bottom + (worldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });
	m_Segments.push_back(Rectf{ (worldBounds.left + m_SegmentSize) + (worldBounds.width / 2.f), worldBounds.bottom + (worldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });
	m_Segments.push_back(Rectf{ (worldBounds.left + m_SegmentSize * 2) + (worldBounds.width / 2.f), worldBounds.bottom + (worldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });
}

Snake::~Snake()
{

}

void Snake::Update(float elapsedTime, Food* food)
{
	switch (m_MovingDirection)
	{
	case Snake::MovingDirection::left:
		m_MovingVelocity = Point2f{ -m_SegmentSize, 0 };
		break;
	case Snake::MovingDirection::right:
		m_MovingVelocity = Point2f{ m_SegmentSize, 0 };
		break;
	case Snake::MovingDirection::up:
		m_MovingVelocity = Point2f{ 0, m_SegmentSize };
		break;
	case Snake::MovingDirection::down:
		m_MovingVelocity = Point2f{ 0, -m_SegmentSize };
		break;
	}

	timePassed += elapsedTime;
	if (timePassed >= 0.001)
	{
		Rectf newSegment{ m_Segments[0].left + m_MovingVelocity.x, m_Segments[0].bottom + m_MovingVelocity.y, m_SegmentSize, m_SegmentSize };
		for (size_t i{ m_Segments.size() - 1 }; i > 0; --i)
		{
			m_Segments[i] = m_Segments[i - 1];
		}
		m_Segments[0] = newSegment;
		timePassed = 0;
		m_SnakeAdvanced = true;
		m_AmountOfSteps++;
		m_AmountOfStepsForPenalty++;
	}

	if (CheckIfHitsBorder() || CheckIfHitsSelf() || CheckIfEatenNothing())
	{
		ResetSnake();
		food->GiveRandomPosition(m_Segments);
		World::ResetScore();
	}
	if (m_AmountOfSteps > m_MaxSteps)
	{
		//cout << "AvgSteps: " << m_AvgSteps << endl;
		m_Fitness = World::GetMaxScore() * 5000 - m_CurrentLife * 150 - m_Penalties * 1000 - (m_AvgSteps / m_CurrentLife) * 100;
		m_Penalties = 0;
		m_AmountOfSteps = 0;
		m_CurrentLife = 1;
		m_AvgSteps = 0;
		Game::GameOver();
	}
}

void Snake::IncreaseAvgSteps()
{
	m_AvgSteps += m_AmountOfSteps;
	//cout << "AvgSteps: " << m_AvgSteps << endl;
}

float Snake::GetFitness() const
{
	return m_Fitness;
}

void Snake::Draw() const
{
	for (const Rectf& segment : m_Segments)
	{
		//utils::SetColor(Color4f{ 0.2f, 0.8f, 0.2f, 1.0f });
		utils::SetColor(Color4f{ 0.1f, 0.1f, 0.1f, 1.0f });
		utils::FillRect(segment);
		//utils::SetColor(Color4f{ 0.1f, 0.6f, 0.1f, 1.0f });
		//utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.0f });
		//utils::DrawRect(segment);
	}
}

void Snake::GrowSnake()
{
	m_Segments.push_back(m_Segments[m_Segments.size() - 1]);
}

bool Snake::CheckIfHitsBorder()
{
	if (m_Segments[0].left < m_WorldBounds.left
		|| m_Segments[0].bottom < m_WorldBounds.bottom
		|| (m_Segments[0].left + m_Segments[0].width) >(m_WorldBounds.left + m_WorldBounds.width)
		|| (m_Segments[0].bottom + m_Segments[0].height) >(m_WorldBounds.bottom + m_WorldBounds.height))
	{
		return true;
	}
	return false;
}

bool Snake::CheckIfHitsSelf()
{
	for (size_t i{ 1 }; i < m_Segments.size(); ++i)
	{
		//Also have to give a little padding for the head to make sure the collision is correct
		if (utils::IsOverlapping(Rectf{ m_Segments[0].left + 1, m_Segments[0].bottom + 1, m_Segments[0].width - 2, m_Segments[0].height - 2 }, m_Segments[i]))
			return true;
	}
	return false;
}

bool Snake::CheckIfEatenNothing()
{
	if (m_AmountOfStepsForPenalty >= 200)
	{
		if (m_LastCheckedScore == World::GetScore())
		{
			m_Penalties++;
			m_AmountOfStepsForPenalty = 0;
			return true;
		}
		m_LastCheckedScore = int(World::GetScore());
	}
	return false;
}

void Snake::ResetSnake()
{
	m_Segments.clear();
	m_Segments.push_back(Rectf{ (m_WorldBounds.left) + (m_WorldBounds.width / 2.f), m_WorldBounds.bottom + (m_WorldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });
	m_Segments.push_back(Rectf{ (m_WorldBounds.left + m_SegmentSize) + (m_WorldBounds.width / 2.f), m_WorldBounds.bottom + (m_WorldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });
	m_Segments.push_back(Rectf{ (m_WorldBounds.left + m_SegmentSize * 2) + (m_WorldBounds.width / 2.f), m_WorldBounds.bottom + (m_WorldBounds.height / 2.f), m_SegmentSize, m_SegmentSize });

	m_MovingDirection = MovingDirection::left;
	m_CurrentLife++;
	m_AmountOfStepsForPenalty = 0;
	//IncreaseAvgSteps();
}

std::vector<float> Snake::GetInput(const Rectf& food)
{
	//Is there danger to the left of the head ? (0 no 1 yes)	1
	//Is there danger to the right of the head ? (0 no 1 yes)	2
	//Is there danger to the top of the head ? (0 no 1 yes)		3
	//Is there danger to the bottom of the head ? (0 no 1 yes)	4
	//Is the snake moving left ? (0 no 1 yes)					5
	//Is the snake moving right ? (0 no 1 yes)					6
	//Is the snake moving up ? (0 no 1 yes)						7
	//Is the snake moving down ? (0 no 1 yes)					8
	//Is there food to the left of the head ? (0 no 1 yes)		9
	//Is there food to the right of the head ? (0 no 1 yes)		10
	//Is there food to the top of the head ? (0 no 1 yes)		11
	//Is there food to the bottom of the head ? (0 no 1 yes)	12

	std::vector<float> inputVec;
	//wall left
	if (m_Segments[0].left - m_SegmentSize < m_WorldBounds.left)
		inputVec.push_back(-1);
	else
		inputVec.push_back(0);

	//wall right
	if ((m_Segments[0].left + m_SegmentSize * 2) > (m_WorldBounds.left + m_WorldBounds.width))
		inputVec.push_back(-1);
	else
		inputVec.push_back(0);

	//wall up
	if (m_Segments[0].bottom + m_SegmentSize < m_WorldBounds.bottom)
		inputVec.push_back(-1);
	else
		inputVec.push_back(0);

	//wall down
	if ((m_Segments[0].bottom - m_SegmentSize * 2) > (m_WorldBounds.bottom + m_WorldBounds.height))
		inputVec.push_back(-1);
	else
		inputVec.push_back(0);

	//segments
	for (size_t i{ 1 }; i < m_Segments.size(); ++i)
	{
		//left
		if (utils::IsOverlapping(Rectf{ m_Segments[0].left - m_SegmentSize + 1, m_Segments[0].bottom + 1, m_Segments[0].width - 2, m_Segments[0].height - 2 }, m_Segments[i]))
		{
			inputVec[0] = -1;
			continue;
		}
		//right
		if (utils::IsOverlapping(Rectf{ m_Segments[0].left + m_SegmentSize + 1, m_Segments[0].bottom + 1, m_Segments[0].width - 2, m_Segments[0].height - 2 }, m_Segments[i]))
		{
			inputVec[1] = -1;
			continue;
		}
		//up
		if (utils::IsOverlapping(Rectf{ m_Segments[0].left + 1, m_Segments[0].bottom + m_SegmentSize + 1, m_Segments[0].width - 2, m_Segments[0].height - 2 }, m_Segments[i]))
		{
			inputVec[2] = -1;
			continue;
		}
		//bottom
		if (utils::IsOverlapping(Rectf{ m_Segments[0].left + 1, m_Segments[0].bottom - m_SegmentSize + 1, m_Segments[0].width - 2, m_Segments[0].height - 2 }, m_Segments[i]))
		{
			inputVec[3] = -1;
			continue;
		}
	}


	Point2f vectorToFood{ food.left - m_Segments[0].left, food.bottom - m_Segments[0].bottom };
	Point2f otherVector{};

	switch (m_MovingDirection)
	{
	case Snake::MovingDirection::left:
		otherVector = Point2f{ -1, 0 };
		break;
	case Snake::MovingDirection::right:
		otherVector = Point2f{ 1, 0 };
		break;
	case Snake::MovingDirection::up:
		otherVector = Point2f{ 0, 1 };
		break;
	case Snake::MovingDirection::down:
		otherVector = Point2f{ 0, -1 };
		break;
	}

	float cosAngle{ (vectorToFood.x * otherVector.x + vectorToFood.y * otherVector.y) / (sqrt((vectorToFood.x * vectorToFood.x) + (vectorToFood.y * vectorToFood.y)) * sqrt((otherVector.x * otherVector.x) + (otherVector.y * otherVector.y))) };
	float angle{ acosf(cosAngle) };
	angle *= 180 / float(M_PI);
	
	inputVec.push_back(sinf(angle));



	/*if (m_MovingDirection == MovingDirection::left)
		inputVec.push_back(1);
	else
		inputVec.push_back(0);
	if (m_MovingDirection == MovingDirection::right)
		inputVec.push_back(1);
	else
		inputVec.push_back(0);
	if (m_MovingDirection == MovingDirection::up)
		inputVec.push_back(1);
	else
		inputVec.push_back(0);
	if (m_MovingDirection == MovingDirection::down)
		inputVec.push_back(1);
	else
		inputVec.push_back(0);*/

	

	//cout << angle << endl;
	// 
	//food left
	//if (food.left < m_Segments[0].left)
	//	inputVec.push_back(1);
	//else
	//	inputVec.push_back(0);
	////food right
	//if (food.left > m_Segments[0].left)
	//	inputVec.push_back(1);
	//else
	//	inputVec.push_back(0);
	////food up
	//if (food.bottom > m_Segments[0].bottom )
	//	inputVec.push_back(1);
	//else
	//	inputVec.push_back(0);
	////food down
	//if (food.bottom < m_Segments[0].bottom )
	//	inputVec.push_back(1);
	//else
	//	inputVec.push_back(0);


	return inputVec;
}