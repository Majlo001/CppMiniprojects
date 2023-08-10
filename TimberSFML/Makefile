all: compile link open

compile:
	g++ -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include" -c main.cpp -o main.o

link:
	g++ main.o -o output/main -L"C:/Users/Majlo/DevTools/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

open:
	start output/main.exe
