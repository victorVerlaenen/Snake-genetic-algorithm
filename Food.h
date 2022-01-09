#pragma once
#include <vector>

class Food
{
public:
	Food(const Rectf& worldBounds, int nrColsandRows, const std::vector<Rectf>& snakeSegments);
	~Food() = default;

	Food(const Food& other) = delete;
	Food& operator=(const Food& other) = delete;
	Food(Food&& other) = delete;
	Food& operator=(Food&& other) = delete;

	void Draw() const;

	void GiveRandomPosition(const std::vector<Rectf>& snakeSegments);
	bool CheckIfHit(const std::vector<Rectf>& snakeSegments);
	Rectf GetFood() const { return m_Food; }
private:
	Rectf m_Food;
	const Rectf m_WorldBounds;
	const int m_NrOfColsAndRows;
};