#pragma once

bool checkIfInside(SDL_FRect* rect, float x, float y) {
    if ((rect->x < x) && ((rect->x + rect->w) > x) && (rect->y < y) && ((rect->y + rect->h) > y)) return true;
    return false;
}

SDL_FRect* startButton;
SDL_Texture* startButtonTexture;

void defStart(SDL_Renderer* renderer, SDL_FRect** start, SDL_Texture** texture) {
    *start = (SDL_FRect*)malloc(sizeof(SDL_FRect));
    (*start)->x = 400;
    (*start)->y = 700;
    (*start)->h = 75;
    (*start)->w = 200;
    *texture = IMG_LoadTexture(renderer, "startButton.png");
}

int handleStartButton(SDL_FRect * rect, float x, float y){
    if (checkIfInside(rect, x, y)) return 1;
    return 0;
}

void destroyWindow(SDL_Renderer* renderer, SDL_Window* window)
{
    ///Destroy renderer
    if (renderer)
        SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    //Destroy window
    if (window)
        SDL_DestroyWindow(window);
    window = nullptr;

    //Quit SDL
    SDL_Quit();
}

void drawButton(SDL_Renderer* renderer, SDL_Texture* texture, SDL_FRect* rect) {
    SDL_RenderTexture(renderer, texture, NULL, rect);
}

int drawGUIWindow(SDL_Renderer* renderer, int WINDOW_WIDTH, int WINDOW_HEIGHT, bool quit, float mouseX, float mouseY, float displayScale, SDL_Window* window, SDL_Event currentEvent){
    SDL_FRect* background = (SDL_FRect*)malloc(sizeof(SDL_FRect));
    background->h = 1000;
    background->w = 1000;
    background->x = 0;
    background->y = 0;
    defStart(renderer, &startButton, &startButtonTexture);
    
    bool running = true;
    SDL_Event e;
    

    const Uint32 frameDelay = 17; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
                destroyWindow(renderer, window);
            }
            //handle events
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                if (handleStartButton(startButton, mouseX, mouseY)) return 12;
            }
        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output


        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background


        //render:
        drawButton(renderer, startButtonTexture, startButton);

        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}