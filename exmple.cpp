#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

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
    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png"); // Thay "background_image.png" bằng đường dẫn của file hình ảnh
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return 1;
    }

    // Tạo texture từ surface của hình ảnh
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Load hình ảnh cho quả bóng từ tệp tin
    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png"); // Thay "ball_image.png" bằng đường dẫn của file hình ảnh
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return 1;
    }

    // Tạo texture từ surface của hình ảnh
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface); // Giải phóng bộ nhớ sau khi tạo texture

    // Tạo một quả bóng và khởi tạo vị trí và vận tốc ban đầu
    Ball ball;
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.velX = 0; // Vận tốc ban đầu là 0
    ball.velY = 0; // Vận tốc ban đầu là 0

    // Lấy kích thước của quả bóng từ hình ảnh
    SDL_QueryTexture(ballTexture, NULL, NULL, &ball.width, &ball.height);

    SDL_Event e;
    bool quit = false;
    bool isDragging = false;
    bool isBallReleased = false; // Biến để kiểm tra xem quả bóng đã được thả ra chưa
    int dragStartX, dragStartY;
    int dragEndX, dragEndY;
    float dragDistance = 0.0f;
    float maxDragDistance = 200.0f; // Khoảng cách kéo tối đa

    while (!quit) {
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
            // Cập nhật vị trí của quả bóng
            ball.x += ball.velX;
            ball.y += ball.velY;

            // Xử lý va chạm với viền màn hình
            if (ball.x - ball.width / 2 < 0 || ball.x + ball.width / 2 > WINDOW_WIDTH) {
                ball.velX = -ball.velX; // Đảo ngược hướng di chuyển khi va chạm với cạnh trái hoặc phải
            }
            if (ball.y - ball.height / 2 < 0 || ball.y + ball.height / 2 > WINDOW_HEIGHT) {
                ball.velY = -ball.velY; // Đảo ngược hướng di chuyển khi va chạm với cạnh trên hoặc dưới
            }
            // Nếu quả bóng dừng lại (ví dụ: vận tốc quá nhỏ), đặt lại isBallReleased thành false
            if (fabs(ball.velX) < 0.1 && fabs(ball.velY) < 0.1) {
                isBallReleased = false;
            }

            // Giảm dần vận tốc của quả bóng
            ball.velX *= FRICTION;
            ball.velY *= FRICTION;
        }

        // Xóa cửa sổ
        SDL_RenderClear(renderer);

        // Vẽ background trước
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Vẽ quả bóng
        SDL_Rect dstRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
        SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);

        // Hiển thị cửa sổ
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Throttle the frame rate
    }

    // Giải phóng bộ nhớ và thoát SDL2
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
