all : main
main : ./src/main.cpp
	g++ -o main ./src/main.cpp -lncurses