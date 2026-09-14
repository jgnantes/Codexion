NAME = codexion
SRC = 
CC = gcc
FLAGS = -Wall -Wextra -Werror -pthread


all: $(NAME)

$(NAME): $(OBJS)
        $(AR) $(NAME) $(OBJS)

%.o: %.c
        $(CC) $(CFLAGS) -c $< -o $@