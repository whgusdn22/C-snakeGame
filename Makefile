all: main

main: ./src/main.cpp ./src/snake.cpp ./src/game.cpp ./src/gate.cpp ./src/item.cpp ./map/map.cpp
	g++ -o main ./src/main.cpp ./src/snake.cpp ./src/game.cpp ./src/gate.cpp ./src/item.cpp ./map/map.cpp -I./include -I./include/map -lpdcurses
