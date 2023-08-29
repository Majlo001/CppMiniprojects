# Definicje zmiennych
CXX = g++
CXXFLAGS = -g -I"C:/Users/Majlo/DevTools/SFML-2.5.1/include"
LDFLAGS = -L"C:/Users/Majlo/DevTools/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Lista plików źródłowych
SOURCES = Main.cpp Input.cpp Draw.cpp Engine.cpp TextureHolder.cpp Update.cpp  Bob.cpp Thomas.cpp PlayableCharacter.cpp LevelManager.cpp DetectCollisions.cpp LoadLevel.cpp

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