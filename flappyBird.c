#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Init Error: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Flappy Puppy",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WIDTH, HEIGHT, 0);

    if (!window) {
        SDL_Log("Window Error: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Renderer Error: %s", SDL_GetError());
        return false;
    }

    return true;
}


int main(int argc, char *argv[]) {
    float gravity = 800.0f;
    SDL_Rect Player = {250, 50, 50, 70};
    float velocityY = 0.0f;
    float PlayerY = 50.0f;

    // Declaring and Initializing based on the accelerating Pipes

    float pipeX = WIDTH;      // horizontal position
    int gap = 150;            // space player passes through
    int pipeTopHeight = 200;  // top pipe height
    int pipeWidth = 80;       // thickness
    float pipeSpeed = 200.0f; // movement speed

    if (!initSDL()) return 1;

    bool running = true;
    bool flap = false;
    bool start = false;
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0) {
                    // WHat then will be the bird state of flap
                    flap = true;
                    start = true;
                }
            }
        }

        int ground = HEIGHT - 50;
        if(start){
            
            if(flap){
                velocityY = - 300.0f;
                flap = false;
            }

            velocityY += gravity * delta_time;
            PlayerY += velocityY * delta_time;  // GRAVITY CAN ONLY ACT ON HEIGHT

           
            if(PlayerY + Player.h >= ground){
              PlayerY = ground - Player.h;
              velocityY = 0.0f;

            }
            Player.y = (int)PlayerY;
    }

            // Background color
            SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect groundRect = {0, ground, WIDTH, 50};
            SDL_RenderFillRect(renderer, &groundRect);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 230);
            SDL_RenderFillRect(renderer, &Player);

            // Render objects here

            SDL_RenderPresent(renderer);

            SDL_Delay(16);
       
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
