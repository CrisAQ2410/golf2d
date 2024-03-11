#include "obstacles_random.h"

void generateRandomObstacles(Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer) {
    // Mảng các đường dẫn đến các hình ảnh chướng ngại vật
    std::vector<std::string> obstacleImages = {
        "img_src/tile32_light.png",
        "img_src/tile32_dark.png"
        // Thêm các đường dẫn đến các hình ảnh khác nếu cần
    };

    // Khoảng cách tối thiểu giữa các chướng ngại vật
    const int minDistance = 100;

    for (int i = 0; i < numObstacles; ++i) {
        bool collision = true;
        while (collision) {
            obstacles[i].x = rand() % WINDOW_WIDTH;
            obstacles[i].y = rand() % WINDOW_HEIGHT;
            obstacles[i].width = 50; // Kích thước có thể được thiết lập tùy ý
            obstacles[i].height = 50; // Kích thước có thể được thiết lập tùy ý

            // Kiểm tra va chạm với các chướng ngại vật đã được tạo trước đó
            collision = false;
            for (int j = 0; j < i; ++j) {
                if (abs(obstacles[i].x - obstacles[j].x) < minDistance &&
                    abs(obstacles[i].y - obstacles[j].y) < minDistance) {
                    collision = true;
                    break;
                }
            }
        }

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