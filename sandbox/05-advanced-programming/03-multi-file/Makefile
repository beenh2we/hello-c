# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -I./include

# Source files
SRCS = src/math_utils.c src/vector.c

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = main

# Default target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c -o $@ -lm

# Pattern rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
