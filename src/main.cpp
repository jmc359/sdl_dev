#include "game.hpp"

int main(void){
	Game *game = new Game();
	game->init("Nebula Attack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 600, false);
	game->run();
	return 0;
}
