#pragma once

typedef struct {
    SDL_FRect* rectangle;
    SDL_Texture* texture;
    std::string function;
}Button;
Button* startButton;


bool checkIfInside(SDL_FRect* rect, float x, float y) {
    if ((rect->x < x) && ((rect->x + rect->w) > x) && (rect->y < y) && ((rect->y + rect->h) > y)) return true;
    return false;
}

int handleButtons(float x, float y) {
    if (checkIfInside(startButton->rectangle, x, y)) return 1;
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

void drawButton(SDL_Renderer* renderer, Button* button) {
    SDL_RenderTexture(renderer, button->texture, NULL, button->rectangle);
}

Button* initializeButton(SDL_Renderer* renderer, int h, int w, float x, float y, std::string function, const char* texture) {
    Button* ret = (Button*)malloc(sizeof(Button));
    SDL_Texture* tex = IMG_LoadTexture(renderer, texture);
    SDL_FRect* rect = (SDL_FRect*)malloc(sizeof(SDL_FRect));
    rect->h = h; rect->w = w; rect->x = x; rect->y = y;
    ret->function = function;
    ret->rectangle = rect;
    ret->texture = tex;
    return ret;
}

int drawGUIWindow(SDL_Renderer* renderer, int WINDOW_WIDTH, int WINDOW_HEIGHT, bool quit, float mouseX, float mouseY, float displayScale, SDL_Window* window, SDL_Event currentEvent){
    SDL_FRect* background = (SDL_FRect*)malloc(sizeof(SDL_FRect));
    startButton = initializeButton(renderer, 75, 200, 400, 700, "Start", "startButton.png");
    background->h = 1000;
    background->w = 1000;
    background->x = 0;
    background->y = 0;
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
                if (handleButtons(mouseX, mouseY)) return 12;
            }
        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output


        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background


        //render:
        drawButton(renderer, startButton);

        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}