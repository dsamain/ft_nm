NAME			= ft_nm

SRCS			= $(addprefix srcs/, main.c utils.c common.c ft_nm_64.c ft_nm_32.c)
OBJS			= $(SRCS:.c=.o)

#CFLAGS			= -Wall -Wextra -Werror 


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all:			$(NAME)

test: all
	./ft_nm ft_nm

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