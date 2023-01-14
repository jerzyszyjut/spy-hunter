#include "Vehicle.h"
#include "Sprite.h"
#include "Display.h"

Vehicle::Vehicle(Display* _display) : Sprite(_display, 0, 0, "./auto.bmp"), x_velocity(DEFAULT_X_VELOCITY), y_velocity(DEFAULT_Y_VELOCITY), direction(NONE), acceleration(NO_ACCELERATION), y_velocity_factor(DEFAULT_Y_VELOCITY_FACTOR)
{
	x = _display->screen_width / 2 - surface->w / 2;
	y = _display->screen_height / 2 - surface->h / 2;
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
	
	double y_offset = y_velocity_factor * DEFAULT_Y_OFFSET;

	y = display->screen_height / 2 - y_offset + (DEFAULT_Y_OFFSET / 2);
}
