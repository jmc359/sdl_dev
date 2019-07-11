#include "game.hpp"

Game::Game(){}
Game::~Game(){}

// Initialize gaming window
void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = 0;
    if (fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Initialize window, renderer, colros
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems Initialized" << std::endl;
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window){
            std::cout << "Window Created" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            
        }
        if( TTF_Init() >= 0 )
        {
            std::cout << "TTF Initialized" << std::endl;   
        }
        else{
            std::cout << TTF_GetError() << std::endl;
        }
        isRunning = true;
    }
    else{
        isRunning = false;
    }

    // set width, height for window and player
    this->width = width;
    this->height = height;

    createSurfaces();
}

void Game::createSurfaces(){
    // load assets
    SDL_Surface *playerSurface = IMG_Load("../assets/character.png");
    playerTex = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    SDL_Surface *spaceSurface = IMG_Load("../assets/space.png");
    spaceTex = SDL_CreateTextureFromSurface(renderer, spaceSurface);
    SDL_FreeSurface(spaceSurface);

    TTF_Font* Sans = TTF_OpenFont("../fonts/OpenSans-Regular.ttf", 36); //this opens a font style and sets a size
    SDL_Color Red = {255,0,0,255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* messageSurface = TTF_RenderText_Solid(Sans, "Space Force", Red); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    messageTex = SDL_CreateTextureFromSurface(renderer, messageSurface); //now you can convert it into a texture
    SDL_FreeSurface(messageSurface);

    messageRect.h = 300;
    messageRect.w = 600;
    messageRect.x = width/2 - messageRect.w/2;
    messageRect.y = height/2 - messageRect.h/2;

    playerRect.x = 0;
    playerRect.y = height/2 - playerH/2;
    playerRect.h = playerH;
    playerRect.w = playerW;
}

// Take care of events while game engine is running
void Game::handleEvents(){
    // look for quit
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
    // Move character according to keystroke
    if (keystate[SDL_SCANCODE_LEFT] && playerRect.x > -playerW/2){
        playerRect.x -= moveRate;
    }
    else if (keystate[SDL_SCANCODE_RIGHT] && playerRect.x < width-playerW/4){
        playerRect.x += moveRate;
    }
    else if (keystate[SDL_SCANCODE_UP] && playerRect.y > -playerH/2){
        playerRect.y -= moveRate;
    }
    else if (keystate[SDL_SCANCODE_DOWN] && playerRect.y < height-playerH/4){
        playerRect.y += moveRate;
    }
}

// Add objects to renderer
void Game::render(){
    // render layer by layer
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spaceTex, NULL, NULL);
    SDL_RenderCopy(renderer, messageTex, NULL, &messageRect);
    SDL_RenderCopy(renderer, playerTex, NULL, &playerRect);
    SDL_RenderPresent(renderer);
}

// Clean after game exit
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

