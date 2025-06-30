/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/30 18:49:47 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_sigint = 0;

static int	open_heredoc_file(void)
{
	int	fd;

	fd = open("heredoc_temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("Error creating temporary file for heredoc");
	return (fd);
}

static void	write_heredoc_to_file(int temp_fd, char *delimiter, t_envp *env)
{
	char	*line;
	int		quotes_in_delimiter;

	quotes_in_delimiter = check_if_contains_quotes(delimiter);
	remove_quotes_from_string(delimiter);
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		if (quotes_in_delimiter == 0) //TODO WHY
			line = expand(line, env->environment);
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
}

static void	handle_heredoc_child(t_tree_node *node, t_envp *env)
{
	int		temp_fd;

	temp_fd = open_heredoc_file();
	if (temp_fd == -1)
		exit(1);
	write_heredoc_to_file(temp_fd, node->redir_arg, env);
	close(temp_fd);
	redirect_stdin_and_exec(node, "heredoc_temp.txt", env);
}

void	heredoc(t_tree_node *node, t_envp *env)
{
	pid_t	pid;
	int status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0 && node->redir_arg != NULL)
	{
		signal_in_child();
		handle_heredoc_child(node, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
		if(WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			g_sigint = 1;
		}
	}
}
