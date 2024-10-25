CC = gcc
#CFLAGS = -Wall -Wextra

SRC = $(shell find src -name "*.c")
OBJ = $(SRC:.c=.o)
TARGET = langaje
LDFLAGS = -lm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
