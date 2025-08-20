NAME = minishell

LIBFT = ./libft/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include

SRC =   src/parser/parser.c \
        src/parser/parser_helper.c \
        src/parser/parser_helper_split.c \
        src/parser/remove_quotes.c \
        src/parser/remove_quotes_helper.c \
        src/parser/expand_helper.c \
        src/parser/expand_helper2.c \
        src/parser/expand.c \
        src/parser/expand_quotes.c\
        src/parser/utils.c \
        src/parser/tokenize.c \
        src/parser/tokenize_helper.c \
        src/parser/get_env.c \
        src/builtins/echo.c \
        src/builtins/pwd.c \
        src/builtins/env.c \
        src/builtins/export.c \
        src/builtins/export_helper.c \
        src/builtins/export_helper2.c \
        src/builtins/export_helper3.c \
        src/builtins/unset.c \
        src/builtins/cd.c \
        src/builtins/cd_helper.c \
        src/builtins/exit.c \
        src/builtins/exit_helper.c \
        src/parser/parsing_main.c \
        src/parser/parsing_main_helper.c \
        src/structures/linked_list_utils.c \
        src/structures/stack_utils.c \
        src/structures/linked_list_utils2.c \
        src/parser/shunting_yard.c \
        src/structures/tree_helper.c \
        src/structures/tree_utils.c \
        src/execution/redirections.c \
        src/execution/redirections_helper.c \
        src/execution/signals.c \
        src/execution/exec_commands.c \
        src/execution/execution.c \
        src/main.c \
        src/execution/pipe.c \
        src/parser/add_redir_argument.c \
        src/structures/redir_swap.c \
        src/execution/heredoc.c \
        src/syntax_error.c \
	src/ft_malloc.c \
        src/structures/tree_builder.c \
        src/execution/heredoc_helper.c \
        src/parser/add_pipe_argument.c \
        src/parser/expand_for_heredoc.c \
        src/parser/expand_heredoc_helper.c
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
