#include "Game.h"
#include "Display.h"
#include "Sprite.h"
#include "Vehicle.h"
#include "Road.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>
#include <stdlib.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include <time.h>
}

Game::Game() : display(Display()), player(Vehicle(&display)), score(0), quit(false), paused(false), load_menu_opened(false), save_choice(0), save_count(0), keystates(SDL_GetKeyboardState(NULL))
{
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i] = NULL;
	}
	
	for (int i = 0; i < ROADS_COUNT; i++)
	{
		roads[i] = NULL;
	}

	for (int i = 0; i < ROADS_COUNT * 2; i++)
	{
		roadsides[i] = NULL;
	}

	for (int i = 0; i < TREES_COUNT; i++)
	{
		trees[i] = NULL;
	}
	
	NewGame();
}

Game::~Game()
{
	DeleteSprites();
}

void Game::DeleteSprites()
{
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		if (background_sprites[i] != NULL)
		{
			delete background_sprites[i];
		}
	}

	for (int i = 0; i < ROADS_COUNT; i++)
	{
		if (roads[i] != NULL)
		{
			delete roads[i];
		}
	}

	for (int i = 0; i < ROADS_COUNT * 2; i++)
	{
		if (roadsides[i] != NULL)
		{
			delete roadsides[i];
		}
	}

	for (int i = 0; i < TREES_COUNT; i++)
	{
		if (trees[i] != NULL)
		{
			delete trees[i];
		}
	}
}

void Game::NewGame()
{
	DeleteSprites();

	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i] = new Sprite(&display, 0, i * BACKGROUND_STRIPES_HEIGHT, "./background_stripe.bmp");
	}
	
	for (int i = 0; i < ROADS_COUNT; i++)
	{
		roads[i] = new Road(&display, DEFAULT_ROAD_WIDTH, display.screen->h);
		roads[i]->x = 0;
		roads[i]->y = i * roads[i]->height;
		roadsides[i * 2] = new Sprite(&display, roads[i]->x - ROADSIDE_WIDTH, i * roads[i]->height, "./roadside.bmp");
		roadsides[i * 2 + 1] = new Sprite(&display, roads[i]->x + roads[i]->width, i * roads[i]->height, "./roadside.bmp");
	}

	for (int i = 0; i < TREES_COUNT; i++)
	{
		if (i % 2 == 0) {
			trees[i] = new Sprite(&display, DEFAULT_TREE_POSITION_X, i * display.screen->h / TREES_COUNT, "./tree.bmp");
		}
		else {
			trees[i] = new Sprite(&display, display.screen_width - DEFAULT_TREE_POSITION_X, i * display.screen->h / TREES_COUNT, "./tree.bmp");
		}
	}

	duration = 0;
	score = 0;
	player.Reset();
}

void Game::Pause(double* time_delta)
{
	if (paused) {
		char buffer[50];
		sprintf(buffer, "PAUSED (press p to unpause)");
		*time_delta = 0;
		int start_x = display.screen_width / 2 - (strlen(buffer) * FONT_WIDTH) / 2 - PAUSE_BOX_PADDING;
		int start_y = display.screen_height / 2 - (PAUSE_BOX_HEIGHT * FONT_HEIGHT) / 2 - PAUSE_BOX_PADDING;
		int width = strlen(buffer) * FONT_WIDTH + 2 * PAUSE_BOX_PADDING;
		int height = PAUSE_BOX_HEIGHT * FONT_HEIGHT + 2 * PAUSE_BOX_PADDING;
		display.DrawRectangle(start_x, start_y, width, height, display.RED, display.BLUE);
		display.DrawString(start_x + PAUSE_BOX_PADDING, start_y + PAUSE_BOX_PADDING, buffer);
	}
}

