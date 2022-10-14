NAME			= ft_nm

SRCS			= $(addprefix srcs/, main.c utils.c)
OBJS			= $(SRCS:.c=.o)

#CFLAGS			= -Wall -Wextra -Werror 

all:			$(NAME)

$(NAME):		$(OBJS)
				gcc $(OBJS) -o $(NAME)

run: all
	./$(NAME)

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)


.PHONY:			all clean fclean re bonus