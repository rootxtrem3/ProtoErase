# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LIBS =

# Targets
TARGET = self_destruct
SOURCES = main.c destruct.c security.c config.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = destruct.h security.h config.h

# Default target
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Compile source files with proper dependencies
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Individual dependencies for clarity
main.o: main.c destruct.h security.h config.h
destruct.o: destruct.c destruct.h security.h
security.o: security.c security.h
config.o: config.c config.h destruct.h security.h

# Clean build files
clean:
	rm -f $(TARGET) $(OBJECTS) self_destruct.conf

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install build-essential

# Create test directory for testing
test-dir:
	mkdir -p /tmp/test_destruct
	echo "Test file 1" > /tmp/test_destruct/file1.txt
	mkdir -p /tmp/test_destruct/subdir
	echo "Test file 2" > /tmp/test_destruct/subdir/file2.txt
	echo "Test directory created: /tmp/test_destruct"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: clean install-deps test-dir run debug