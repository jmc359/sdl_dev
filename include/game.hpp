#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_TTF.h>
#include <iostream>
#include "agent.hpp"

class Game{
public:
	Game();
	~Game();
	void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	void createSurfaces();
	bool running(){ return isRunning; }

private:
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	int width; 
	int height; 
	bool isRunning;
	Agent *player; 
	Triangle *enemy;

	SDL_Window *window; 
	SDL_Renderer *renderer; 
	SDL_Texture *spaceTex, *messageTex;
	SDL_Rect messageRect; 
};

#endif
