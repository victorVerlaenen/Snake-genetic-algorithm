#pragma once
#include <memory>
class Texture;
class Food;

class World
{
public:
	World(const Rectf& bounds, const int nrOfcolsAndRows);
	~World() = default;

	World(const World& other) = delete;
	World& operator=(const World& other) = delete;
	World(World&& other) = delete;
	World& operator=(World&& other) = delete;

	Rectf GetWorldBounds() const { return m_Bounds; }
	int GetNrOfColsAndRows() const { return m_NrOfColsAndRows; }
	void IncreaseScore();
	static void ResetScore();
	static void ResetMaxScore();
	static float GetMaxScore() { return m_MaxScore; }
	static float GetScore() { return m_Score; }
private:
	const Rectf m_Bounds;
	const int m_NrOfColsAndRows;
	
	float m_TimeSurvived{0};
	static float m_Score;
	static float m_MaxScore;
};