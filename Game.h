#pragma once
#include <memory>
#include <vector>
class Snake;
class World;
class Food;
class GeneticPlayer;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;
	static void GameOver();

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;
	std::unique_ptr<Snake> m_pSnake;
	std::unique_ptr<World> m_pWorld; 
	std::unique_ptr<Food> m_pFood;
	std::unique_ptr<GeneticPlayer> m_pPlayer;
	static int m_Generation;
	static int m_Individual;
	static bool m_NextGen;
	static bool m_NextInd;
	std::vector<float> m_Fitnessess;
	static bool m_Pause;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void MoveSnake(int move);
};