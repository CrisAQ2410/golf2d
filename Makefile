all:
    g++ -Isource/include -Lsource/lib -o main main.cpp mouse_handler.cpp obstacles_random.cpp check_collision.cpp handle_collision.cpp hole_random.cpp check_hole_collision.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
    g++ -Isource/include -Lsource/lib -o src/main.cpp src/physics_handler.cpp src/mouse_handler.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
    g++ -Isource/include -Lsource/lib -o tetris tetris.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
    