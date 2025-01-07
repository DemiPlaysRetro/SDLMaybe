#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PLAYER_SPEED = 5.0f;
const float TURN_SPEED = 0.1f;

struct Player {
    float x, y;      // Player position
    float angle;     // Player angle in radians

    Player(float startX, float startY)
        : x(startX), y(startY), angle(0.0f) {}
    
    void moveForward() {
        x += PLAYER_SPEED * cos(angle);
        y += PLAYER_SPEED * sin(angle);
    }

    void turnLeft() {
        angle -= TURN_SPEED;
    }

    void turnRight() {
        angle += TURN_SPEED;
    }
};

void drawPlayer(SDL_Renderer* renderer, const Player& player) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    // Draw player as a line representing direction
    SDL_RenderDrawLine(renderer, player.x, player.y,
                       player.x + 20 * cos(player.angle),
                       player.y + 20 * sin(player.angle));
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Player Movement",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Player player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2); // Start in the middle of the window
    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        
        if (currentKeyStates[SDL_SCANCODE_W]) {
            player.moveForward();
        }
        if (currentKeyStates[SDL_SCANCODE_A]) {
            player.turnLeft();
        }
        if (currentKeyStates[SDL_SCANCODE_D]) {
            player.turnRight();
        }

        // Clear screen
        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //SDL_RenderClear(renderer);

        // Draw player
        drawPlayer(renderer, player);

        // Present renderer
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
    
    return 0;
}