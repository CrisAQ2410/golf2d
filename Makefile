all:
	g++ -Isource/include -Lsource/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image