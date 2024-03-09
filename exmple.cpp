#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

// Các thông số cửa sổ
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FRICTION = 0.98f; // Hệ số ma sát để giảm dần vận tốc

// Cấu trúc để lưu trữ thông tin về quả bóng
struct Ball {
    float x, y; // Vị trí
    float velX, velY; // Vận tốc
    int width, height; // Kích thước của quả bóng
    SDL_Texture* texture; // Texture của quả bóng
};

// Cấu trúc để lưu trữ thông tin về chướng ngại vật
struct Obstacle {
    float x, y; // Vị trí
    int width, height; // Kích thước của chướng ngại vật
    SDL_Texture* texture; // Texture của chướng ngại vật
};

struct Hole {
    float x, y; // Vị trí của hố
    int width, height; // Kích thước của hố
    SDL_Texture* texture; // Texture của hố
};


// Hàm kiểm tra va chạm giữa quả bóng và chướng ngại vật
bool checkCollision(const Ball& ball, const Obstacle& obstacle) {
    // Tính toán các giới hạn của hình chữ nhật bao quanh quả bóng
    int ballLeft = ball.x - ball.width / 2;
    int ballRight = ball.x + ball.width / 2;
    int ballTop = ball.y - ball.height / 2;
    int ballBottom = ball.y + ball.height / 2;

    // Tính toán các giới hạn của hình chữ nhật bao quanh chướng ngại vật
    int obstacleLeft = obstacle.x - obstacle.width / 2;
    int obstacleRight = obstacle.x + obstacle.width / 2;
    int obstacleTop = obstacle.y - obstacle.height / 2;
    int obstacleBottom = obstacle.y + obstacle.height / 2;

    // Kiểm tra va chạm giữa hai hình chữ nhật
    if (ballRight >= obstacleLeft && ballLeft <= obstacleRight &&
        ballBottom >= obstacleTop && ballTop <= obstacleBottom) {
        return true; // Có va chạm
    }

    return false; // Không có va chạm
}

void handleCollision(Ball& ball, const Obstacle& obstacle) {
    // Tính toán véc-tơ từ trung tâm quả bóng đến điểm va chạm
    float dX = ball.x - obstacle.x;
    float dY = ball.y - obstacle.y;

    // Tính toán khoảng cách từ trung tâm quả bóng đến điểm va chạm
    float distance = sqrt(dX * dX + dY * dY);

    // Chuẩn hóa véc-tơ va chạm
    float normalX = dX / distance;
    float normalY = dY / distance;

    // Tính toán vận tốc tương đối của quả bóng và véc-tơ pháp tuyến
    float relativeVelocity = (ball.velX * normalX) + (ball.velY * normalY);

    // Áp dụng định luật phản xạ
    ball.velX -= 2 * relativeVelocity * normalX;
    ball.velY -= 2 * relativeVelocity * normalY;
}

