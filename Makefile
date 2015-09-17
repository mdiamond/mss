CC = g++ -g -Wall
CFLAGS = -std=c++11
LDFLAGS = -lSDL2 -lSDL2_ttf
OBJFLAGS = $(CC) $(CFLAGS) -c
OBJECTS = Timer.o Module.o Oscillator.o Output.o Graphics_Object.o Rect.o Text.o Waveform.o signal_processing.o image_processing.o tests.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
Timer.o : Timer.cpp Timer.hpp
	$(OBJFLAGS) Timer.cpp
Module.o : Module.cpp Module.hpp
	$(OBJFLAGS) Module.cpp
Output.o Oscillator.o : $(wildcard Modules/*.cpp) $(wildcard Modules/*.hpp)
	$(OBJFLAGS) $(wildcard Modules/*.cpp)
Graphics_Object.o : Graphics_Object.cpp Graphics_Object.hpp
	$(OBJFLAGS) Graphics_Object.cpp
Rect.o Text.o Waveform.o : $(wildcard Graphics_Objects/*.cpp) $(wildcard Graphics_Objects/*.hpp)
	$(OBJFLAGS) $(wildcard Graphics_Objects/*.cpp)
signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(OBJFLAGS) signal_processing.cpp
image_processing.o : image_processing.cpp image_processing.hpp
	$(OBJFLAGS) image_processing.cpp
tests.o : tests.cpp tests.hpp
	$(OBJFLAGS) tests.cpp
main.o : main.cpp main.hpp
	$(OBJFLAGS) main.cpp
clean :
		rm synth $(OBJECTS)
