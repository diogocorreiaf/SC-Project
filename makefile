# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Wpedantic

# Source files
SRCS = main.c keys.c aes_implementation.c utils.c blowfish_implementation.c rsa_implementation.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = ScProject.exe

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Clean rule to remove object files and the executable
clean:
	del /Q $(OBJS) $(TARGET)

# Dependencies
main.o: main.c blowfish_implementation.h
keys.o: keys.c keys.h
aes_implementation.o: aes_implementation.c aes_implementation.h
utils.o: utils.c utils.h
blowfish_implementation.o: blowfish_implementation.c blowfish_implementation.h utils.h
rsa_implementation.o: rsa_implementation.c rsa_implementation.h