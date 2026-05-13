#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>

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

TTF_Font *font = NULL;
SDL_Texture *scoreTexture = NULL;
bool TTFinitSDL(){
    if(TTF_Init() == -1){
        printf("TTF not initializing: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("Roboto_Condensed-Regular.ttf", 24);
    if(!font){
        printf("Font not displaying: %s\n", TTF_GetError());
        TTF_Quit();
        return false;
    }
    SDL_Rect rect = {400, 10, 100, 50};
    return true;
}

void TTF_Rendering(const char *text){
   
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, white);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int tw, th;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect dst = {10, 10, tw, th}; 
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    return;
}

bool checkCollision(SDL_Rect A, SDL_Rect B){
    if(A.x + A.w <= B.x) return false;
    if(A.x >= B.x + B.w) return false;

    if(A.y + A.h <= B.y) return false;
    if(A.y >= B.y + B.h) return false;

    return true;
}



int main(int argc, char *argv[]) {

    char scoreText[32];

    float gravity = 800.0f;
    SDL_Rect Player = {250, 50, 50, 70};
    float velocityY = 0.0f;
    float PlayerY = 50.0f;

    // Declaring and Initializing based on the accelerating PipesD

    float pipeX = WIDTH;      // horizontal position
    int gap = 180;            // space player passes through
    int pipeTopHeight = 200;  // top pipe height
    int pipeWidth = 80;       // thickness
    float pipeSpeed = 200.0f; // movement speed

    srand(time(NULL));

    if (!initSDL()) return 1;
    if (!TTFinitSDL()) return 1;

    bool running = true;
    bool flap = false;
    bool start = false;
    bool gameOver = false;
    
    int score = 0;
    bool passed = false; // IF PLAYER SHOULD CROSS THAT GAP BETWEEN THE TOP PIPE AND THE BOTTOM PIPE
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();


sprintf(scoreText, "SCORE: 0"); 

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
                    if(gameOver){
                        start = false;
                        gameOver = false;
                        PlayerY = 50.0f;
                        pipeX = WIDTH;
                        velocityY = 0;
                        pipeTopHeight = 200;
                        score = 0;
                        passed = false;
                    }
                    else{ 
                        flap = true;
                        start = true;
                    }

                }
            }
        }

        int ground = HEIGHT - 50;

        if(pipeX + pipeWidth < 0){
            pipeX = WIDTH;
            pipeTopHeight = rand() % 250 + 50;
        }
        SDL_Rect topPipe = {(int)pipeX, 0, pipeWidth, pipeTopHeight};
        SDL_Rect bottomPipe = {(int)pipeX, pipeTopHeight + gap, pipeWidth, HEIGHT - (pipeTopHeight + gap)};

        // I NEED TO WORK ON THE COLLISION PART OF THE GAME
        if(start && !gameOver){
            if(checkCollision(Player, topPipe) || checkCollision(Player, bottomPipe)){
              //running = false; // THIS MIGHT NOT GIVE A BETTER EXPERIENCE, WHY NOT THE GAME RESTARTING
              gameOver = true;
            }
            
            if(flap){
                velocityY = - 300.0f;
                flap = false;
            }
          
            if(!passed && Player.x > pipeX + pipeWidth){
                passed = true;
                score++;
                sprintf(scoreText, "SCORE: %d", score);
            }

            // RESET POSITION OF PIPE
            if(pipeX + pipeWidth < 0){
                pipeX = WIDTH;
                pipeTopHeight = rand() % 250 + 50;
                passed = false;
            }
           
            velocityY += gravity * delta_time; // v = u + at THE INITAL VELOCITY IS 0
            PlayerY += velocityY * delta_time;  // GRAVITY CAN ONLY ACT ON HEIGHT

           
            if(PlayerY + Player.h >= ground){
              PlayerY = ground - Player.h;
              velocityY = 0.0f;

            }
            Player.y = (int)PlayerY; 
           
       }
       if(!gameOver){
        pipeX -= delta_time * pipeSpeed;
       }

            // BACKGROUND COLOR RENDERING SIMULATING SKY ENVIRONMENT
            SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
            SDL_RenderClear(renderer);
            
            //RENDERING FOTR THE TOP PIPE
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);      
            SDL_RenderFillRect(renderer, &topPipe);
            
            // RENDERING FOR THE BOTTOM PIPE
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
            SDL_RenderFillRect(renderer, &bottomPipe);
            
            // RENDERING FOR THE GROUND LEVEL, SIMULATING GRASS
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect groundRect = {0, ground, WIDTH, 50};
            SDL_RenderFillRect(renderer, &groundRect);
            
            // RENDERING FOR THE PLAYER IN CHOICE COLOR
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 230);
            SDL_RenderFillRect(renderer, &Player);

            if(gameOver){
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Rect overLay = {0, 0, WIDTH, HEIGHT};
                SDL_RenderFillRect(renderer, &overLay);
            }
            TTF_Rendering(scoreText);
           

            

            SDL_RenderPresent(renderer);

    }

    //DESTRUCTION OF THE RENDERING AFTER THE LOOP
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
