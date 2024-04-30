CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
LFLAGS = -lm -lncurses

NAME = spendings

SRC = main.c days.c calculations.c files.c
DEPS = header.h

OBJS = main.o days.o calculations.o files.o

YOU:$(SRC) $(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)

%.o:%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(NAME)
