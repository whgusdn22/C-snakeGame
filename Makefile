all: main

main: ./src/main.cpp ./src/snake.cpp ./src/game.cpp ./src/gate.cpp ./src/item.cpp ./map/map.cpp ./UI/scoreboard.h
	g++ -o main ./src/main.cpp ./src/snake.cpp ./src/game.cpp ./src/gate.cpp ./src/item.cpp ./map/map.cpp ./UI/scoreboard.h -I./include -lpdcurses
