#include "pch.h"
#include "Game.h"
#include "Snake.h"
#include "Food.h"
#include "World.h"
#include "GeneticPlayer.h"
#include "utils.h"
#include <iostream>
using std::cout, std::endl;

int Game::m_Generation{ 0 };
int Game::m_Individual{ 0 };
bool Game::m_NextGen{ false };
bool Game::m_NextInd{ false };
bool Game::m_Pause{ true };

void Game::GameOver()
{
	m_Individual++;
	m_NextInd = true;
	if (m_Individual >= 100)
	{
		m_Individual = 0;
		m_Generation++;
		m_NextGen = true;
		if (m_Generation == 50 && m_Pause)
		{
			system("pause");
			m_Pause = false;
		}
		if (m_Generation >= 100)
		{
			cout << "COMPLETE";
			system("pause");
		}
	}
	cout << endl << "======================================" << endl << "generation: " << m_Generation << ", individual: " << m_Individual << endl;
}
Game::Game(const Window& window)
	:m_Window{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_pWorld = std::make_unique<World>(Rectf{ 50.f, 50.f, 400.f, 400.f }, 16);//gives a border of 50 for the window
	m_pSnake = std::make_unique<Snake>(m_pWorld->GetWorldBounds(), m_pWorld->GetWorldBounds().width / m_pWorld->GetNrOfColsAndRows());
	m_pFood = std::make_unique<Food>(m_pWorld->GetWorldBounds(), m_pWorld->GetNrOfColsAndRows(), m_pSnake->GetSegments());
	m_pPlayer = std::make_unique<GeneticPlayer>(100, 100, 1, 9.f, 15, 10.f);
	cout << "generation: " << m_Generation << ", individual: " << m_Individual << endl;

	
}

void Game::Cleanup()
{
}

void Game::Update(float elapsedSec)
{
	if (m_NextInd)
	{
		m_TempLife = 1;
		m_Fitnessess.push_back(m_pSnake->GetFitness());
		//cout << m_pSnake->GetFitness() << endl;
		m_pPlayer->SetIndividual(m_Individual);
		m_NextInd = false;
		World::ResetMaxScore();
	}
	if (m_NextGen)
	{
		m_NextGen = false;
		m_pPlayer->NextGeneration(m_Fitnessess);
		m_Fitnessess.clear();
	}

	MoveSnake(m_pPlayer->GetMove(m_pSnake->GetInput(m_pFood->GetFood())));
	m_pSnake->Update(elapsedSec, m_pFood.get());
	if (m_pFood->CheckIfHit(m_pSnake->GetSegments()))
	{
		m_pSnake->IncreaseAvgSteps();
		m_pSnake->GrowSnake();
		m_pSnake->IncreaseFoodEaten();
		m_TempLife = m_pSnake->GetLife();
		m_pWorld->IncreaseScore();
	}
}

void Game::Draw() const
{
	ClearBackground();
	m_pFood->Draw();
	m_pSnake->Draw();
}

void Game::MoveSnake(int move) 
{
	if (m_pSnake->GetAdvanced())
	{
		switch (move)
		{
		case 0:
			if (m_pSnake->GetMovingDirection() == Snake::MovingDirection::left || m_pSnake->GetMovingDirection() == Snake::MovingDirection::right)
				break;
			m_pSnake->SetMovingDirection(Snake::MovingDirection::left);
			m_pSnake->SetAdvanced(false);
			break;
		case 1:
			if (m_pSnake->GetMovingDirection() == Snake::MovingDirection::left || m_pSnake->GetMovingDirection() == Snake::MovingDirection::right)
				break;
			m_pSnake->SetMovingDirection(Snake::MovingDirection::right);
			m_pSnake->SetAdvanced(false);
			break;
		case 2:
			if (m_pSnake->GetMovingDirection() == Snake::MovingDirection::up || m_pSnake->GetMovingDirection() == Snake::MovingDirection::down)
				break;
			m_pSnake->SetMovingDirection(Snake::MovingDirection::up);
			m_pSnake->SetAdvanced(false);
			break;
		case 3:
			if (m_pSnake->GetMovingDirection() == Snake::MovingDirection::up || m_pSnake->GetMovingDirection() == Snake::MovingDirection::down)
				break;
			m_pSnake->SetMovingDirection(Snake::MovingDirection::down);
			m_pSnake->SetAdvanced(false);
			break;
		}
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch ( e.keysym.sym )
	{
	case SDLK_UP:
		m_pSnake->IncreaseSpeed();
		break;
	case SDLK_DOWN:
		m_pSnake->DecreaseSpeed();
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
	glClearColor(0.3f, 0.4f, 0.26f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	utils::SetColor(Color4f{ 0.47f, 0.64f, 0.32f, 1.0f });
	utils::FillRect(m_pWorld->GetWorldBounds());
}
