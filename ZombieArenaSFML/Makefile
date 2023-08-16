# Definicje zmiennych
CXX = g++
CXXFLAGS = -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include"
LDFLAGS = -L"C:/Users/Majlo/DevTools/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Lista plików źródłowych
SOURCES = main.cpp Player.cpp createBackground.cpp Zombie.cpp TextureHolder.cpp Bullet.cpp Pickup.cpp

# Automatyczna generacja listy plików obiektowych
OBJECTS = $(patsubst %.cpp,output/%.o,$(SOURCES))

# Cel główny
all: compile link open

# Reguła kompilacji plików źródłowych do plików obiektowych
compile: $(OBJECTS) 
	$(foreach src,$(SOURCES),$(CXX) $(CXXFLAGS) -c $(src) -o output/$(notdir $(src:.cpp=.o));)


# Reguła linkowania
link: $(OBJECTS)
	$(CXX) $(OBJECTS) -o output/main $(LDFLAGS)

# Reguła otwierania
open:
	start output/main.exe