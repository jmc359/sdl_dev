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
	virtual void updatePosition(int screenW, int screenH, const Uint8 *keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2){};
	void updatePosition(){};
	void render();
	SDL_Rect rect;

protected:
	int moveRate, width;
	const char *assetFile;
	SDL_Texture *texture; 
	SDL_Renderer *renderer;
};

class Player : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int screenW, int screenH, const Uint8* keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2){
    // Move character according to keystroke
	rect.h = screenH/3;
	rect.w = screenW/4;
    if (keystate[SDL_SCANCODE_LEFT] && rect.x >= 0){
        rect.x -= moveRate;
    }
    if (keystate[SDL_SCANCODE_RIGHT] && rect.x + rect.w/2 <= screenW/2){
        rect.x += moveRate;
    }
    else if (keystate[SDL_SCANCODE_RIGHT]){
        spaceRect1->x -= moveRate;
        spaceRect2->x -= moveRate;
        if (spaceRect1->x + spaceRect1->w <= 0){
            spaceRect1->x = spaceRect1->w;
        }
        if (spaceRect2->x + spaceRect2->w <= 0){
            spaceRect2->x = spaceRect2->w;
        }
    }
    if (keystate[SDL_SCANCODE_UP] && rect.y >= 0){
        rect.y -= moveRate;
    }
    if (keystate[SDL_SCANCODE_DOWN] && rect.y + rect.h <= screenH){
        rect.y += moveRate;
    }
}
};

class Triangle : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int screenW, int screenH){
		rect.w = screenW/10;
		rect.h = screenH/6;
		if (rect.x + rect.w > 0){
	        rect.x -= moveRate;
	    }
	}
};

class Missile : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int width){
		if (rect.x + rect.w < width){
	        rect.x += 10;
	    }
	}
};
#endif