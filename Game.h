#pragma once
#include "Display.h"
#include "Sprite.h"
#include "Vehicle.h"
#include "Road.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define BACKGROUND_STRIPES_COUNT 8
#define BACKGROUND_STRIPES_HEIGHT 90
#define BACKGROUND_STRIPE_OFFSET 5
#define SPRITES_COUNT 20
#define ROADS_COUNT 12
#define ROAD_OFFSET 10
#define ROADSIDE_WIDTH 80
#define SCOREBOARD_PADDING 4
#define SCOREBOARD_HEIGHT 4
#define LEGEND_WIDTH 20
#define LEGEND_HEIGHT 6
#define LEGEND_PADDING 4
#define REQUIREMENTS_WIDTH 15
#define REQUIREMENTS_HEIGHT 2
#define REQUIREMENTS_PADDING 4
#define DEFAULT_TREE_POSITION_X 75
#define TREES_COUNT 6

class Game
{
	public:
		Display display;
		SDL_Event event;
		bool quit;
		const Uint8* keystates;
		Sprite* sprites[SPRITES_COUNT];
		Sprite* background_sprites[BACKGROUND_STRIPES_COUNT];
		Sprite* roadsides[ROADS_COUNT * 2];
		Sprite* trees[TREES_COUNT];
		Road* roads[ROADS_COUNT];
		Vehicle player;
		double duration, score;
		
		Game();
		virtual ~Game();
		int Run();
		void HandleInput(Vehicle*, SDL_Event*);
		void DrawScoreboard();
		void DrawLegend();
		void DrawFullfilledRequirements();
		void DrawSprites();
		void MoveBackground(double);
		void NewGame();
		void CountPoints(double);
		bool CheckCollision(Sprite**, int);
		void CheckCollisions();
};


