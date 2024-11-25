NAME = philo

SRCS = main.c check_actions.c utils.c utils2.c time.c life_cycle.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Werror -Wextra #-fsanitize=thread
#FLAGS = -Wall -Werror -Wextra -fsanitize=address

CC = cc -g

RM = rm -f

#------------------------------------------------

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o:%.c Makefile philo.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