int Game::Run()
{
	double t1=SDL_GetTicks(), t2=0, time_delta=0;
	SDL_Event current_event;
	
	while (!quit) {
		t2 = SDL_GetTicks();
		
		time_delta = (t2 - t1) / 1000;

		DrawSprites();
		DrawScoreboard();
		DrawFullfilledRequirements();
		DrawLegend();
		Pause(&time_delta);
		if (load_menu_opened) DrawSavesList();
		
		duration += time_delta;
		t1 = t2;

		while (SDL_PollEvent(&current_event)) {
			HandleInput(&player, &current_event);
		};
		player.HandleMovement(time_delta);
		
		CountPoints(time_delta);
		CheckCollisions();

		MoveBackground(time_delta);
		display.Draw();
	};
	return 0;
}

void Game::HandleInput(Vehicle* player, SDL_Event* current_event)
{
	if (current_event->type == SDL_QUIT || current_event->key.keysym.sym == SDLK_ESCAPE) {
		quit = true;
	}

	if (load_menu_opened && current_event->type == SDL_KEYDOWN) {
		if (current_event->key.keysym.sym == SDLK_DOWN) {
			if (save_count - 1 > save_choice) {
				save_choice++;
			}
		}
		else if (current_event->key.keysym.sym == SDLK_UP) {
			if (save_choice > 0) {
				save_choice--;
			}
		}
		else if (current_event->key.keysym.sym == 13) {
			Load();
		}
	}

	if (current_event->type == SDL_KEYDOWN) {
		switch (current_event->key.keysym.sym) {
			case SDLK_p:
				if (!load_menu_opened) paused = !paused;
				break;
			case SDLK_n:
				NewGame();
				break;
			case SDLK_s:
				Save();
				break;
			case SDLK_l:
				load_menu_opened = !load_menu_opened;
				paused = load_menu_opened;
				break;
		}
	}
	
	if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) {
		player->direction = NONE;
	}
	else if (keystates[SDL_SCANCODE_LEFT]) {
		player->direction = LEFT;
	}
	else if (keystates[SDL_SCANCODE_RIGHT]) {
		player->direction = RIGHT;
	}
	else {
		player->direction = NONE;
	}

	if (keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN]) {
		player->acceleration = NO_ACCELERATION;
	}
	else if (keystates[SDL_SCANCODE_UP]) {
		player->acceleration = ACCELERATE;
	}
	else if (keystates[SDL_SCANCODE_DOWN]) {
		player->acceleration = DECELERATE;
	}
	else {
		player->acceleration = NO_ACCELERATION;
	}
}

void Game::DrawScoreboard() {
	char buffer[50];
	int line = 0;
	display.DrawRectangle(1, 1, display.screen_width - SCOREBOARD_PADDING, SCOREBOARD_HEIGHT * FONT_HEIGHT, display.RED, display.BLUE);
	sprintf(buffer, "Jerzy Szyjut, 193064");
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * FONT_WIDTH) / 2, FONT_HEIGHT * line++ + SCOREBOARD_PADDING, buffer);
	sprintf(buffer, "Score: %d", (int)score);
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * FONT_WIDTH) / 2, FONT_HEIGHT * line++ + SCOREBOARD_PADDING, buffer);
	sprintf(buffer, "Duration: %d seconds", (int)duration);
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * FONT_WIDTH) / 2, FONT_HEIGHT * line++ + SCOREBOARD_PADDING, buffer);
}

void Game::DrawLegend()
{
	int line = LEGEND_HEIGHT;
	int start_x = 1;
	int start_y = display.screen_height - FONT_HEIGHT * line - LEGEND_PADDING;
	int width = LEGEND_WIDTH * FONT_WIDTH;
	int height = line * FONT_HEIGHT;
	display.DrawRectangle(start_x, start_y, width, height, display.RED, display.BLUE);
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "Controls:");
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "\32/\33 - left/right");
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "\30/\31 - speed up/down");
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "n - new game");
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "esc - quit");
	display.DrawString(start_x + LEGEND_PADDING, display.screen_height - FONT_HEIGHT * line--, "p - pause");
}

