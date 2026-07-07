CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = libC.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
