#include "game.hpp"
#include "agent.hpp"

Game::Game(){}
Game::~Game(){}

// Initialize gaming window
void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen){
        flags |= SDL_WINDOW_FULLSCREEN;
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
    player->init("../assets/character.png", 0, 0, 256, 192, renderer, 5);

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
        SDL_GetWindowSize(window, &width, &height);
        updateRect(&spaceRect1, 0, 0, width, height);
        updateRect(&spaceRect2, width, 0, width, height);
        updateRect(&messageRect, width/2 - messageRect.w/2, height/2 - messageRect.h, 600, 300);
        updateRect(&instructionRect, messageRect.x + messageRect.w/8, messageRect.y + messageRect.h, 400, 100);
        int delay = time(NULL) - lastBlink;
        if (delay > blinkRate){
            blink = !blink;
            lastBlink = time(NULL);
        }
        handleEvents();
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, spaceTex1, NULL, NULL);
        SDL_RenderCopy(renderer, spaceTex2, NULL, NULL);
        SDL_RenderCopy(renderer, messageTex, NULL, &messageRect);
        if (blink){
            SDL_RenderCopy(renderer, instructionTex, NULL, &instructionRect);
        }
        SDL_RenderPresent(renderer);
    }
}

// create all background surfaces
void Game::createSurfaces(){
    spaceTex1 = generateTexture("../assets/space.png");
    spaceTex2 = generateTexture("../assets/space.png");
    messageTex = generateFont("../fonts/OpenSans-Regular.ttf", 36, "Space Force", {255,0,0,255});
    instructionTex = generateFont("../fonts/OpenSans-Regular.ttf", 24, "Press 'Return' to Begin", {255,255,255,255});
    updateRect(&messageRect, width/2 - messageRect.w/2, height/2 - messageRect.h, 600, 300);
    updateRect(&instructionRect, messageRect.x + messageRect.w/8, messageRect.y + messageRect.h, 400, 100);
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

// Move agents and textures appropriately
void Game::update(){
    int w = width;
    int h = height;
    SDL_GetWindowSize(window, &width, &height);
    if (w != width or h != height){
        updateRect(&spaceRect1, 0, 0, width, height);
        updateRect(&spaceRect2, width, 0, width, height);
    }
    else{
        updateRect(&spaceRect1, spaceRect1.x, spaceRect1.y, width, height);
        updateRect(&spaceRect2, spaceRect2.x, spaceRect2.y, width, height);
    }
    player->updatePosition(width, height, keystate, &spaceRect1, &spaceRect2);
    addEnemy(0.02, 10);
    updateEnemies();
    removeEnemies();
}

// Add objects to renderer
void Game::render(){
    // render layer by layer
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spaceTex1, NULL, &spaceRect1);
    SDL_RenderCopy(renderer, spaceTex2, NULL, &spaceRect2);
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
    std::cout << "Game Cleaned" << std::endl;
}

// populate deque with enemy after 'rate' seconds
void Game::addEnemy(float rate, int speed){
    float delay = ((float)(clock() - lastEnemyTime)) / CLOCKS_PER_SEC;
    if (delay > rate){
        lastEnemyTime = clock();
        srand(rand());
        Triangle *enemy = new Triangle();
        enemy->init("../assets/triangle.png", width, rand() % (height-100), 100, 100, renderer, speed);
        enemies.push_back(enemy);
    }
}

// pop enemy from deque
void Game::removeEnemies(){
    while(enemies.size() > 0 && enemies.front()->rect.x + enemies.front()->rect.w <= 0){
        enemies.pop_front();
    }
}

// update enemy positions
void Game::updateEnemies(){
    for (auto it = enemies.begin(); it != enemies.end(); it++){
        (*it)->updatePosition();
    }
}

// generate SDL texture given file name
SDL_Texture *Game::generateTexture(const char *filename){
    SDL_Surface *surface = IMG_Load(filename);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// generate SDL font given file name and relevant parameters
SDL_Texture *Game::generateFont(const char *filename, int fontSize, const char *text, SDL_Color color){
    TTF_Font *font = TTF_OpenFont(filename, fontSize); //this opens a font style and sets a size
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); //now you can convert it into a texture
    SDL_FreeSurface(surface);
    return texture;
}

// update SDL_Rect attributes
void Game::updateRect(SDL_Rect *rect, int x, int y, int w, int h){
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

