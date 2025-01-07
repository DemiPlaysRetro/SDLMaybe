#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255
#define RED 255, 0, 0, 255
#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 0

// Map Values
const int WIDTH = 512, HEIGHT = 512;

 // 512
int map1[16][32] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2},
        {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};
int rows = sizeof(map1) / sizeof(map1[0]);
int cols = sizeof(map1[0]) / sizeof(map1[0][0]);
int CellSize = 32;

// Player Values
bool onGround;
bool spawned = false;
float jspeed = 20.0f; // Player Speed
float wspeed = 6.0f;
float airwspeed = 15.0f;
int pr = CellSize;   // Player Size (1:1)
float Dampening = 20.0f;
float gravity = 70.0f;
float playerX,playerY,potPlayerX,potPlayerY,cameraPosX,cameraPosY,
playerVelX,playerVelY;
// potPlayer<> = Potential Player X
SDL_Rect plr = {
    0, 0,
    pr, pr};

// Ray Values

// Other Values
const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;
bool firstRun = 1;
bool running;

void qp(float var) {
    std::cout<<var<<std::endl;
}

int GetTile(int x,int y) {
    if (x>=0 && x < cols*CellSize && y>=0 && y< rows*CellSize) { // OOB Check
        return (map1[y][x]);
    } else {
        return 0;
    }
}

void SetTile(int x, int y, int val) {
    if (x>=0 && x < WIDTH && y>=0 && y<HEIGHT) { // OOB Check
        map1[y/CellSize][x/CellSize] = val;
        return;
    }
}

