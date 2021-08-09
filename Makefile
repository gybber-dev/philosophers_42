NAME		= philo

SRCS	= 	main.c \
			init.c \
			message.c \
			simulation.c \
			timer.c \
			utils.c

OBJS		= $(patsubst %.c, %.o, $(SRCS))
CC			= gcc
FLAGS		= -Wall -Wextra -Werror -pthread
HEADER		= philosophers.h
RM			= rm -f

all:		${NAME}

$(NAME): $(HEADER) $(SRCS)
			gcc $(SRCS) $(FLAGS) -o $(NAME)

%.o : %.c
			${CC} $(FLAGS) -c $< -o $@

clean:
			${RM} ${OBJS} ${BOBJS} *.out

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

norm:
			norminette $(SRCS)
			norminette $(HEADER)

.PHONY:		all clean fclean re norm