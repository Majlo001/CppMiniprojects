all: compile link open

compile:
	g++ -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include" -c main.cpp -o output/main.o
	g++ -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include" -c Bat.cpp -o output/Bat.o
	g++ -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include" -c Ball.cpp -o output/Ball.o

link:
	g++ output/main.o output/Bat.o output/Ball.o -o output/main -L"C:/Users/Majlo/DevTools/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

open:
	start output/main.exe