#include "Game.h"
#include "Display.h"
#include "Sprite.h"
#include "Vehicle.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

Game::Game() : display(Display()), player(Vehicle(&display))
{
	quit = false;
	keystates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i] = NULL;
	}
	
	NewGame();
}

Game::~Game()
{
}

void Game::NewGame()
{
	for (int i = 0; i < SPRITES_COUNT; i++)
	{
		sprites[i] = NULL;
	}

	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		if (background_sprites[i] != NULL) {
			SDL_FreeSurface(background_sprites[i]->surface);
		}
		background_sprites[i] = new Sprite(&display, 0, i * 90, "./background_stripe.bmp");
	}

	duration = 0;
	player = Vehicle(&display);
	sprites[0] = &player;
}

int Game::Run()
{
	double t1=SDL_GetTicks(), t2=0;
	SDL_Event current_event;
	
	while (!quit) {
		t2 = SDL_GetTicks();
		double time_delta = (t2 - t1) / 1000;
		duration += time_delta;
		t1 = t2;

		while (SDL_PollEvent(&current_event)) {
			HandleInput(&player, &current_event);

			if (current_event.type == SDL_KEYDOWN && current_event.key.keysym.sym == SDLK_n) {
				NewGame();
			}
		};
		player.HandleMovement(time_delta);

		DrawSprites();
		DrawScoreboard();

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
	display.DrawRectangle(1, 1, display.screen_width - 2, 50, display.RED, display.BLUE);
	sprintf(buffer, "Jerzy Szyjut, 193064");
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * 8) / 2, 8, buffer);
	sprintf(buffer, "Score: %d", (int)player.score);
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * 8) / 2, 16, buffer);
	sprintf(buffer, "Duration: %d seconds", (int)duration);
	display.DrawString(display.screen_width / 2 - (strlen(buffer) * 8) / 2, 24, buffer);
}

void::Game::DrawSprites() {
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i]->Draw();
	}
	for (int i = 0; i < SPRITES_COUNT; i++)
	{
		if (sprites[i] != NULL)
		{
			sprites[i]->Draw();
		}
	}
}

void Game::MoveBackground(double time_delta)
{
	for (int i = 0; i < BACKGROUND_STRIPES_COUNT; i++)
	{
		background_sprites[i]->y += player.y_velocity * time_delta;
		if (background_sprites[i]->y > display.screen_height)
		{
			background_sprites[i]->y = 1 - background_sprites[i]->surface->h;
		}
	}
}


