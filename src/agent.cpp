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

void Agent::render(){
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
