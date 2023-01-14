#define _USE_MATH_DEFINES
#include "Game.h"

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char **argv) {
	Game game = Game();

	return game.Run();
};