void Game::DrawFullfilledRequirements()
{
	int start_x = display.screen_width - REQUIREMENTS_WIDTH * FONT_WIDTH - REQUIREMENTS_PADDING;
	int start_y = display.screen_height - FONT_HEIGHT * REQUIREMENTS_HEIGHT - REQUIREMENTS_PADDING;
	int width = REQUIREMENTS_WIDTH * FONT_WIDTH;
	int height = REQUIREMENTS_HEIGHT * FONT_HEIGHT;
	display.DrawRectangle(start_x, start_y, width, height, display.RED, display.BLUE);
	display.DrawString(start_x + REQUIREMENTS_PADDING, start_y + REQUIREMENTS_PADDING, "a,b,c,d,e,f,h,i");
}

void::Game::DrawSprites() {
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i]->Draw();
	}
	for (int i = 0; i < ROADS_COUNT; i++)
	{
		roads[i]->Draw();
	}
	for (int i = 0; i < ROADS_COUNT * 2; i++)
	{
		roadsides[i]->Draw();
	}
	for (int i = 0; i < TREES_COUNT; i++)
	{
		trees[i]->Draw();
	}

	player.Draw();
}

void Game::MoveBackground(double time_delta)
{
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i]->y += player.y_velocity * time_delta;
		if (background_sprites[i]->y > display.screen_height)
		{
			background_sprites[i]->y = background_sprites[i]->y - background_sprites[i]->surface->h - display.screen_height;
		}
	}
	for (int i = 0; i < ROADS_COUNT; i++)
	{
		roads[i]->y += player.y_velocity * time_delta;
		if (roads[i]->y > display.screen_height)
		{
			int next_road_index = i + 1;
			if (next_road_index >= ROADS_COUNT) {
				next_road_index = 0;
			}
			roads[i]->ChangeSize(roads[next_road_index]);
			roads[i]->y = roads[i]->y - roads[i]->height - display.screen_height;
		}
		roadsides[i * 2]->y = roads[i]->y;
		roadsides[i * 2]->x = roads[i]->x - ROADSIDE_WIDTH;
		roadsides[i * 2 + 1]->y = roads[i]->y;
		roadsides[i * 2 + 1]->x = roads[i]->x + roads[i]->width;
	}
	for (int i = 0; i < TREES_COUNT; i++)
	{
		trees[i]->y += player.y_velocity * time_delta;
		if (trees[i]->y > display.screen_height)
		{
			trees[i]->y = trees[i]->y - trees[i]->surface->h - display.screen_height;
			if (rand() % 2 == 0) {
				trees[i]->x = DEFAULT_TREE_POSITION_X + rand() % trees[i]->surface->w - trees[i]->surface->w / 2;
			}
			else {
				trees[i]->x = display.screen_width - DEFAULT_TREE_POSITION_X + rand() % trees[i]->surface->w - trees[i]->surface->w / 2;
			}
		}
	}
}

bool Game::CheckCollision(Sprite** array, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (player.Collides(array[i]))
		{
			return true;
		}
	}
	return false;
}

void Game::CountPoints(double time_delta)
{
	bool collides_with_road = CheckCollision((Sprite**)roads, ROADS_COUNT);
	bool collides_with_roadside = CheckCollision(roadsides, ROADS_COUNT * 2);
	
	if (collides_with_road && !collides_with_roadside)
	{
		score += player.y_velocity * time_delta;
	}
}

void Game::CheckCollisions()
{
	bool collides_with_road = CheckCollision((Sprite**)roads, ROADS_COUNT);
	bool collides_with_roadside = CheckCollision(roadsides, ROADS_COUNT * 2);
	
	if (!(collides_with_roadside || collides_with_road))
	{
		player.x = SCREEN_WIDTH / 2 - player.surface->w / 2;
	}
}

