build:
	g++ -fconcepts -fpermissive -std=c++2a -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -O3 *.cpp -o main

run: build
	./main
