#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

void generateRandomObstacles(Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer) {
    std::vector<std::string> obstacleImages = {
        "img_src/tile32_light.png",
        "img_src/tile32_dark.png"
    };

    const int minDistance = 100;
    Hole hole;

    for (int i = 0; i < numObstacles; ++i) {
        bool collision = true;
        while (collision) {
            obstacles[i].x = rand() % WINDOW_WIDTH;
            obstacles[i].y = rand() % WINDOW_HEIGHT;
            obstacles[i].width = 50;
            obstacles[i].height = 50;

            collision = false;
            // Kiểm tra va chạm với các vật thể đã tạo trước đó
            for (int j = 0; j < i; ++j) {
                if (abs(obstacles[i].x - obstacles[j].x) < minDistance &&
                    abs(obstacles[i].y - obstacles[j].y) < minDistance) {
                    collision = true;
                    break;
                }
            }
            // Kiểm tra va chạm với hố
            if (abs(obstacles[i].x - hole.x) < minDistance &&
                abs(obstacles[i].y - hole.y) < minDistance) {
                collision = true;
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

void resetGame(Ball& ball, Obstacle obstacles[], Hole& hole, int& score, int& strokes, bool& win, SDL_Renderer* renderer) {
    ball.x = rand() % WINDOW_WIDTH; // Vị trí ngẫu nhiên trên trục x
    ball.y = rand() % WINDOW_HEIGHT; // Vị trí ngẫu nhiên trên trục y
    ball.velX = 0; // Vận tốc ban đầu là 0
    ball.velY = 0; // Vận tốc ban đầu là 0

    const int NUM_OBSTACLES = 24; // Số lượng chướng ngại vật

    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);
    generateRandomHole(hole, renderer);

    score = 200;
    strokes = 0;
    win = false;
}

bool isDragging = false;
int dragStartX, dragStartY;
float dragDistance = 0.0f;
int dragEndX, dragEndY;

void drawPowerBar(SDL_Renderer* renderer) {
    // Vẽ thanh lực
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(renderer, dragStartX, dragStartY, dragStartX - (dragEndX - dragStartX), dragStartY - (dragEndY - dragStartY));
}

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;
    SDL_Color textColor = {255, 255, 255}; // Màu văn bản là trắng

    // Khởi tạo SDL2
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG); // Khởi tạo thư viện để load ảnh PNG
    TTF_Init();

    // Tạo cửa sổ
    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Khởi tạo font
    font = TTF_OpenFont("font/font.ttf", 30);
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return 1;
    }

    // Load hình ảnh background từ tệp tin
    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Khởi tạo SDL Mixer
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Mix_Init: %s\n", Mix_GetError());
        // Xử lý lỗi khi không thể khởi tạo SDL Mixer
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        // Xử lý lỗi khi không thể mở kênh âm thanh
    }

    // Tải âm thanh
    Mix_Chunk* holeSound = Mix_LoadWAV("sound/hole.mp3");
    if (!holeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
        // Xử lý lỗi khi không thể tải âm thanh
    }

    // Tải âm thanh cho va chạm
    Mix_Chunk* collisionSound = Mix_LoadWAV("sound/swing.mp3");
    if (!collisionSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
        // Xử lý lỗi khi không thể tải âm thanh
    }

    // Tải âm thanh khi kéo bóng
    Mix_Chunk* chargeSound = Mix_LoadWAV("sound/charge.mp3");
    if (!chargeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    // Seed cho hàm random
    srand(time(NULL));

    // Tạo một mảng chứa các chướng ngại vật
    const int NUM_OBSTACLES = 24; // Số lượng chướng ngại vật
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
    float maxDragDistance = 200.0f;

    int strokes = 0;
    int score = 200;

    // Main loop
    bool win = false;
    bool quit = false;
    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
            else if (e.type == SDL_MOUSEBUTTONDOWN && !isDragging && !isBallReleased) {
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
                Mix_PlayChannel(-1, chargeSound, 0);
                // Khi chuột được nhả, thiết lập biến isDragging thành false và đặt lại vận tốc của quả bóng
                isDragging = false;
                isBallReleased = true; // Đánh dấu rằng quả bóng đã được thả ra
                dragDistance = 0.0f;
                strokes ++;
                if (strokes > 1) score -=5;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r && win) {
                    resetGame(ball, obstacles, hole, score, strokes, win, renderer);
                }
            }
        }

        if (isBallReleased) {
            // Cập nhật vị trí của quả bóng và xử lý va chạm với các chướng ngại vật
            ball.x += ball.velX;
            ball.y += ball.velY;
  
            // Xử lý va chạm với viền màn hình
            if (ball.x - ball.width / 2 < 0 || ball.x + ball.width / 2 > WINDOW_WIDTH) {
                ball.velX = -ball.velX; // Đảo ngược hướng di chuyển khi va chạm với cạnh trái hoặc phải
                Mix_PlayChannel(-1, collisionSound, 0);
            }
            if (ball.y - ball.height / 2 < 0 || ball.y + ball.height / 2 > WINDOW_HEIGHT) {
                ball.velY = -ball.velY; // Đảo ngược hướng di chuyển khi va chạm với cạnh trên hoặc dưới
                Mix_PlayChannel(-1, collisionSound, 0);
            }

            if (fabs(ball.velX) < 0.1 && fabs(ball.velY) < 0.1) {
                isBallReleased = false;
            }

            // Kiểm tra va chạm giữa quả bóng và các chướng ngại vật
            for (int i = 0; i < NUM_OBSTACLES; ++i) {
                if (checkCollision(ball, obstacles[i])) {
                    Mix_PlayChannel(-1, collisionSound, 0);
                    // Xử lý va chạm giữa quả bóng và chướng ngại vật
                    handleCollision(ball, obstacles[i]);
                }
            }

            if (checkHoleCollision(ball, hole)) {
                Mix_PlayChannel(-1, holeSound, 0);
                // Xử lý khi quả bóng chạm vào hố
                float distanceToHole = sqrt((ball.x - hole.x) * (ball.x - hole.x) + (ball.y - hole.y) * (ball.y - hole.y));
                float angleToHole = atan2(hole.y - ball.y, hole.x - ball.x);

                // Thiết lập vận tốc của quả bóng để nó di chuyển theo hướng của lỗ
                float speed = 5.0f; // Tốc độ di chuyển của quả bóng vào lỗ
                ball.velX = speed * cos(angleToHole);
                ball.velY = speed * sin(angleToHole);

                // Di chuyển quả bóng mỗi lần vòng lặp
                ball.x += ball.velX;
                ball.y += ball.velY;

                // Kiểm tra nếu quả bóng đủ gần lỗ, thì dừng lại
                if (distanceToHole < 10.0f) { // Điều chỉnh giá trị này tùy theo độ lớn của lỗ
                    ball.velX = 0;
                    ball.velY = 0;
                    win = true;
                }
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

        if (isDragging) {
            drawPowerBar(renderer);
        }

        // Vẽ quả bóng
        SDL_Rect ballRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
        SDL_RenderCopy(renderer, ball.texture, NULL, &ballRect);

        // Vẽ và cập nhật điểm
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_Rect scoreRect = {350, 570, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        // Vẽ và cập nhật điểm
        SDL_Surface* strokesSurface = TTF_RenderText_Solid(font, ("Strokes: " + std::to_string(strokes)).c_str(), textColor);
        SDL_Texture* strokesTexture = SDL_CreateTextureFromSurface(renderer, strokesSurface);
        SDL_Rect strokesRect = {350, 1, strokesSurface->w, strokesSurface->h};
        SDL_RenderCopy(renderer, strokesTexture, NULL, &strokesRect);
        SDL_FreeSurface(strokesSurface);
        SDL_DestroyTexture(strokesTexture);
        if (win) {

            // Vẽ khung
            SDL_Rect frameRect = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderFillRect(renderer, &frameRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &frameRect);

            SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Your score: " + std::to_string(score)).c_str(), textColor);
            SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            SDL_Rect scoreRect = {WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 , scoreSurface->w, scoreSurface->h};
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_FreeSurface(scoreSurface);
            SDL_DestroyTexture(scoreTexture);

            SDL_Surface* strokesSurface = TTF_RenderText_Solid(font, ("Your strokes: " + std::to_string(strokes)).c_str(), textColor);
            SDL_Texture* strokesTexture = SDL_CreateTextureFromSurface(renderer, strokesSurface);
            SDL_Rect strokesRect = {WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 40, strokesSurface->w, strokesSurface->h};
            SDL_RenderCopy(renderer, strokesTexture, NULL, &strokesRect);
            SDL_FreeSurface(strokesSurface);
            SDL_DestroyTexture(strokesTexture);

            SDL_Surface* winSurface = TTF_RenderText_Solid(font,"Congratulations! You Won!", textColor);
            SDL_Texture* winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
            SDL_Rect winRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 - 80, winSurface->w, winSurface->h};
            SDL_RenderCopy(renderer, winTexture, NULL, &winRect);
            SDL_FreeSurface(winSurface);
            SDL_DestroyTexture(winTexture);

            SDL_Surface* restartSurface = TTF_RenderText_Solid(font,"Press R to restart", textColor);
            SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
            SDL_Rect restartRect = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 40, restartSurface->w, restartSurface->h};
            SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
            SDL_FreeSurface(restartSurface);
            SDL_DestroyTexture(restartTexture);
         }

        // Cập nhật cửa sổ
        SDL_RenderPresent(renderer);

        // Thêm độ trễ ngắn để không quá tải CPU
        SDL_Delay(10);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(ball.texture);
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        SDL_DestroyTexture(obstacles[i].texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    IMG_Quit(); // Tắt SDL_image
    SDL_Quit(); // Tắt SDL

    return 0;
}