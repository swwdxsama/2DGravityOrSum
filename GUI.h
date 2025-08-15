#pragma once

bool checkIfInside(SDL_FRect rect, float x, float y) {
    if ((rect.x < x) && ((rect.x + rect.w) > x) && (rect.y < y) && ((rect.y + rect.h) > y)) return true;
    return false;
}

SDL_FRect startButton;

void drawGUIWindow(SDL_Renderer* renderer, int WINDOW_WIDTH, int WINDOW_HEIGHT, bool quit, float mouseX, float mouseY, float displayScale, SDL_Window* window, SDL_Event currentEvent){
    initWindow();

    bool running = true;
    SDL_Event e;

    const Uint32 frameDelay = 17; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
                destroyWindow();
            }
            //handle events
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {

            }
        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output


        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background



        //render:


        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}