#pragma once
#include "raylib.h"
#include "Entities.h"

class Game
{
public:
	Game();

	void Restart();
	void ProcessInput();
	void ProcessDirection();
	void ProcessColision();
	void Run();
	void Render(const int Window_Width, const int Window_Height);
private:
	const double TicksPerSecond = 1 / 10.f;
	Vector2 Grid = {13 , 13}; // Grid size
	Snake Player;
	Apple Food;
	bool Won = false;
};