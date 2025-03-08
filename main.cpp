#include <iostream>
#include "raylib.h"
#include "Entities.h"
#include "Game.h"

void DragWindow()
{
	static bool Dragging = false;
	static Vector2 CursorPos_Original = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		// Update the cursor position
		CursorPos_Original = GetMousePosition();
		Dragging = true;
	}
	else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		Dragging = false;

	if (Dragging)
	{
		Vector2 WindowPos = GetWindowPosition();
		Vector2 CursorPos_Current = GetMousePosition();
		Vector2 CursorDelta = { CursorPos_Current.x - CursorPos_Original.x , CursorPos_Current.y - CursorPos_Original.y };

		SetWindowPosition(WindowPos.x + CursorDelta.x, WindowPos.y + CursorDelta.y);
	}
}

int WinMain()
{
	// Set a seed of rand based on time (Used to generate the position of the apple)
	srand(time(nullptr));

	// Set the window flags ( VSYNC + NoBorders )
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED);

	// Window init
	const int Window_Width = 600;
	const int Window_Height = 600;
	InitWindow(Window_Width, Window_Height, "Snake Game");

	// Create Game object
	Game game;

	while (!WindowShouldClose())
	{
		// Handles dragging
		DragWindow();

		// Runs the game logic
		game.Run();

		// Render Start
		BeginDrawing();
		ClearBackground(Color(30, 30, 30));

		// Render the game elements
		game.Render(Window_Width, Window_Height);

		EndDrawing();
		// Render End
	}

	return 0;
}