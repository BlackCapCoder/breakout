build:
	g++ -fconcepts -fpermissive -std=c++2a -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -O0 *.cpp -o main

run: build
	./main
