TARGET := wave2data

CC := clang++
CFLAGS := --std=c++11 -O2

SRC := $(shell ls *.cpp)
HED := $(shell ls *.h)
OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

.cpp.o:
		$(CC) $(CFLAGS) -c $<

clean:
		rm $(TARGET)
		rm *.o
