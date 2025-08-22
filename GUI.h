#pragma once

typedef struct {
    SDL_FRect* rectangle;
    SDL_Texture* texture;
    std::string function;
}Button;
Button* startButton;

typedef struct {
    SDL_FRect* rectangle;
    const char* data;
    bool active;
} TextBox;

int getSize(const char* text) {
    char a;
    int i = 0;
    a = text[0];
    while (a != '\0') {
        i++; a = text[i];
    }
    return i;
}

void startInput(SDL_Window* window, TextBox* textbox) {
    SDL_Rect rect = {
        (int)textbox->rectangle->x,
        (int)textbox->rectangle->y,
        (int)textbox->rectangle->w,
        (int)textbox->rectangle->h
    };
    SDL_SetTextInputArea(window, &rect, getSize(textbox->data));
    SDL_StartTextInput(window);
    textbox->active = true;
}

void stopInput(SDL_Window* window, TextBox* textbox) {
    SDL_StopTextInput(window);
    textbox->active = false;
}

void drawTextBox(SDL_Renderer* renderer, TextBox* text) {//AND TO THAT
    // draw box outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float x = text->rectangle->x;
    float y = text->rectangle->y;
    float w = text->rectangle->w;
    float h = text->rectangle->h;

    SDL_RenderLine(renderer, x, y, x + w, y);     // top
    SDL_RenderLine(renderer, x, y, x, y + h); // left
    SDL_RenderLine(renderer, x + w, y, x + w, y + h); // right
    SDL_RenderLine(renderer, x, y + h, x + w, y + h); // bottom

    // render typed text if any
    if (!(text->data[0] == '\0')) {
        SDL_Color color = { 255, 255, 255, 255 }; // white text
        SDL_RenderDebugText(renderer, x, y, text->data);
        
    }
}
/*ILL GET BACK TO IT LATER

void handleTextEvent(TextBox* textbox, SDL_Event* e) {
    if (!textbox->active) return;

    if (e->type == SDL_EVENT_TEXT_INPUT) {
        textbox->data += e->text.text; // append typed chars
    }
    else if (e->type == SDL_EVENT_KEY_DOWN) {
        if (e->key.key == SDLK_BACKSPACE && !textbox->data.empty()) {
            textbox->data.pop_back();
        }
    }
}
*/
TextBox* createTextBox(float x, float y, float w, float h) {
    SDL_FRect* rect = (SDL_FRect*)malloc(sizeof(SDL_FRect));
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    TextBox* text = (TextBox*)malloc(sizeof(TextBox));
    text->rectangle = rect;
    return text;
}


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
   // TextBox* textbox = createTextBox(20, 20, 100, 20);
    const Uint32 frameDelay = 17; // 60 FPS

    while (running) {
        Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
                destroyWindow(renderer, window);
            }
            if (e.type == SDL_EVENT_TEXT_INPUT || e.type == SDL_EVENT_KEY_DOWN) {
                //handleTextEvent(textbox, &e); //AND TO THAT
            }
            //handle events
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                //if (checkIfInside(textbox->rectangle, mouseX, mouseY)) {
                //    startInput(window, textbox);
                //}
                //else {
                //    stopInput(window, textbox);
                //}
                int button = handleButtons(mouseX, mouseY);
                
                switch (button) {
                case 1: 
                    return 12;
                    break;
                case 2:

                    break;
                case 3:
                    break;

                }

            }

        }

        const bool* keystate = SDL_GetKeyboardState(nullptr);
        //handle keyboard output


        SDL_SetRenderDrawColor(renderer, 30, 0, 40, 255);
        SDL_RenderFillRect(renderer, background);//background


        //render:
        drawButton(renderer, startButton);
        //drawTextBox(renderer, textbox);
        SDL_RenderPresent(renderer);

        Uint64 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}