CC = g++ -g -Wall
#CFLAGS =
LDFLAGS = -lSDL2
OBJECTS = Module.o signal_processing.o main.o

all : synth

synth : $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
# $(CFLAGS)
Module : Module.cpp Module.hpp
	$(CC) -c Module.cpp
signal_processing : signal_processing.cpp signal_processing.hpp
	$(CC) -c signal_processing.cpp
main.o : main.cpp main.hpp
	$(CC) -c main.cpp
clean :
		rm synth $(OBJECTS)
