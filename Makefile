CC = gcc

CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall

LIBS = `pkg-config --libs gtk+-3.0` -Wall

# File names

SRC = sgwelcome.c

OBJ = $(SRC:.c=.o)

EXE = sgwelcome

# Build executable files

all: $(EXE)

sgwelcome: sgwelcome.o

	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Debug step

debug:

	$(CC) $(CFLAGS) -g $(SRC) -o debug $(LIBS)

# Test step

test:

	./sgwelcome

# Clean object files and executables

clean:

	rm -f $(OBJ) $(EXE) debug
