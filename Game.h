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

#define ROADS_COUNT 12
#define ROAD_OFFSET 10

#define ROADSIDE_WIDTH 80

#define SCOREBOARD_PADDING 4
#define SCOREBOARD_HEIGHT 4

#define LEGEND_WIDTH 20
#define LEGEND_HEIGHT 7
#define LEGEND_PADDING 4

#define REQUIREMENTS_WIDTH 20
#define REQUIREMENTS_HEIGHT 2
#define REQUIREMENTS_PADDING 4

#define PAUSE_BOX_HEIGHT 1
#define PAUSE_BOX_PADDING 4

#define DEFAULT_TREE_POSITION_X 75
#define TREES_COUNT 6

#define SAVES_LIST_PADDING 4
#define SAVES_DIRECTORY "saves"

class Game
{
	public:
		Display display;
		SDL_Event event;
		Sprite* background_sprites[BACKGROUND_STRIPES_COUNT];
		Sprite* roadsides[ROADS_COUNT * 2];
		Sprite* trees[TREES_COUNT];
		Road* roads[ROADS_COUNT];
		Vehicle player;
		const Uint8* keystates;
		double duration, score;
		bool quit, paused, load_menu_opened;
		int save_choice, save_count;

		Game();
		virtual ~Game();
		void DeleteSprites();
		int Run();
		void HandleInput(Vehicle*, SDL_Event*);
		void DrawScoreboard();
		void DrawLegend();
		void DrawFullfilledRequirements();
		void DrawSprites();
		void MoveBackground(double);
		void NewGame();
		void Pause(double *);
		void CountPoints(double);
		bool CheckCollision(Sprite**, int);
		void CheckCollisions();
		void Save();
		void Load();
		void DrawSavesList();
};
