/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/01 12:16:15 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (quotes_in_delimiter == 1)
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

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		handle_heredoc_child(node, env);
	else
		waitpid(pid, NULL, 0);
}
