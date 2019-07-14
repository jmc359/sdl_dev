#include "game.hpp"

int main(void){
	Game *game = new Game();
	game->init("Nebula Attack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 600, false);

	int frameTime;
	uint32_t frameStart;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	while(game->running()){

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime){
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();
	return 0;
}