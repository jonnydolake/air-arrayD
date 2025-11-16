CC = 	  clang
CFLAGS = -Wall -Wextra -std=c23
TARGET =  build/main
SOURCE =  main.c
# Default target
all: $(TARGET)

# Create build directory if it doesn't exist
build:
	mkdir -p build

# Compile the main target
$(TARGET): $(SOURCE) | build
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf build

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
