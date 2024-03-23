#include "obstacles_random.h"

void generateRandomObstacles(Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer) {
    std::vector<std::string> obstacleImages = {
        "img_src/tile32_light.png",
        "img_src/tile32_dark.png"
    };

    const int minDistance = 100;

    for (int i = 0; i < numObstacles; ++i) {
        bool collision = true;
        while (collision) {
            obstacles[i].x = rand() % WINDOW_WIDTH;
            obstacles[i].y = rand() % WINDOW_HEIGHT;
            obstacles[i].width = 50; 
            obstacles[i].height = 50; 

            collision = false;
            for (int j = 0; j < i; ++j) {
                if (abs(obstacles[i].x - obstacles[j].x) < minDistance &&
                    abs(obstacles[i].y - obstacles[j].y) < minDistance) {
                    collision = true;
                    break;
                }
            }
        }

        int randomIndex = rand() % obstacleImages.size();
        SDL_Surface* obstacleSurface = IMG_Load(obstacleImages[randomIndex].c_str());
        if (!obstacleSurface) {
            printf("Unable to load obstacle image: %s\n", IMG_GetError());
            exit(1);
        }
        obstacles[i].texture = SDL_CreateTextureFromSurface(renderer, obstacleSurface);
        SDL_FreeSurface(obstacleSurface);
    }
}