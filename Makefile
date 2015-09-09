CC = g++ -g -Wall
#CFLAGS =
LDFLAGS = -lSDL2
OBJECTS = Module.o Output.o Oscillator.o signal_processing.o image_processing.o tests.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
# $(CFLAGS)
Module.o : Module.cpp Module.hpp
	$(CC) -c Module.cpp
Output.o : Output.cpp Output.hpp
	$(CC) -c Output.cpp
Oscillator.o : Oscillator.cpp Oscillator.hpp
	$(CC) -c Oscillator.cpp
signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(CC) -c signal_processing.cpp
image_processing.o : image_processing.cpp image_processing.hpp
	$(CC) -c image_processing.cpp
tests.o : tests.cpp tests.hpp
	$(CC) -c tests.cpp
main.o : main.cpp main.hpp
	$(CC) -c main.cpp
clean :
		rm synth $(OBJECTS)
