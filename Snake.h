#pragma once
#include <vector>
class Food;

class Snake
{
public:
	enum class MovingDirection
	{
		left = 0,
		right = 1,
		up = 2,
		down = 3
	};

	Snake(const Rectf& worldBounds, float segmentSize);
	~Snake();

	Snake(const Snake& other) = delete;
	Snake& operator=(const Snake& other) = delete;
	Snake(Snake&& other) = delete;
	Snake& operator=(Snake&& other) = delete;

	void Update(float elapsedTime, Food* food);
	void Draw() const;

	std::vector<float> GetInput(const Rectf& food);
	Snake::MovingDirection GetMovingDirection() const { return m_MovingDirection; }
	void GrowSnake();
	void SetMovingDirection(Snake::MovingDirection movingDirection) { m_MovingDirection = movingDirection; }
	bool GetAdvanced() const { return m_SnakeAdvanced; }
	void SetAdvanced(bool advanced) { m_SnakeAdvanced = advanced; }
	std::vector<Rectf> GetSegments(){ return m_Segments; }
	void ResetSnake();
	float GetFitness() const;
	void IncreaseAvgSteps();
	void IncreaseSpeed() { m_MovingSpeed *= 2; }
	void DecreaseSpeed() { m_MovingSpeed *= 2; }
private:
	bool CheckIfHitsBorder();
	bool CheckIfHitsSelf();
	bool CheckIfEatenNothing();
	int m_Size{ 3 };
	float timePassed{ 0 };
	Point2f m_MovingVelocity;
	MovingDirection m_MovingDirection;
	const float m_SegmentSize;
	const float m_Speed{ m_SegmentSize };
	std::vector<Rectf> m_Segments;
	bool m_SnakeAdvanced{ false };
	const Rectf m_WorldBounds;
	int m_CurrentLife{ 1 };
	int m_MaxSteps{ 200 };
	int m_AmountOfSteps{ 0 };
	int m_AmountOfStepsForPenalty{ 0 };
	int m_LastCheckedScore{ 0 };
	float m_Fitness{ 0 };
	int m_Penalties{ 0 };
	int m_AvgSteps{ 0 };
	float m_MovingSpeed{ 0.2f };
};