CC = gcc
CFLAGS = -Wall -g
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs)

OBJS = game.o main.o
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(SDL2_LDFLAGS) -o $(TARGET)

game.o: game.c game.h
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c game.c

main.o: main.c game.h
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c main.c

clean:
	rm -f $(OBJS) $(TARGET)

