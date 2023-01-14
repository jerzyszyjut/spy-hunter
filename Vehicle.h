#pragma once

#include "Sprite.h"
#include "Display.h"

enum movement_direction
{
	LEFT,
	RIGHT,
	NONE,
};

enum acceleration_direction {
	ACCELERATE,
	DECELERATE,
	NO_ACCELERATION,
};

class Vehicle: public Sprite
{
	public:
		double x_velocity, y_velocity;
		int direction, acceleration;
		double score;
		
		Vehicle(Display*);
		void HandleMovement(double);
		
	private:
		double y_velocity_factor;
};