float getDelta() {
    static Uint32 lastFrame = SDL_GetTicks();
    Uint32 currentFrame = SDL_GetTicks();
    float delta = (currentFrame - lastFrame) / 1000.0f;
    lastFrame = currentFrame;
    return delta;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Player Demo: 2D Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event event;

    playerX = 1;
    playerY = 1;

    running = 1;

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

        // Handle Input
        //playerVelY = 0;

        const Uint8 *keystates = SDL_GetKeyboardState(NULL);

        if (keystates[SDL_SCANCODE_W])
        {
            if (onGround)
                playerVelY = -jspeed;
            onGround=false;
        }
        if (keystates[SDL_SCANCODE_S])
        {
            playerVelY = jspeed;
        }
        if (keystates[SDL_SCANCODE_A])
        {
            playerVelX += -wspeed/2.5;
        }
        if (keystates[SDL_SCANCODE_D])
        {
            playerVelX += wspeed/2.5;
        }

        // Check Terminal

        if (playerVelX > 10.0f) {
            playerVelX = 10.0f;
        } else if (playerVelX < -10.0f) {
            playerVelX = -10.0f;
        }
        
        // Dampening
        if (fabs(playerVelX) < 0.8) {
            playerVelX = 0;  // Stop when velocity is close enough to zero
        } else {
            qp(playerVelX);
            playerVelX -= Dampening * 0.015 * ((playerVelX > 0) ? 1 : -1);
        }

        // Gravity
        playerVelY+=gravity * 0.015;
        //qp(GetTile(potPlayerX,potPlayerY));

        // Potential Position
        potPlayerX = playerX + playerVelX * 0.015;
        potPlayerY = playerY + playerVelY * 0.015;

        // OOB Collision
        if (GetTile(potPlayerX,playerY)==0) { // X
            if (playerVelX>0) { // Moving Right
                potPlayerX=(int)potPlayerX;
                playerVelX=0;
            } else { // Moving Left
                potPlayerX=(int)potPlayerX+1;
                playerVelX = 0;
            }
        } else if (GetTile(playerX,potPlayerY)==0) { // Y
            if (playerVelY>0) { // Moving Down, will implement kill in the future
                potPlayerY=(int)potPlayerY;
                playerVelY=0;
            } else {
                potPlayerY=(int)potPlayerY+1;
                playerVelY=0;
            } // Moving Up
        }

        // Collision
        if (playerVelX <= 0) { // Left
            if (GetTile(potPlayerX,playerY)==2||GetTile(potPlayerX,playerY+0.9f)==2) {
                potPlayerX=(int)potPlayerX+1;
                playerVelX = 0;
            }
        } else { // Right
            if (GetTile(potPlayerX+0.99f,playerY)==2||GetTile(potPlayerX+0.99f,playerY+0.9f)==2) {
                potPlayerX=(int)potPlayerX;
                playerVelX=0;
            }
        }

        if (playerVelY >= 0) { // Down
            if (GetTile(playerX,potPlayerY+1)==2||GetTile(playerX+0.9f,potPlayerY+1)==2) {
                potPlayerY=(int)potPlayerY;
                playerVelY=0;
                onGround = true;
            }
        } else { // Up
            if (GetTile(playerX,potPlayerY)==2||GetTile(playerX+0.9f,potPlayerY)==2) {
                potPlayerY=(int)potPlayerY+1;
                playerVelY=0;
            }
        }

        playerX = potPlayerX;
        playerY = potPlayerY;

        cameraPosX = playerX;
        cameraPosY = playerY;

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, BLACK);
        SDL_RenderClear(renderer);

        // Draw the map

        int VisibleTilesX = WIDTH/CellSize;
        int VisibleTilesY = HEIGHT/CellSize;

        // Calculate Top-Leftmost visible tile
        float offsetX = cameraPosX - (float)(VisibleTilesX) / 2.0f;
        float offsetY = cameraPosY - (float)(VisibleTilesY) / 2.0f;

        //SDL_SetRenderDrawColor(renderer,RED);
        //SDL_RenderDrawPoint(renderer,offsetX,offsetY);

        // Clamp to boundaries

        if (offsetX < 0) offsetX = 0;
        if (offsetY < 0) offsetY = 0;
        if (offsetX > cols - VisibleTilesX) offsetX = cols - VisibleTilesX;
        if (offsetY > rows - VisibleTilesY) offsetY = rows - VisibleTilesY; 

        // Get offsets for smooth movement
        float tileoffX = (offsetX-floor(offsetX))*CellSize; // Returns decimal part of offsetX multiplied to screen coords
        float tileoffY = (offsetY-floor(offsetY))*CellSize;
        
        /*
        SDL_SetRenderDrawColor(renderer,RED);
        SDL_Rect oldP = {
            playerX,playerY,pr,pr
        };
        SDL_RenderDrawRect(renderer,&oldP);
        SDL_SetRenderDrawColor(renderer,WHITE);
        SDL_RenderDrawPoint(renderer,offsetX,offsetY);
        SDL_RenderDrawPoint(renderer,offsetX*CellSize,offsetY*CellSize);
        */
        

        for (int i = 0; i < VisibleTilesY+1; i++)
        { 
            for (int j = 0; j < VisibleTilesX+1; j++)
            {
                switch (GetTile(j+offsetX,i+offsetY)) {
                    case 2: // Block
                    {
                        SDL_SetRenderDrawColor(renderer, GREEN);
                        SDL_Rect temp1 = {
                            j * CellSize - tileoffX,
                            i * CellSize - tileoffY,
                            CellSize, CellSize};
                        SDL_RenderDrawRect(renderer, &temp1);
                        break;
                    }
                    case 3: // Spawnpoint
                    {
                        if (!spawned) {
                        std::cout << "Triggered" << std::endl;
                        playerX = j+offsetX;
                        playerY = i+offsetY;
                        spawned = true;
                        }

                        break;
                    }
                    default:
                        break;
                };
            }
        }

        // Draw player, plr.x or plr.y are screen coords, while playerX and playerY are world coords
        SDL_SetRenderDrawColor(renderer, BLUE);
        plr.x = (playerX-offsetX)*CellSize;
        plr.y = (playerY-offsetY)*CellSize;
        SDL_RenderDrawRect(renderer, &plr);

        // Draw Screen
        SDL_RenderPresent(renderer);

        // How long was this game iteration?
        frameTime = SDL_GetTicks() - frameStart;

        // Delay accordingly
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        // This ain't the first run no-mo
        if (firstRun)
        {
            firstRun = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}