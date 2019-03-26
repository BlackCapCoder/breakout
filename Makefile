build:
	g++ -std=c++17 -lSDL2 -lSDL2_image -O3 *.cpp -o main

remake:
	rm -f *.o

run: build
	./main
