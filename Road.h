#pragma once
#include "Sprite.h"

#define DEFAULT_ROAD_WIDTH 400
#define MIN_ROAD_WIDTH 250
#define MAX_ROAD_WIDTH 500
#define ROAD_WIDTH_CHANGE 10
#define ROAD_CHANGE_WIDTH_RESIZING_CHANCE 4

class Road : public Sprite
{
	public:
		int width, height;

		Road(Display*, int, int);
		void Draw();
		void ChangeSize(Road*);
};

