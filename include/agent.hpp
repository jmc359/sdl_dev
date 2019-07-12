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
	void init(const char *assetFile, int xPos, int yPos, int width, int height, SDL_Renderer *renderer, int moveRate);
	virtual void updatePosition(int screenW, int screenH, const Uint8 *keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2);
	void render();
	SDL_Rect rect;

protected:
	int moveRate, width;
	const char *assetFile;
	SDL_Texture *texture; 
	SDL_Renderer *renderer;
};

class Triangle : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(){
		if (rect.x + rect.w > 0){
	        rect.x -= moveRate;
	    }
	}
};

class Missile : public Agent{
	public:
	using Agent::updatePosition;
	void updatePosition(int width){
		if (rect.x + rect.w < width){
	        rect.x += 10;
	    }
	}
};
#endif