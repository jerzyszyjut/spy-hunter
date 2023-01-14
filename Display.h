#pragma once

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	960
#define SCREEN_HEIGHT	540
#define FONT_HEIGHT 8
#define FONT_WIDTH 8

class Display
{
	public:
		SDL_Surface* screen, * charset;
		SDL_Texture* scrtex;
		SDL_Window* window;
		SDL_Renderer* renderer;
		int screen_width, screen_height;
		Uint32 BLACK, RED, GREEN, WHITE, BLUE, DARK_GREEN;

		Display();
		virtual ~Display();
		void Draw();
		void DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
		void DrawString(int x, int y, const char* text);
		void DrawSurface(SDL_Surface* sprite, int x, int y);
		void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
		void DrawLine(int x, int y, int l, int dx, int dy, Uint32 color);
};

