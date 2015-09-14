CC = g++ -g -Wall
CFLAGS = -std=c++11
LDFLAGS = -lSDL2
OBJECTS = Module.o Output.o Oscillator.o signal_processing.o image_processing.o tests.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)
Module.o : Module.cpp Module.hpp
	$(CC) $(CFLAGS) -c Module.cpp
Output.o : Output.cpp Output.hpp
	$(CC) $(CFLAGS) -c Output.cpp
Oscillator.o : Oscillator.cpp Oscillator.hpp
	$(CC) $(CFLAGS) -c Oscillator.cpp
signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(CC) $(CFLAGS) -c signal_processing.cpp
image_processing.o : image_processing.cpp image_processing.hpp
	$(CC) $(CFLAGS) -c image_processing.cpp
tests.o : tests.cpp tests.hpp
	$(CC) $(CFLAGS) -c tests.cpp
main.o : main.cpp main.hpp
	$(CC) $(CFLAGS) -c main.cpp
clean :
		rm synth $(OBJECTS)
