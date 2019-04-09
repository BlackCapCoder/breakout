build:
	g++ -fconcepts -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -O3 *.cpp -o main

run: build
	./main
