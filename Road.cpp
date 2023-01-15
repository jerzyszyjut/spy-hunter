#include "Road.h"

#include <time.h>
#include <cstdlib>

Road::Road(Display* display, int width, int height) : Sprite(display, 0, 0, "road.bmp"), width(width), height(height)
{
	if (height > this->surface->h) {
		this->height = this->surface->h;
	}
	
	if (width > this->surface->w) {
		this->width = this->surface->w;
	}
}

void Road::Draw()
{
	surface->w = width;
	surface->h = height;
	x = display->screen_width / 2 - width / 2;
	
	SDL_Rect src_rect, dest_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = width;
	src_rect.h = height;
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = width;
	dest_rect.h = height;
	SDL_BlitSurface(surface, &src_rect, display->screen, &dest_rect);
}

void Road::ChangeSize(Road* next_road)
{
	if (rand() % ROAD_CHANGE_WIDTH_RESIZING_CHANCE == 0) {
		if (next_road->width > width) {
			if (rand() % ROAD_CHANGE_WIDTH_RESIZING_CHANCE == 0 || next_road->width > MAX_ROAD_WIDTH) {
				width = next_road->width - ROAD_WIDTH_CHANGE;
			}
			else {
				width = next_road->width + ROAD_WIDTH_CHANGE;
			}
		}
		else {
			if (rand() % ROAD_CHANGE_WIDTH_RESIZING_CHANCE == 0 || next_road->width < MIN_ROAD_WIDTH) {
				width = next_road->width + ROAD_WIDTH_CHANGE;
			}
			else {
				width = next_road->width - ROAD_WIDTH_CHANGE;
			}
		}
	}
	else {
		width = next_road->width;
	}
}
