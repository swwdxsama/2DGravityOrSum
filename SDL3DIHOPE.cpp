// SDL3DIHOPE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <iostream>
#include <list>
#include "Objects.h"
#include "GUI.h"



//define window dimensions
constexpr int WINDOW_WIDTH{ 1000 };
constexpr int WINDOW_HEIGHT{ 1000 };
bool quit{ false };
float mouseX{ -1.0f }, mouseY{ -1.0f };
float displayScale{ 1.0f };
//The window
SDL_Window* window{ nullptr };
//The window renderer
SDL_Renderer* renderer{ nullptr };
SDL_Event currentEvent;
SDL_FRect* background = (SDL_FRect*)malloc(sizeof(SDL_FRect));
std::list<Obj> objects;




bool initWindow()
{
    bool success{ true };

    //Try to initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL initialization failed: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Try to create the window and renderer
        displayScale = SDL_GetDisplayContentScale(1);

        if (!SDL_CreateWindowAndRenderer(
            "SDL Hello World Example",
            static_cast<int>(displayScale * WINDOW_WIDTH),
            static_cast<int>(displayScale * WINDOW_HEIGHT),
            SDL_WINDOW_HIGH_PIXEL_DENSITY,
            &window, &renderer))
        {
            SDL_Log("Failed to create window and renderer: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Apply global display scaling to renderer
            SDL_SetRenderScale(renderer, displayScale, displayScale);

            //Set background color
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            //Apply background color
            SDL_RenderClear(renderer);
        }
    }

    return success;
}

void destroyWindow()
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

void handleObjects() {
    Obj* back = &objects.back();
    Obj* front = &objects.front();
    //Obj backCopy = *back;
    //Obj frontCopy = *front;
    //listAdd(back->prevs, backCopy);
    //listAdd(front->prevs, frontCopy);
    applyGravitationalPull(*back, *front);
    applyMovement(*front);
    applyMovement(*back);
    
    if (detectCollision(*back, *front)) {
        printf("collision\n");
        resolveCollision(*back, *front);
    }
    applyTransformation(*back, *front);
    applyTransformation(*back, *back);

    
    drawObject(renderer, *front);
    drawObject(renderer, *back);
}



int main()
{
    background->h = 1000;
    background->w = 1000;
    background->x = 0;
    background->y = 0;
    Obj obj;
    obj.x = 300;
    obj.y = 700;
    obj.dx = 3;
    obj.dy = 3;
    obj.radius = 30;
    obj.color = BLUE;
    obj.mass = 15500000;
    //obj.prevs = createList();
    objects.push_front(obj);
    Obj obj2;
    obj2.x = 700;
    obj2.y = 300;
    obj2.dx = -3;
    obj2.dy = -3;
    obj2.radius = 30;
    obj2.color = RED;
    obj2.mass = 15500000;
    //obj2.prevs = createList();
    objects.push_front(obj2);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << '\n';
        return 1;
    }
    initWindow();

    bool running = true;
    SDL_Event e;
    drawGUIWindow(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, quit, mouseX, mouseY, displayScale, window, currentEvent);
    destroyWindow();
    initWindow();

    const Uint32 frameDelay = 17; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
            //handle events

        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output

        
        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background
        
        

        //render:
        handleObjects();
        system("CLS");
        
        

        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    char inp[256];
    scanf_s("%s", inp);

    return 0;
}
