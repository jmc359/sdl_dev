#ifndef agent_hpp
#define agent_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_TTF.h>
#include <iostream>

class Agent{
public:
	Agent();
	~Agent();
	void init(const char *assetFile, int xPos, int yPos, int width, int height, SDL_Renderer *renderer);
	virtual void updatePosition(int screenW, int screenH, const Uint8 *keystate);
	void render();

private:
	int moveRate = 5;
	SDL_Rect rect;
	const char *assetFile;
	SDL_Texture *texture; 
	SDL_Renderer *renderer;
};

#endif