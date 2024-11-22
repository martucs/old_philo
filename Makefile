NAME = philo

SRCS = main_test3.c

OBJS = $(SRCS:.c=.o) #todos archivos que se llaman igual que los .c pero terminados en .o (aun no creados)

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
