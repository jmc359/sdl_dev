#include "game.hpp"
#include "agent.hpp"

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

    // initialize player
    player = new Agent();
    player->init("../assets/character.png", 0, 0, 256, 192, renderer);

    // set width, height for window
    this->width = width;
    this->height = height;

    createSurfaces();
    startScreen(0.5);
}

// Render start screen
void Game::startScreen(double blinkRate){
    bool blink = false;
    while (!keystate[SDL_SCANCODE_RETURN] && isRunning){
        int delay = time(NULL) - lastBlink;
        if (delay > blinkRate){
            blink = !blink;
            lastBlink = time(NULL);
        }
        handleEvents();
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, spaceTex, NULL, NULL);
        SDL_RenderCopy(renderer, messageTex, NULL, &messageRect);
        if (blink){
            SDL_RenderCopy(renderer, instructionTex, NULL, &instructionRect);
        }
        SDL_RenderPresent(renderer);
    }
}

// create all background surfaces
void Game::createSurfaces(){
    SDL_Surface *spaceSurface = IMG_Load("../assets/space.png");
    spaceTex = SDL_CreateTextureFromSurface(renderer, spaceSurface);
    SDL_FreeSurface(spaceSurface);

    TTF_Font* bigSans = TTF_OpenFont("../fonts/OpenSans-Regular.ttf", 36); //this opens a font style and sets a size
    SDL_Color Red = {255,0,0,255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* messageSurface = TTF_RenderText_Solid(bigSans, "Space Force", Red); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    messageTex = SDL_CreateTextureFromSurface(renderer, messageSurface); //now you can convert it into a texture
    SDL_FreeSurface(messageSurface);

    TTF_Font* Sans = TTF_OpenFont("../fonts/OpenSans-Regular.ttf", 24); //this opens a font style and sets a size
    SDL_Color White = {255,255,255,255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* instructionSurface = TTF_RenderText_Solid(Sans, "Press 'Return' to Begin", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    instructionTex = SDL_CreateTextureFromSurface(renderer, instructionSurface); //now you can convert it into a texture
    SDL_FreeSurface(instructionSurface);

    messageRect.h = 300;
    messageRect.w = 600;
    messageRect.x = width/2 - messageRect.w/2;
    messageRect.y = height/2 - messageRect.h;

    instructionRect.h = 100;
    instructionRect.w = 400;
    instructionRect.x = messageRect.x + messageRect.w/8;
    instructionRect.y = messageRect.y + messageRect.h;
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

// Move agents appropriately
void Game::update(){
    player->updatePosition(width, height, keystate);
    addEnemy(0.5);
    updateEnemies();
    removeEnemies();
}

// Add objects to renderer
void Game::render(){
    // render layer by layer
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spaceTex, NULL, NULL);
    player->render();
    for (auto it = enemies.begin(); it != enemies.end(); it++){
        (*it)->render();
    }
    SDL_RenderPresent(renderer);
}

// Clean after game exit
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

// populate deque with enemy after 'rate' seconds
void Game::addEnemy(double rate){
    int delay = time(NULL) - lastEnemyTime;
    if (delay > rate){
        lastEnemyTime = time(NULL);
        srand(rand());
        Triangle *enemy = new Triangle();
        enemy->init("../assets/triangle.png", width, rand() % (height-100), 100, 100, renderer);
        enemies.push_back(enemy);
    }
}

// pop enemy from deque
void Game::removeEnemies(){
    while(enemies.size() > 0 && enemies.front()->rect.x + enemies.front()->rect.w < 0){
        enemies.pop_front();
    }
}

// update enemy positions
void Game::updateEnemies(){
    for (auto it = enemies.begin(); it != enemies.end(); it++){
        (*it)->rect.x -= 3;
    }
}

