#pragma once

#include "Sprite.h"
#include "Display.h"

Sprite::Sprite(Display* _display, double _x, double _y, char* _bmp_path) : display(_display), x(_x), y(_y)
{
	surface = SDL_LoadBMP(_bmp_path);
}

Sprite::~Sprite()
{
	SDL_FreeSurface(surface);
}

void Sprite::Draw()
{
	display->DrawSurface(surface, x, y);
}

bool Sprite::Collides(Sprite* _sprite)
{
	if (x < _sprite->x + _sprite->surface->w &&
		x + surface->w > _sprite->x &&
		y < _sprite->y + _sprite->surface->h &&
		y + surface->h > _sprite->y)
	{
		return true;
	}
	return false;
}

void Sprite::Save(FILE* file)
{
	fwrite(&x, sizeof(x), 1, file);
	fwrite(&y, sizeof(y), 1, file);
}

void Sprite::Load(FILE* file)
{
	fread(&x, sizeof(x), 1, file);
	fread(&y, sizeof(y), 1, file);
}
