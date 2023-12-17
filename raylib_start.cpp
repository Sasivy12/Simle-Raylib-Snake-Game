#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <iostream>

#include "game.h"
#include "food.h"
#include "snake.h"

Color Green = { 90, 166, 50 };
Color darkGreen = { 19, 120, 23 };

const int cellSize = 28;
const int cellCnt = 25;
double lastUpdateTime = 0;
int score = 0;
bool DeathScreen = false;

Food food = Food();
Snake snake = Snake();

bool EventTriggered(double interval)
{
	double currTime = GetTime();

	if (currTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currTime;
		return true;
	}
	return false;
}


void Food::Draw()
{
	Rectangle rect = Rectangle{ position.x * cellSize, position.y * cellSize, cellSize, cellSize };
	DrawRectangleRounded(rect, 0.5, 5, RED);
}

Vector2 Food::GenerateRandPos()
{
	float x = GetRandomValue(0, cellCnt - 1);
	float y = GetRandomValue(0, cellCnt - 1);
	return Vector2{ x, y };
}



void Snake::Draw()
{
	for (int i = 0; i < Body.size(); i++)
	{
		float x = Body[i].x;
		float y = Body[i].y;

		Rectangle rect = Rectangle{ x * cellSize, y * cellSize, cellSize, cellSize };
		DrawRectangleRounded(rect, 0.5, 6, DARKGREEN);
	}
}

void Snake::Move()
{
	Body.pop_back();	
	Body.push_front(Vector2Add(Body[0], Direction));
}



void Game::Draw()
{
	snake.Draw();
	food.Draw();
};

void Game::Update()
{
	snake.Move();
	FoodCollision();
}

void Game::FoodCollision()
{
	if (Vector2Equals(snake.Body[0], food.position))
	{
		Sound crunch = LoadSound("C:\\Users\\05\\Downloads\\crunch.wav");

		food.position = food.GenerateRandPos();
		score++;
		snake.getBigger = true;

		PlaySound(crunch);
	}
	
}

void Game::SnakeBigger()
{
	if (snake.getBigger)
	{
		int newX = snake.Body[0].x;
		int newY = snake.Body[0].y;
		Vector2 newSegment = { newX + snake.Direction.x, newY + +snake.Direction.y };
		snake.Body.push_back(newSegment);
		snake.getBigger = false;
	}
}

void Game::TouchesEdges()
{

	if (snake.Body[0].x <= -1)
	{
		snake.Body[0].x = cellCnt;
	}

	if (snake.Body[0].x > cellCnt)
	{
		snake.Body[0].x = 0;
	}

	if (snake.Body[0].y == -1)
	{
		snake.Body[0].y = cellCnt;
	}

	if (snake.Body[0].y > cellCnt)
	{
		snake.Body[0].y = 0;
	}

}

void Game::TouchesItself()
{

	for (int i = 0; i < snake.Body.size(); i++)
	{
		for (int j = i + 1; j < snake.Body.size(); j++)
		{
			if (snake.Body[i].x == snake.Body[j].x && snake.Body[i].y == snake.Body[j].y)
			{
				Sound death = LoadSound("C:\\Users\\05\\Downloads\\death.wav");

				DeathScreen = true;
				PlaySound(death);
			}
		}
	}
}

void Game::SizeReset()
{
	snake.Body = { Vector2{cellCnt / 2, 9}, Vector2 {cellCnt / 2 - 1, 9}, Vector2 {cellCnt / 2 - 2, 9} };
}


int main()
{
	InitWindow(cellSize * cellCnt, cellSize * cellCnt, "Snake");

	SetTargetFPS(60);

	InitAudioDevice();

	Music backgroundMusic = LoadMusicStream("C:\\Users\\05\\Downloads\\background_music.mp3");
	PlayMusicStream(backgroundMusic);

	Game game;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		if (DeathScreen == true)
		{
			UpdateMusicStream(backgroundMusic);

			DrawText("YOU DIED!!!", cellSize * cellCnt / 2 - 150, cellSize * cellCnt / 2 - 60, 60, BLACK);
			DrawText("Press 'R' to reset", cellSize * cellCnt / 2 - 120, cellSize * cellCnt / 2, 20, BLACK);

			if (IsKeyPressed(KEY_R))
			{
				DeathScreen = false;
				score = 0;
				game.SizeReset();
				food.Food::GenerateRandPos();
			}
		}

		if(DeathScreen == false)
		{
			UpdateMusicStream(backgroundMusic);

			float gameSpeed = 0.2;
			if (EventTriggered(gameSpeed))
			{
				game.Update();
				game.FoodCollision();
				game.SnakeBigger();
				game.TouchesEdges();
				game.TouchesItself();
			}

			if (IsKeyPressed(KEY_DOWN) && snake.Direction.y != -1)
			{
				snake.Direction = { 0, 1 };
			}
			if (IsKeyPressed(KEY_LEFT) && snake.Direction.x != 1)
			{
				snake.Direction = { -1, 0 };
			}
			if (IsKeyPressed(KEY_RIGHT) && snake.Direction.x != -1)
			{
				snake.Direction = { 1, 0 };
			}
			if (IsKeyPressed(KEY_UP) && snake.Direction.y != 1)
			{
				snake.Direction = { 0, -1 };
			}

			if (score % 5 == 0)
			{
				gameSpeed -= 0.5;
			}

			DrawText("Score: ", cellSize * cellCnt / 2 - 330, cellSize * cellCnt - 650, 30, BLACK);
			DrawText(TextFormat("%i", score), cellSize * cellCnt / 2 - 220, cellSize * cellCnt - 650, 30, BLACK);

		}

		ClearBackground(Green);

		DrawText("Snake", cellSize * cellCnt / 2 - 38, cellSize * cellCnt - 680, 40, BLACK);

		game.Draw();

		EndDrawing();
	}

	UnloadMusicStream(backgroundMusic);
	CloseWindow();

	return 0;
}