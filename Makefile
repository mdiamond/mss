CC = g++ -g -Wall
#CFLAGS =
LDFLAGS = -lSDL2
OBJECTS = main.o

all : synth

synth : $(OBJECTS)
		$(CC) -o $@ $^ $(LDFLAGS)
# $(CFLAGS)
main.o : main.cpp
		$(CC) -c main.cpp
clean :
		rm synth $(OBJECTS)
