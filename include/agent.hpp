#ifndef agent_hpp
#define agent_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_TTF.h>
#include <iostream>
#include <deque>

class Agent{
public:
	Agent();
	~Agent();
	void init(const char *assetFile, int xPos, int yPos, int width, int height, SDL_Renderer *renderer);
	bool detectCollision(SDL_Rect *r);
	void updatePosition(){};
	void render();
	SDL_Rect rect;

protected:
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	virtual void updatePosition(int screenW, int screenH, int moveRate, const Uint8 *keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2){};
};

class Triangle : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int screenW, int screenH, int moveRate){
		// Move enemy at constant rate
		rect.w = screenW/20;
		rect.h = screenH/10;
		if (rect.x + rect.w > 0){
		    rect.x -= moveRate;
		}
	}
};

class Missile : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int moveRate){
		// Move missile at constant rate
        rect.x += moveRate;
	}
};

class Player : public Agent{
public:
	using Agent::updatePosition;
	virtual void updatePosition(int screenW, int screenH, int moveRate, const Uint8* keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2){
	    // Move character according to keystroke
	    rect.w = screenW/8;
	    rect.h = screenH/4;
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
	    if (keystate[SDL_SCANCODE_SPACE]){
	    	Missile *missile = new Missile();
	    	missile->init("../assets/missile.png", rect.x + 3 * rect.w/4, rect.y + rect.h/2, rect.w/5, rect.w/5, renderer);
	    	missiles.push_back(missile);
	    }
	}

	void updateMissiles(int speed, int width, std::deque<Triangle *>& enemies){
		// update missiles, accounting for collisions
	    for (auto it = missiles.begin(); it != missiles.end(); ){
	    	bool collision = false;
	    	for (auto it1 = enemies.begin(); it1 != enemies.end(); ){
	    		if((*it)->detectCollision(&((*it1)->rect))){
	    			collision = true;
		            missiles.erase(it++);
		            enemies.erase(it1++);
		            break;
		        }
		        else{
			        ++it1;
		        }
	    	}
	    	if (not collision){
	    		(*it++)->updatePosition(speed);
	    	}
	    }
	}

	void removeMissiles(int width){
		// pop missiles that go offscreen
	    while(missiles.size() > 0 && missiles.front()->rect.x >= width){
	        missiles.pop_front();
	    }
	}

	void renderMissiles(){
		;// render missiles
		for (auto it = missiles.begin(); it != missiles.end(); it++){
			(*it)->render();
		}
	}

protected:
	std::deque<Missile *> missiles;
	int max_missiles = 10;

};

#endif