void Game::Save()
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	char buffer[100];

	sprintf(buffer, "%s/%s.txt", SAVES_DIRECTORY, asctime(timeinfo));
	for (int i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] == ' ' || buffer[i] == '\n')
		{
			buffer[i] = '_';
		}
		if (buffer[i] == ':')
		{
			buffer[i] = '-';
		}
	}

	FILE* file;
	fopen_s(&file, buffer, "wb");
	fwrite(&score, sizeof(score), 1, file);
	fwrite(&duration, sizeof(duration), 1, file);
	
	player.Save(file);
	
	for (int i = 0; i < ROADS_COUNT; i++)
	{
		roads[i]->Save(file);
	}

	for (int i = 0; i < TREES_COUNT; i++)
	{
		trees[i]->Save(file);
	}
	
	fclose(file);
}

void Game::Load()
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[512], temp[MAX_PATH];
	TCHAR NPath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, NPath); // Get current directory (as w_char)

	wcstombs(temp, NPath, MAX_PATH); // Convert from w_char to char
	sprintf(sPath, "%s\\%s\\*.*", temp, SAVES_DIRECTORY); // Create a string with our desired file path
	mbstowcs(NPath, sPath, MAX_PATH); // Convert back from char to w_char

	hFind = FindFirstFile(NPath, &fdFile);
	int count = 0;
	do
	{
		if (strcmp((const char*)fdFile.cFileName, ".") != 0 && strcmp((const char*)fdFile.cFileName, "..") != 0) // First to files are always . and ..
		{
			if (count == save_choice) {
				char buffer[MAX_PATH];
				wcstombs(temp, fdFile.cFileName, MAX_PATH);
				sprintf(buffer, "%s/%s", SAVES_DIRECTORY, temp);
				FILE* file;
				fopen_s(&file, buffer, "rb");
				fread(&score, sizeof(score), 1, file);
				fread(&duration, sizeof(duration), 1, file);

				player.Load(file);

				for (int i = 0; i < ROADS_COUNT; i++)
				{
					roads[i]->Load(file);
				}

				for (int i = 0; i < TREES_COUNT; i++)
				{
					trees[i]->Load(file);
				}

				fclose(file);
			}
			load_menu_opened = false;
			count++;
		}
	} while (FindNextFile(hFind, &fdFile)); // Count files
}

void Game::DrawSavesList()
{
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[512], temp[MAX_PATH];
	TCHAR NPath[MAX_PATH];
	
	GetCurrentDirectory(MAX_PATH, NPath); // Get current directory (as w_char)

	wcstombs(temp, NPath, MAX_PATH); // Convert from w_char to char
	sprintf(sPath, "%s\\%s\\*.*", temp, SAVES_DIRECTORY); // Create a string with our desired file path
	mbstowcs(NPath, sPath, MAX_PATH); // Convert back from char to w_char
	
	display.DrawRectangle(0, 0, display.screen_width, display.screen_height, display.RED, display.BLUE);

	hFind = FindFirstFile(NPath, &fdFile);

	int i = 0, offset = 0;
	do
	{
		if (strcmp((const char *)fdFile.cFileName, ".") != 0
			&& strcmp((const char*)fdFile.cFileName, "..") != 0) // First to files are always . and ..
		{
			wcstombs(temp, fdFile.cFileName, MAX_PATH);
			if (save_choice == i)
			{
				offset = FONT_WIDTH + SAVES_LIST_PADDING * 2;
				display.DrawString(SAVES_LIST_PADDING, SAVES_LIST_PADDING + i * (FONT_HEIGHT + SAVES_LIST_PADDING), "\20");
			}
			display.DrawString(offset + SAVES_LIST_PADDING, SAVES_LIST_PADDING + i * (FONT_HEIGHT + SAVES_LIST_PADDING), temp);
			offset = 0;
			i++;
		}
	} while (FindNextFile(hFind, &fdFile)); // Count files
	save_count = i;
		
	FindClose(hFind);
}
