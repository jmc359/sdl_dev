#include "game.hpp"
#include "agent.hpp"

#define ENEMY_RATE (0.02)
#define ENEMY_SPEED (10)
#define MISSILE_SPEED (5)
#define PLAYER_SPEED (5)

Game::Game(){}
Game::~Game(){}

// Initialize gaming window
void Game::init(const char *title, int width, int height, bool fullscreen, bool logger){
    int flags = SDL_WINDOW_RESIZABLE;
    if(fullscreen){
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // set title, width, height for window, and logger flag
    this->title = title;
    this->width = width;
    this->height = height;
    this->logger = logger;

    // Initialize window, renderer, colors
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        log("Subsystems initialized");
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if(window){
            log("Window created");
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        if(TTF_Init() >= 0){
            log("TTF initialized");   
        }
        else{
            log(TTF_GetError(), "ERROR");
        }
        isRunning = true;
    }
    else{
        isRunning = false;
    }

    // initialize player
    player = new Player();
    player->init("../assets/character.png", 0, 0, width/8, height/4, renderer);

    createSurfaces();
    startScreen(0.5);
}

// Render start screen
void Game::startScreen(double blinkRate){
    bool blink = false;
    while(!keystate[SDL_SCANCODE_RETURN] && isRunning){
        SDL_GetWindowSize(window, &width, &height);
        updateRect(&spaceRect1, 0, 0, width, height);
        updateRect(&spaceRect2, width, 0, width, height);
        updateRect(&messageRect, width/2 - messageRect.w/2, height/2 - messageRect.h, 600, 300);
        updateRect(&instructionRect, messageRect.x + messageRect.w/8, messageRect.y + messageRect.h, 400, 100);
        int delay = time(NULL) - lastBlink;
        if(delay > blinkRate){
            blink = !blink;
            lastBlink = time(NULL);
        }
        handleEvents();
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, spaceTex1, NULL, NULL);
        SDL_RenderCopy(renderer, spaceTex2, NULL, NULL);
        SDL_RenderCopy(renderer, messageTex, NULL, &messageRect);
        if(blink){
            SDL_RenderCopy(renderer, instructionTex, NULL, &instructionRect);
        }
        SDL_RenderPresent(renderer);
    }
}

// create all background surfaces
void Game::createSurfaces(){
    log("Background created");
    spaceTex1 = generateTexture("../assets/space.png");
    spaceTex2 = generateTexture("../assets/space.png");
    messageTex = generateFont("../fonts/OpenSans-Regular.ttf", 36, title, {255,0,0,255});
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
            log("Game quit");
            isRunning = false;
            break;
        default:
            break;
    }
}

// Move agents and textures appropriately
void Game::update(){
    checkPause();
    int w = width;
    int h = height;
    SDL_GetWindowSize(window, &width, &height);
    if(w != width or h != height){
        updateRect(&spaceRect1, 0, 0, width, height);
        updateRect(&spaceRect2, width, 0, width, height);
    }
    else{
        updateRect(&spaceRect1, spaceRect1.x, spaceRect1.y, width, height);
        updateRect(&spaceRect2, spaceRect2.x, spaceRect2.y, width, height);
    }
    player->updatePosition(width, height, PLAYER_SPEED, keystate, &spaceRect1, &spaceRect2);
    addEnemy(ENEMY_RATE);
    updateEnemies(ENEMY_SPEED);
    player->updateMissiles(MISSILE_SPEED, width, enemies);
    player->removeMissiles(width);
    removeEnemies();
}

// Add objects to renderer
void Game::render(){
    // render layer by layer
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, spaceTex1, NULL, &spaceRect1);
    SDL_RenderCopy(renderer, spaceTex2, NULL, &spaceRect2);
    player->render();
    player->renderMissiles();
    renderEnemies();
    SDL_RenderPresent(renderer);
}

// Clean after game exit
void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    log("Game cleaned");
}

void Game::renderEnemies(){
    for(auto it = enemies.begin(); it != enemies.end(); it++){
        (*it)->render();
    }
}

// populate deque with enemy after 'rate' seconds
void Game::addEnemy(float rate){
    float delay = ((float)(clock() - lastEnemyTime)) / CLOCKS_PER_SEC;
    if (delay > rate){
        lastEnemyTime = clock();
        srand(rand());
        Triangle *enemy = new Triangle();
        enemy->init("../assets/triangle.png", width, rand() % (height-100), width/20, height/10, renderer);
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
void Game::updateEnemies(int speed){
    for (auto it = enemies.begin(); it != enemies.end(); ){
        if((*it)->detectCollision(&(player->rect))){
            enemies.erase(it++);
        }
        else{
            (*it)->updatePosition(width, height, speed);
            ++it;
        }
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
    TTF_Font *font = TTF_OpenFont(filename, fontSize); 
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color); 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); 
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

// Initiate pause menu
void Game::checkPause(){
    if (keystate[SDL_SCANCODE_P]){
        log("Game paused");
        SDL_Texture *pauseTex = generateFont("../fonts/OpenSans-Regular.ttf", 36, "Game Paused", {255,0,255,255});
        SDL_Texture *cueTex = generateFont("../fonts/OpenSans-Regular.ttf", 24, "Press 'R' to Resume", {255,255,255,255});
        while(!keystate[SDL_SCANCODE_R] && isRunning){
            handleEvents();
            int w = width;
            int h = height;
            SDL_GetWindowSize(window, &width, &height);

            updateRect(&messageRect, width/2 - messageRect.w/2, height/2 - messageRect.h, 600, 300);
            updateRect(&instructionRect, messageRect.x + messageRect.w/8, messageRect.y + messageRect.h, 400, 100);
            if(w != width or h != height){
                updateRect(&spaceRect1, 0, 0, width, height);
                updateRect(&spaceRect2, width, 0, width, height);
            }
            else{
                updateRect(&spaceRect1, spaceRect1.x, spaceRect1.y, width, height);
                updateRect(&spaceRect2, spaceRect2.x, spaceRect2.y, width, height);
            }
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, spaceTex1, NULL, &spaceRect1);
            SDL_RenderCopy(renderer, spaceTex2, NULL, &spaceRect2);
            player->render();
            for(auto it = enemies.begin(); it != enemies.end(); it++){
                (*it)->render();
            }
            SDL_RenderCopy(renderer, pauseTex, NULL, &messageRect);
            SDL_RenderCopy(renderer, cueTex, NULL, &instructionRect);
            SDL_RenderPresent(renderer);    
        }
        log("Game resumed");
    }
}

// Game running loop
void Game::run(){
    int frameTime;
    uint32_t frameStart;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while(isRunning){
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    clean();
}

void Game::log(const char *message, const char *level){
    if (logger){
        time_t rawtime;
        char buffer[80];
        time (&rawtime);
        struct tm *timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", timeinfo);
        std::cout << "[" << level << "]" << buffer << message << std::endl;
    }
}