// Tạo vị trí ngẫu nhiên cho các chướng ngại vật và lựa chọn ngẫu nhiên hình ảnh
void generateRandomObstacles(Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer) {
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

void generateRandomHole(Hole& hole, SDL_Renderer* renderer) {
    // Load hình ảnh của hố từ tệp tin
    SDL_Surface* holeSurface = IMG_Load("img_src/hole.png");
    if (!holeSurface) {
        printf("Unable to load hole image: %s\n", IMG_GetError());
        exit(1);
    }
    hole.texture = SDL_CreateTextureFromSurface(renderer, holeSurface);
    SDL_FreeSurface(holeSurface);

    // Tạo vị trí ngẫu nhiên cho hố
    hole.x = rand() % WINDOW_WIDTH;
    hole.y = rand() % WINDOW_HEIGHT;
    hole.width = 20; // Kích thước có thể được thiết lập tùy ý
    hole.height = 20; // Kích thước có thể được thiết lập tùy ý
}

bool checkHoleCollision(const Ball& ball, const Hole& hole) {
    int ballLeft = ball.x - ball.width / 2;
    int ballRight = ball.x + ball.width / 2;
    int ballTop = ball.y - ball.height / 2;
    int ballBottom = ball.y + ball.height / 2;

    int holeLeft = hole.x - hole.width / 2;
    int holeRight = hole.x + hole.width / 2;
    int holeTop = hole.y - hole.height / 2;
    int holeBottom = hole.y + hole.height / 2;

    if (ballRight >= holeLeft && ballLeft <= holeRight &&
        ballBottom >= holeTop && ballTop <= holeBottom) {
        return true;
    }

    return false;
}


int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Khởi tạo SDL2
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG); // Khởi tạo thư viện để load ảnh PNG

    // Tạo cửa sổ
    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load hình ảnh background từ tệp tin
    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Seed cho hàm random
    srand(time(NULL));

    // Tạo một mảng chứa các chướng ngại vật
    const int NUM_OBSTACLES = 15; // Số lượng chướng ngại vật
    Obstacle obstacles[NUM_OBSTACLES];

    // Thiết lập vị trí của các chướng ngại vật và chọn ngẫu nhiên hình ảnh
    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);

    Hole hole;
    generateRandomHole(hole, renderer);

    // Tạo một quả bóng và khởi tạo vị trí ban đầu ngẫu nhiên
    Ball ball;
    ball.x = rand() % WINDOW_WIDTH; // Vị trí ngẫu nhiên trên trục x
    ball.y = rand() % WINDOW_HEIGHT; // Vị trí ngẫu nhiên trên trục y
    ball.velX = 0; // Vận tốc ban đầu là 0
    ball.velY = 0; // Vận tốc ban đầu là 0

    // Load hình ảnh cho quả bóng từ tệp tin
    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png");
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return 1;
    }
    ball.texture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);
    SDL_QueryTexture(ball.texture, NULL, NULL, &ball.width, &ball.height);
    
    SDL_Event e;
    bool isBallReleased = false;
    bool isDragging = false;
    int dragStartX, dragStartY;
    int dragEndX, dragEndY;
    float dragDistance = 0.0f;
    float maxDragDistance = 200.0f;
    int count = 0;


    // Main loop
    bool quit = false;
    while (!quit) {
        // Xử lý sự kiện
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && !isDragging && !isBallReleased) {
                // Khi chuột được nhấn, ghi lại vị trí xuất phát của chuột và đặt biến isDragging thành true
                isDragging = true;
                SDL_GetMouseState(&dragStartX, &dragStartY);
            } else if (e.type == SDL_MOUSEMOTION && isDragging) {
                // Khi người dùng di chuyển chuột, tính toán vận tốc của quả bóng dựa trên hướng kéo chuột
                SDL_GetMouseState(&dragEndX, &dragEndY);
                dragDistance = std::hypot(dragEndX - dragStartX, dragEndY - dragStartY);

                // Cập nhật vận tốc của quả bóng dựa trên hướng kéo chuột
                float angle = atan2(dragEndY - dragStartY, dragEndX - dragStartX);
                ball.velX = -dragDistance * cos(angle) / 10.0f;
                ball.velY = -dragDistance * sin(angle) / 10.0f;
            } else if (e.type == SDL_MOUSEBUTTONUP && isDragging) {
                // Khi chuột được nhả, thiết lập biến isDragging thành false và đặt lại vận tốc của quả bóng
                isDragging = false;
                isBallReleased = true; // Đánh dấu rằng quả bóng đã được thả ra
                dragDistance = 0.0f;
            }
        }

        if (isBallReleased) {
            // Cập nhật vị trí của quả bóng và xử lý va chạm với các chướng ngại vật
            ball.x += ball.velX;
            ball.y += ball.velY;

            // Xử lý va chạm với viền màn hình
            if (ball.x - ball.width / 2 < 0 || ball.x + ball.width / 2 > WINDOW_WIDTH) {
                ball.velX = -ball.velX; // Đảo ngược hướng di chuyển khi va chạm với cạnh trái hoặc phải
            }
            if (ball.y - ball.height / 2 < 0 || ball.y + ball.height / 2 > WINDOW_HEIGHT) {
                ball.velY = -ball.velY; // Đảo ngược hướng di chuyển khi va chạm với cạnh trên hoặc dưới
            }

            if (fabs(ball.velX) < 0.1 && fabs(ball.velY) < 0.1) {
                isBallReleased = false;
            }

            count++;

            // Kiểm tra va chạm giữa quả bóng và các chướng ngại vật
            for (int i = 0; i < NUM_OBSTACLES; ++i) {
                if (checkCollision(ball, obstacles[i])) {
                    // Xử lý va chạm giữa quả bóng và chướng ngại vật
                    handleCollision(ball, obstacles[i]);
                }
            }

            if (checkHoleCollision(ball, hole)) {
                // Xử lý khi quả bóng chạm vào hố
                // Ví dụ: Cho quả bóng biến mất
                ball.x = -1000; // Di chuyển quả bóng ra khỏi màn hình
                ball.y = -1000; // Di chuyển quả bóng ra khỏi màn hình

                printf("Qua bong vao lo sau %d lan di chuyen.\n", count);

            }
 
            // Giảm dần vận tốc của quả bóng
            ball.velX *= FRICTION;
            ball.velY *= FRICTION;
        }

        // Xóa cửa sổ
        SDL_RenderClear(renderer);

        // Vẽ background trước
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Vẽ các chướng ngại vật
        for (int i = 0; i < NUM_OBSTACLES; ++i) {
            SDL_Rect obstacleRect = { (int)(obstacles[i].x - obstacles[i].width / 2), (int)(obstacles[i].y - obstacles[i].height / 2), obstacles[i].width, obstacles[i].height };
            SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacleRect);
        }
        
        //Vẽ hố
        SDL_Rect holeRect = { (int)(hole.x - hole.width / 2), (int)(hole.y - hole.height / 2), hole.width, hole.height };
        SDL_RenderCopy(renderer, hole.texture, NULL, &holeRect);

        // Vẽ quả bóng
        SDL_Rect ballRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
        SDL_RenderCopy(renderer, ball.texture, NULL, &ballRect);

        // Hiển thị cửa sổ
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Throttle the frame rate
    }

    // Giải phóng bộ nhớ và thoát SDL2
    SDL_DestroyTexture(ball.texture);
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        SDL_DestroyTexture(obstacles[i].texture);
    }
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
