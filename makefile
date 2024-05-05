
CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic

SRCS = main.c keys.c aes_implementation.c utils.c blowfish_implementation.c rsa_implementation.c

OBJS = $(SRCS:.c=.o)

TARGET = ScProject.exe

GMP_INCLUDE = -I/path/to/gmp/include

LIBS = -L/path/to/gmp/lib -lgmp -lm

%.o: %.c
	$(CC) $(CFLAGS) $(GMP_INCLUDE) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

# Clean 
clean:
	rm -f $(OBJS) $(TARGET)

# Dependencies
main.o: main.c blowfish_implementation.h
keys.o: keys.c keys.h
aes_implementation.o: aes_implementation.c aes_implementation.h
utils.o: utils.c utils.h
blowfish_implementation.o: blowfish_implementation.c blowfish_implementation.h utils.h
rsa_implementation.o: rsa_implementation.c rsa_implementation.h
