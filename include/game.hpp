#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_TTF.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <deque>
#include "agent.hpp"

class Game{
public:
	Game();
	~Game();

	void init(const char *title, int width, int height, bool fullscreen, bool debug);
	void handleEvents();
	void update();
	void checkPause();
	void render();
	void clean();
	void run();
	void createSurfaces();
	void updateRect(SDL_Rect *rect, int x, int y, int w, int h);
	bool running(){ return isRunning; }
	void log(const char *message);
	void startScreen(double blinkRate);

	void addEnemy(float rate);
	void updateEnemies(int speed);
	void removeEnemies();

	SDL_Texture *generateTexture(const char *filename);
	SDL_Texture *generateFont(const char *filename, int fontSize, const char *text, SDL_Color color);

private:
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	clock_t lastEnemyTime = clock();
	double lastBlink = time(NULL);

	int width; 
	int height; 
	bool isRunning, debug;
	const char *title;
	Agent *player; 
	std::deque<Triangle *> enemies;

	SDL_Window *window; 
	SDL_Renderer *renderer; 
	SDL_Texture *spaceTex1, *spaceTex2, *messageTex, *instructionTex;
	SDL_Rect spaceRect1, spaceRect2, messageRect, instructionRect; 
};

#endif
