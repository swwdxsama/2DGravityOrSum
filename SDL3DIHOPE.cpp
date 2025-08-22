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



typedef struct nn {
    struct nn* next;
    struct nn* prev;
    Obj* object;
}objNode;

typedef struct {
    objNode* first;
    objNode* last;
}objList;

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
objList objects;
SDL_Texture* grid;
SDL_FRect* gr[3][3];

objList createObjList() {
    objList ret;
    ret.first = NULL;
    ret.last = NULL;
    return ret;
}

void initializeGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3l; j++) {
            SDL_FRect* rect = (SDL_FRect*)malloc(sizeof(SDL_FRect));
            rect->x = 1000 * (j - 1);
            rect->y = 1000 * (i - 1);
            rect->w = 1000;
            rect->h = 1000;
            gr[j][i] = rect;
        }
    }
}

void drawGrid() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3l; j++) {
            SDL_RenderTexture(renderer, grid, NULL, gr[j][i]);
        }
    }
}

void transformGrid(float dx, float dy) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            gr[j][i]->x += dx;
            gr[j][i]->y += dy;
            if (gr[j][i]->x > 2000) {
                gr[j][i]->x -= 3000;
            }
            if (gr[j][i]->x < -1000) {
                gr[j][i]->x += 3000;
            }
            if (gr[j][i]->y > 2000) {
                gr[j][i]->y -= 3000;
            }
            if (gr[j][i]->y < -1000) {
                gr[j][i]->y += 3000;
            }

        }
    }
}

objNode* createNode(Obj* object) {
    objNode* node = (objNode*)malloc(sizeof(objNode));
    node->object = object;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void addObject(objList* list, objNode* node) {
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    }
    else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }
}

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

void applyGravitationalPullToAllObjects(objNode* first) {//yeah its prolly like n^2 or smth, absolutely atrocious
    objNode* next = first->next;
    while (first != NULL) {
        while (next != NULL) {
            applyGravitationalPull(*first->object, *next->object);
            next = next->next;
        }
        first = first->next;
        if (first == NULL) return;
        next = first->next;
    }
}

void checkAllCollisions(objNode* first) {//its n^2 too, if your computer experiences lag when too many objects, get a gud computah
    objNode* next = first->next;
    while (first != NULL) {
        while (next != NULL) {
            if (detectCollision(*first->object, *next->object)) {
                printf("collision\n");
                resolveCollision(*first->object, *next->object);
            }
            next = next->next;
        }
        first = first->next;
        if (first == NULL) return;
        next = first->next;
    }
}

void handleObjectsWithMain() {
    objNode* main = objects.first;
    if (main == NULL) return;
    while (main != NULL) {
        if (main->object->main) break;
        main = main->next;
    }
    objNode* node = objects.first;
    applyGravitationalPullToAllObjects(node);
    while (node != NULL) {
        applyMovement(*node->object);
        node = node->next;
    }
    node = objects.first;
    checkAllCollisions(node);
    transformGrid(main->object->dx, main->object->dy);
    drawGrid();
    
    while (node != NULL) {
        applyTransformation(*main->object, *node->object);
        drawObject(renderer, *node->object);
        node = node->next;
    }
}

void handleMouseMovement(float dx, float dy) {
    objNode* node = objects.first;
    while (node != NULL) {
        applyMovement(*node->object, dx, dy);
        node = node->next;
    }
}

int main()
{
    background->h = 1000;
    background->w = 1000;
    background->x = 0;
    background->y = 0;
    objects = createObjList();
    Obj* object = createObject(20, 500, 500, -0.5, -0.5, 500000, BLUE, true);
    Obj* object2 = createObject(20, 200, 200, 1, 0, 10000000, RED, false);
    objNode* node = createNode(object);
    objNode* node2 = createNode(object2);
    addObject(&objects, node);
    addObject(&objects, node2);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << '\n';
        return 1;
    }
    initWindow();

    bool running = true;
    SDL_Event e;
    drawGUIWindow(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, quit, mouseX, mouseY, displayScale, window, currentEvent);
    //destroyWindow();
    initWindow();

    const Uint32 frameDelay = 17; // 60 FPS
    grid = IMG_LoadTexture(renderer, "grid.png");
    initializeGrid();
    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
            //handle events
            if (e.type == SDL_EVENT_MOUSE_MOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
                float dx = e.motion.xrel;  // relative movement
                float dy = e.motion.yrel;
                handleMouseMovement(dx, dy);
                transformGrid(dx, dy);
            }

        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output

        
        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background
        
        

        //render:
        handleObjectsWithMain();
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
