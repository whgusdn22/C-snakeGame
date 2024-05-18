all : main
main : ./src/maim.cpp
	g++ -o main ./src/main.cpp -lncurese