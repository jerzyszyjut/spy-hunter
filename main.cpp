#define _USE_MATH_DEFINES
#include "Game.h"
#include <cstdlib>
#include <time.h>

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char **argv) {
	srand(time(NULL));

	Game game = Game();

	return game.Run();
};
