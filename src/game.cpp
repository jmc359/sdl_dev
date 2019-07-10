#include "game.hpp"

Game::Game(){}
Game::~Game(){}

// Initialize gaming window
void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = 0;
    if (fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems Initialized" << std::endl;
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window){
            std::cout << "Window Created" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer Created" << std::endl;
        }
        isRunning = true;
    }
    else{
        isRunning = false;
    }
    SDL_Surface *tmpSurface = IMG_Load("../assets/character.png");
    playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

// Take care of events while game engine is running
void Game::handleEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

// Update
void Game::update(){
    cnt++;
    destR.h = 192;
    destR.w = 256;
}

// Add objects to renderer
void Game::render(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, playerTex, NULL, &destR);
    SDL_RenderPresent(renderer);
}

// Clean after game exit
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

