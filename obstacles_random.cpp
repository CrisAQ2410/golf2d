#include "obstacles_random.h"

void generateRandomObstacles (Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer) {
    // Mảng các đường dẫn đến các hình ảnh chướng ngại vật
    std::vector<std::string> obstacleImages = {
        "img_src/tile32_light.png",
        "img_src/tile32_dark.png"
        // Thêm các đường dẫn đến các hình ảnh khác nếu cần
    };

    for (int i = 0; i < numObstacles; ++i) {
        obstacles[i].x = rand() % WINDOW_WIDTH;
        obstacles[i].y = rand() % WINDOW_HEIGHT;
        obstacles[i].width = 50; // Kích thước có thể được thiết lập tùy ý
        obstacles[i].height = 50; // Kích thước có thể được thiết lập tùy ý

        // Chọn ngẫu nhiên một hình ảnh từ bộ sưu tập
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