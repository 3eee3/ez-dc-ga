CC = g++
LD = g++

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
TARGET = main

CFLAGS = -g -Wall
LDLIBS = -lGL -lGLU -lglut 
INCLUDES = 

SRC_DIR = 
BUILD_DIR = 
VPATH = utils

# Rules
all: $(TARGET)

$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: clean

# Dependencies
$(TARGET): $(OBJ) 

