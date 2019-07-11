#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>

class Game{
public:
	Game();
	~Game();
	void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; }

private:
	int moveRate = 5;
	int playerH = 192;
	int playerW = 256;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	int width; 
	int height;
	bool isRunning;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *playerTex;
	SDL_Rect playerRect;
};

#endif
