#include "agent.hpp"

Agent::Agent(){}
Agent::~Agent(){}

void Agent::init(const char *assetFile, int xPos, int yPos, int width, int height, SDL_Renderer *renderer, int moveRate){
    this->assetFile = assetFile;
    this->renderer = renderer;
    this->moveRate = moveRate;
    this->width = width;
    
    rect.x = xPos;
    rect.y = yPos;
    rect.w = width;
    rect.h = height;

    SDL_Surface *surface = IMG_Load(assetFile);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Agent::updatePosition(int screenW, int screenH, const Uint8* keystate, SDL_Rect *spaceRect1, SDL_Rect *spaceRect2){
    // Move character according to keystroke
    if (keystate[SDL_SCANCODE_LEFT] && rect.x >= -rect.w/2){
        rect.x -= moveRate;
    }
    if (keystate[SDL_SCANCODE_RIGHT] && rect.x <= screenW/2-rect.w/4){
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
    if (keystate[SDL_SCANCODE_UP] && rect.y >= -rect.h/2){
        rect.y -= moveRate;
    }
    if (keystate[SDL_SCANCODE_DOWN] && rect.y <= screenH-rect.h/4){
        rect.y += moveRate;
    }
}

void Agent::render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
