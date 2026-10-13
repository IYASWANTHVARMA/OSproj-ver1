CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2 -pthread
TARGET = pagesim
SOURCE = src/pagesim.c

.PHONY: all clean test
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

test: $(TARGET)
	./tests/run_tests.sh

clean:
	rm -f $(TARGET)
