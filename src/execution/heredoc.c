/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/29 20:57:06 by lkhoury          ###   ########.fr       */
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

static void	write_heredoc_to_file(int temp_fd, char *delimiter, t_envp *env, t_gc_list *grbg_collector)
{
	char	*line;
	int		quotes_in_delimiter;

	quotes_in_delimiter = check_if_contains_quotes(delimiter);
	remove_quotes_from_string(delimiter);
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (quotes_in_delimiter == 0)
			line = expand(line, env, grbg_collector);
		if (!line)
		{
			ft_putendl_fd("expand returned NULL", 2);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
}

static void	handle_heredoc_child(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	int		temp_fd;

	temp_fd = open_heredoc_file();
	if (temp_fd == -1)
	{
		ft_free_gc(grbg_collector);
		exit(1);
	}
	write_heredoc_to_file(temp_fd, node->redir_arg, env, grbg_collector);
	close(temp_fd);
	redirect_stdin_and_exec(node, "heredoc_temp.txt", env, grbg_collector);
}

void	heredoc(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	pid_t	pid;
	int status;

	pid = fork(); //TODO add ext code
	if (pid == -1)
		return ;
	if (pid == 0 && node->redir_arg != NULL)
	{
		restore_signals_heredoc();
		handle_heredoc_child(node, env, grbg_collector);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
            env->exit_code = WEXITSTATUS(status);
	}
}
