#include <iostream>
#include <chrono>
#include "Game.h"

Game::Game()
{
	Restart();
}

void Game::Restart()
{
	Player.Reset(Grid);
	Food.NewPosition(Grid);
}

void Game::ProcessInput()
{
	int KeyPressed = GetKeyPressed();

	if (KeyPressed == KEY_W && Player.Direction != "Down")
		Player.Direction = "Up";
	else if (KeyPressed == KEY_A && Player.Direction != "Right")
		Player.Direction = "Left";
	else if (KeyPressed == KEY_S && Player.Direction != "Up")
		Player.Direction = "Down";
	else if (KeyPressed == KEY_D && Player.Direction != "Left")
		Player.Direction = "Right";
}

void Game::ProcessDirection()
{
	static double Accumulator = 0;

	static auto LastTime = std::chrono::high_resolution_clock::now();
	auto CurrentTime = std::chrono::high_resolution_clock::now();

	double DeltaTime = std::chrono::duration<double>(CurrentTime - LastTime).count();

	LastTime = CurrentTime;

	Accumulator += DeltaTime;

	while (Accumulator >= TicksPerSecond)
	{
		Accumulator -= TicksPerSecond;

		Vector2 RelativePos = { 0 , 0 };

		if (Player.Direction == "Up")
			RelativePos = { 0 , -1 };
		else if (Player.Direction == "Left")
			RelativePos = { -1 , 0 };
		else if (Player.Direction == "Down")
			RelativePos = { 0 , 1 };
		else if (Player.Direction == "Right")
			RelativePos = { 1 , 0 };
		else
			return;

		Vector2 OldPos = Player.Stored_Pos.at(0); // Old head pos
		Vector2 FinalPos = { OldPos.x + RelativePos.x , OldPos.y + RelativePos.y };

		Player.Stored_Pos.insert(Player.Stored_Pos.begin(), FinalPos);

		while (Player.Size < Player.Stored_Pos.size()) // Making this a while just to be 100% sure that everything is OK
			Player.Stored_Pos.pop_back();
	}
}

void Game::ProcessColision()
{
	Vector2 HeadPos = Player.Stored_Pos.at(0);

	// Yes, hitting an apple is still a colision
	if (HeadPos.x == Food.Pos.x && HeadPos.y == Food.Pos.y)
	{
		Player.Size++;

		if (Player.Size >= (Grid.x * Grid.y)) // Player has reached the maximum size
		{
			Won = true;
			return;
		}

		for (int i = 0; i < Player.Stored_Pos.size(); i++)
		{
			Vector2 BodyPart = Player.Stored_Pos.at(i);

			// So it doesn't spawn at the player
			if (BodyPart.x == Food.Pos.x && BodyPart.y == Food.Pos.y)
			{
				Food.NewPosition(Grid);

				// Setting i = 0 so this runs every time the NewPosition is called (Bruteforce)
				i = 0;
			}
		}

		//Player.Points++;
	}

	if (HeadPos.x < 0 || HeadPos.x > Grid.x - 1)
	{
		Player.IsAlive = false;
	}
	else if (HeadPos.y < 0 || HeadPos.y > Grid.y - 1)
	{
		Player.IsAlive = false;
	}

	for (int i = 1; i < Player.Stored_Pos.size(); i++) // Start at 1 so the head doesn't colide with itself
	{
		Vector2 BodyPart = Player.Stored_Pos.at(i);

		if (HeadPos.x == BodyPart.x && HeadPos.y == BodyPart.y)
		{
			Player.IsAlive = false;
			break;
		}
	}

	if (!Player.IsAlive)
		Restart();
}

void Game::Run()
{
	if (Won)
		return;

	ProcessInput();
	ProcessDirection();
	ProcessColision();
}

void Game::Render(const int Window_Width, const int Window_Height)
{
	float RelativeWidth = Window_Width / Grid.x;
	float RelativeHeight = Window_Height / Grid.y;

	for (int y = 0; y < Grid.y; y++)
	{
		for (int x = 0; x < Grid.x; x++)
		{
			Rectangle FramePos = { RelativeWidth * x , RelativeHeight * y , RelativeWidth , RelativeHeight }; // Need to add one so you can see the first line

			Color FrameOutline_Color = { 50 , 50 , 50 , 255 };
			Color FrameFill_Color = { 0 , 0 , 0 , 0 };

			if (Food.Pos.x == x && Food.Pos.y == y)
				FrameFill_Color = { 242 , 71 , 55 , 255 };

			for (int i = 0; i < Player.Stored_Pos.size(); i++)
			{
				Vector2 BodyPos = Player.Stored_Pos.at(i);

				if (BodyPos.x == x && BodyPos.y == y)
				{
					if (i == 0) // Head
						FrameFill_Color = { 110 , 227 , 0 , 255 };
					else
						FrameFill_Color = { 124 , 255 , 0 , 255 };
				}
			}

			DrawRectangleLinesEx(FramePos, 1, FrameOutline_Color);
			DrawRectangleRec(FramePos, FrameFill_Color);
		}
	}
}