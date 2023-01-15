#include "Display.h"
#include <stdio.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

Display::Display()
{
	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
	}

	screen = SDL_CreateRGBSurface(0, screen_width, screen_height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	int rc = SDL_CreateWindowAndRenderer(screen_width, screen_height, 0, &window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, WINDOW_TITLE);

	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
	};
	SDL_SetColorKey(charset, true, 0x000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);


	BLACK = SDL_MapRGB(screen->format, 0, 0, 0);
	RED = SDL_MapRGB(screen->format, 255, 0, 0);
	GREEN = SDL_MapRGB(screen->format, 0, 255, 0);
	WHITE = SDL_MapRGB(screen->format, 255, 255, 255);
	BLUE = SDL_MapRGB(screen->format, 0, 0, 255);
	DARK_GREEN = SDL_MapRGB(screen->format, 0, 128, 0);
	
	SDL_ShowCursor(SDL_DISABLE);
	SDL_FillRect(screen, NULL, GREEN);
}

Display::~Display()
{
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Display::Draw()
{
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
}

void Display::DrawString(int x, int y, const char* text) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = FONT_WIDTH;
	s.h = FONT_HEIGHT;
	d.w = FONT_WIDTH;
	d.h = FONT_HEIGHT;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * FONT_WIDTH;
		py = (c / 16) * FONT_HEIGHT;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += FONT_WIDTH;
		text++;
	};
};

void Display::DrawSurface(SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void Display::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

void Display::DrawLine(int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void Display::DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(x, y, k, 0, 1, outlineColor);
	DrawLine(x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(x, y, l, 1, 0, outlineColor);
	DrawLine(x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++) {
		DrawLine(x + 1, i, l - 2, 1, 0, fillColor);
	}
};
