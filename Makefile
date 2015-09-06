CC = g++ -g -Wall
#CFLAGS =
LDFLAGS = -lSDL2
OBJECTS = Module.o signal_processing.o tests.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
# $(CFLAGS)
Module.o : Module.cpp Module.hpp
	$(CC) -c Module.cpp
signal_processing.o : signal_processing.cpp signal_processing.hpp
	$(CC) -c signal_processing.cpp
tests.o : tests.cpp tests.hpp
	$(CC) -c tests.cpp
main.o : main.cpp main.hpp
	$(CC) -c main.cpp
clean :
		rm synth $(OBJECTS)
