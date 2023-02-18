NAME		= minishell
CC	 		= cc 
INCLUDES	= -I includes
CFLAGS		= -Wall -Wextra -Werror -fsanitize=address -g $(INCLUDES)
LIBS		= -lreadline
SRCS		= srcs/main.c \
				srcs/error.c \
				srcs/tokenize.c \
				srcs/destructer.c \
				srcs/parse.c \
				srcs/expand.c \
				srcs/redirect.c \
				srcs/pipe.c

OBJS		= $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
test: all
	./test.sh
.PHONY: all clean fclean re test
