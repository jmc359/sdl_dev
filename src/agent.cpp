#include "agent.hpp"

Agent::Agent(){}
Agent::~Agent(){}

void Agent::init(const char *assetFile, int xPos, int yPos, int width, int height, SDL_Renderer *renderer){
    this->renderer = renderer;
    
    rect.x = xPos;
    rect.y = yPos;
    rect.w = width;
    rect.h = height;

    SDL_Surface *surface = IMG_Load(assetFile);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Agent::render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

bool Agent::detectCollision(SDL_Rect *r){
    int width1 = rect.x + rect.w;
    int height1 = rect.y + rect.h;
    int width2 = r->x + r->w;
    int height2 = r->y + r->h;

    if (rect.x < width2 and r->x < width1 and rect.y < height2 and r->y < height1){
        return true;
    }
    return false;
}