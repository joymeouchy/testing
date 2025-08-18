/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/18 20:59:22 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


char	*open_heredoc_file(int *temp_fd, t_gc_list *grbg_collector,
	int heredoc_counter)
{
	char	*filename;
	char	*base;
	int		fd;
	int		i;

	i = heredoc_counter;
	while (1)
	{
		base = ft_strjoin(ft_strdup("heredoc_temp", grbg_collector),
				ft_itoa(i, grbg_collector), grbg_collector);
		filename = ft_strjoin(base, ".txt", grbg_collector);
		fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd != -1)
		{
			*temp_fd = -1;
			break ;
		}
		if (errno != EEXIST)
		{
			perror("Error creating temporary file for heredoc");
			return (NULL);
		}
		i++;
	}
	*temp_fd = fd;
	return (filename);
}

void	replace_heredoc_node(t_tree_node *node, char *filename)
{
	node->data = "<";
	node->token = LEFT_REDIRECTION;
	node->redir_arg = filename;
	node->heredoc_created = 1;
}

static void	write_heredoc_to_file(int temp_fd, char *delimiter, t_envp *env,
		t_gc_list *grbg_collector)
{
	char	*line;
	int		quotes_in_delimiter;

	quotes_in_delimiter = check_if_contains_quotes(delimiter);
	remove_quotes_from_string(delimiter);
	while (1)
	{
		line = readline("> ");
		if (quotes_in_delimiter == 0)
			line = expand(line, env, grbg_collector);
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, temp_fd);
		free(line);
	}
}

void	ctrl_c_heredoc(int sig)
{
	(void)sig;
	printf("\n");
	g_sigint = 130;
	exit(130);
}
void set_heredoc_signals(void)
{
    signal(SIGINT, ctrl_c_heredoc);
    signal(SIGQUIT, SIG_IGN);
}

static void	heredoc_child(int temp_fd, char *delimiter,
			t_envp *env, t_gc_list *grbg_collector)
{
	set_heredoc_signals();
	if (g_sigint)
		exit(g_sigint);
	write_heredoc_to_file(temp_fd, delimiter, env, grbg_collector);
	close(temp_fd);
	exit(0);
}

static void	heredoc_parent(t_tree_node *node, t_envp *env,
			char *filename)
{
	int	status;

	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	else
		env->exit_code = 130;

	if (env->exit_code == 0 && filename)
		replace_heredoc_node(node, filename);
	else if (filename)
		unlink(filename);
}

void	heredoc(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector,
			int heredoc_counter)
{
	int		temp_fd;
	char	*filename;
	pid_t	pid;

	if (!node->redir_arg)
		return;
	filename = open_heredoc_file(&temp_fd, grbg_collector, heredoc_counter);
	if (!filename || temp_fd == -1)
		return;

	pid = fork();
	if (pid == 0)
		heredoc_child(temp_fd, node->redir_arg, env, grbg_collector);
	else if (pid > 0)
		heredoc_parent(node, env, filename);
}

void	swap_heredoc_node(t_tree_node *node, t_envp *env,
		t_gc_list *grbg_collector, int heredoc_counter)
{
	if (!node || env->exit_code == 130)
		return;
	if (node->token == LEFT_D_REDIRECTION)
	{
		heredoc(node, env, grbg_collector, heredoc_counter);
		if (env->exit_code == 130)
		{
			if (node->redir_arg)
				unlink(node->redir_arg);
			return;
		}
		heredoc_counter++;
	}
	swap_heredoc_node(node->left, env, grbg_collector, heredoc_counter);
	swap_heredoc_node(node->right, env, grbg_collector, heredoc_counter);
}
