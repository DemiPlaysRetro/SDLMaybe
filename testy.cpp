#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

// Screen Values
const int WIDTH = 512, HEIGHT = 512;

// Player Values
float PlayerX, PlayerY;
float PlayerRadius, PlayerAngle,PlayerSpeed,RotationStep;
bool MoveForwards;

// Other Values
const int FPS = 60;
const int frameDelay = 1000/FPS;
Uint32 frameStart;
int frameTime;

float RadiansToDegrees(float radians)
{
    return (180 / M_PI) * radians;
}

float DegreesToRadians(float degrees)
{
    return (M_PI / 180) * degrees;
}

void DrawCircle(SDL_Renderer *renderer, int x, int y, float radius)
{
    for (int i = 0; i < 360; i++)
    {
        SDL_RenderDrawPoint(renderer, cos(DegreesToRadians(i)) * radius + x, sin(DegreesToRadians(i)) * radius + y);
    }
}

int main(int argc, char *argv[])
{
    PlayerX = WIDTH / 2;
    PlayerY = HEIGHT / 2;
    PlayerRadius = 11.25;
    RotationStep = 0.1f;
    PlayerSpeed = 3;
    PlayerAngle = 0;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    bool running = 1;

    while (running)
    {
        // Record frame start
        frameStart = SDL_GetTicks();

        // Event Loop
        while (SDL_PollEvent(&event))
        {
            // Quit
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if(keystates[SDL_SCANCODE_W]) {
            float DeltaX = cos(PlayerAngle)*PlayerSpeed;
            float DeltaY = sin(PlayerAngle)*PlayerSpeed;
            PlayerY+=DeltaY;
            PlayerX+=DeltaX;
        }
        if(keystates[SDL_SCANCODE_A]) {
            PlayerAngle-=RotationStep;
        }
        if(keystates[SDL_SCANCODE_D]) {
            PlayerAngle+=RotationStep;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        // Draw player
        SDL_SetRenderDrawColor(renderer, 0,255,0,255);
        DrawCircle(renderer, PlayerX, PlayerY, PlayerRadius);

        // Draw Guideline;
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        SDL_RenderDrawLine(renderer, PlayerX, PlayerY, cos(PlayerAngle) * PlayerRadius + PlayerX, sin(PlayerAngle) * PlayerRadius + PlayerY);

        // Draw Screen
        SDL_RenderPresent(renderer);

        // How long was this game iteration?
        frameTime = SDL_GetTicks() - frameStart;

        // Delay accordingly
        if (frameDelay>frameTime) {
            SDL_Delay(frameDelay-frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}