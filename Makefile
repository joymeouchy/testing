NAME = minishell
LIBFT= ./libft/libft.a

CC = gcc
CFLAGS = -Werror -Wall -Wextra -g
LFLAGS= -lreadline -L/usr/local/opt/readline/Lib -I/usr/local/opt/readline/include

SRC =   src/parser/parser.c \
		src/parser/remove_quotes.c \
		src/parser/expand.c \
		src/parser/utils.c \
		src/parser/tokenize.c \
		src/parser/get_env.c \
		src/builtins/echo.c\
		src/builtins/pwd.c \
		src/builtins/env.c \
		src/builtins/export.c \
		src/builtins/unset.c \
		src/builtins/cd.c \
		src/builtins/exit.c \
		src/parser/parsing_main.c\
		src/structures/free_structures.c \
		src/structures/linked_list_utils.c\
		src/structures/stack_utils.c\
        src/structures/linked_list_utils2.c\
		src/parser/shunting_yard.c\
        src/structures/tree_utils.c\
		src/structures/redirections.c\
		src/execution/signals.c\
		src/execution/exec_commands.c\
		src/execution/execution.c\
		src/main.c\
		src/execution/pipe.c\
		src/parser/add_redir_argument.c\

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LFLAGS)

$(LIBFT):
	make -C ./libft


%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
		rm -f $(OBJ)
		make -C ./libft clean

fclean: clean
		rm -f $(NAME)
		make -C ./libft fclean

re: fclean all
.PHONY: all clean fclean re