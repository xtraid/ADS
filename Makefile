CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -I.

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = libads.a

TEST_SRCS = $(wildcard tests/*.c)
TEST_BINS = $(TEST_SRCS:.c=)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Compila ogni tests/foo.c -> tests/foo
tests/%: tests/%.c $(TARGET)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -L. -lads -o $@

test: $(TEST_BINS)
	@for test in $(TEST_BINS); do \
		echo "==> Running $$test"; \
		./$$test || exit 1; \
	done

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_BINS)
