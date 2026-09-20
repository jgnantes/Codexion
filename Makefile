NAME = codexion
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC = main.c src/dongle_utils.c src/heap_utils.c src/monitor_utils.c \
	src/print_utils.c src/request_utils.c src/scheduler_utils.c \
	src/startup_utils.c src/state_utils.c src/thread_utils.c src/time_utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I src -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all