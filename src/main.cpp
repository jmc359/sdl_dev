#include "game.hpp"

// main function for game
int main(void){
	Game *game = new Game();
	game->init("Nebula Attack", 1080, 600, false, true);
	game->run();
	return 0;
}
