#pragma once

#include "Display.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include <cstdio>
}

class Sprite
{
	public:
		Display* display;
		double x, y;
		SDL_Surface* surface;

		Sprite(Display*, double, double, char*);
		~Sprite();
		void Draw();
		void Save(FILE*);
		void Load(FILE*);
		bool Collides(Sprite*);
};

