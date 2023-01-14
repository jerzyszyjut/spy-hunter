#pragma once
#include "Display.h"
#include "Sprite.h"
#include "Vehicle.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define BACKGROUND_STRIPES_COUNT 8
#define SPRITES_COUNT 20

class Game
{
	public:
		Display display;
		SDL_Event event;
		bool quit;
		const Uint8* keystates;
		Sprite* sprites[SPRITES_COUNT];
		Sprite* background_sprites[BACKGROUND_STRIPES_COUNT];
		Vehicle player;
		double duration;
		
		Game();
		virtual ~Game();
		int Run();
		void HandleInput(Vehicle*, SDL_Event*);
		void DrawScoreboard();
		void DrawSprites();
		void MoveBackground(double);
		void NewGame();
};


