#include "Vehicle.h"
#include "Sprite.h"
#include "Display.h"

#define MIN_Y_VELOCITY_FACTOR 0.1
#define MAX_Y_VELOCITY_FACTOR 1.0
#define DEFAULT_Y_VELOCITY_FACTOR 0.5
#define DEFAULT_Y_VELOCITY 500
#define DEFAULT_Y_OFFSET 200
#define DEFAULT_Y_ACCELERATION 0.01
#define DEFAULT_X_VELOCITY 1000.0

Vehicle::Vehicle(Display* _display) : Sprite(_display, 100, 100, "./auto.bmp"), x_velocity(DEFAULT_X_VELOCITY), y_velocity(DEFAULT_Y_VELOCITY), direction(NONE), acceleration(NO_ACCELERATION), y_velocity_factor(DEFAULT_Y_VELOCITY_FACTOR), score(0)
{
	x = _display->screen_width / 2;
	y = _display->screen_height / 2;
}

void Vehicle::HandleMovement(double time_delta)
{ 
	switch (direction) {
		case LEFT:
			x -= x_velocity * time_delta;
			break;
		case RIGHT:
			x += x_velocity * time_delta;
			break;
		case NONE:
			break;
	};

	switch (acceleration)
	{
		case ACCELERATE:
			if (y_velocity_factor < MAX_Y_VELOCITY_FACTOR) y_velocity_factor += (DEFAULT_Y_ACCELERATION * time_delta) * DEFAULT_Y_VELOCITY;
			break;
		case DECELERATE:
			if (y_velocity_factor > MIN_Y_VELOCITY_FACTOR) y_velocity_factor -= (DEFAULT_Y_ACCELERATION * time_delta) * DEFAULT_Y_VELOCITY;
			break;
		case NO_ACCELERATION:
			break;
	}
	y_velocity = DEFAULT_Y_VELOCITY * y_velocity_factor;
	score += y_velocity * time_delta;
	
	double y_offset = y_velocity_factor * DEFAULT_Y_OFFSET;

	y = display->screen_height / 2 - y_offset + (DEFAULT_Y_OFFSET / 2);
}
