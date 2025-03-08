#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include "raylib.h"

class Snake
{
public:
	bool IsAlive;
	//int Points = 0;
	std::string Direction;
	int Size; // Head
	std::vector<Vector2> Stored_Pos;

	void Reset(Vector2 Grid)
	{
		Vector2 InitialPos = { std::floor(Grid.x / 2) , std::floor(Grid.y / 2) };

		IsAlive = true;
		Direction = "";
		Size = 3;
		Stored_Pos.clear();
		Stored_Pos.push_back(InitialPos);
	}
};

class Apple
{
public:
	Vector2 Pos;

	void NewPosition(Vector2 Grid)
	{
		// Get a random position
		Vector2 RandomPos = { rand() % (int)Grid.x , rand() % (int)Grid.y };

		Pos = RandomPos;
	}
};