CC = gcc
CFLAGS = -Wall -Wreturn-type -Wextra -g -pedantic -std=c11
SRCS = src/struct.c src/struct.h main.c
OBJS = $(SRCS:.c=.o)
EXEC = thanhchris

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name '*.o' -type f -delete
	rm -f $(EXEC)

rebuild: clean all
