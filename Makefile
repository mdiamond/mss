CC = g++ -g -Wall
CFLAGS = -std=c++11
LDFLAGS = -lSDL2
OBJFLAGS = $(CC) $(CFLAGS) -c
OBJECTS = Module.o Output.o Oscillator.o signal_processing.o image_processing.o tests.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
Module.o : Module.cpp Module.hpp
	$(OBJFLAGS) Module.cpp
Output.o : Output.cpp Output.hpp
	$(OBJFLAGS) Output.cpp
Oscillator.o : Oscillator.cpp Oscillator.hpp
	$(OBJFLAGS) Oscillator.cpp
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
