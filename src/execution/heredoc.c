/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/19 19:10:50 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_child(int temp_fd, char *delimiter, t_envp *env,
		t_gc_list *grbg_collector)
{
	set_heredoc_signals();
	if (g_sigint)
		exit(g_sigint);
	write_heredoc_to_file(temp_fd, delimiter, env, grbg_collector);
	close(temp_fd);
	exit(0);
}

static void	heredoc_parent(t_tree_node *node, t_envp *env, char *filename)
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
		return ;
	filename = open_heredoc_file(&temp_fd, grbg_collector, heredoc_counter);
	if (!filename || temp_fd == -1)
		return ;
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
		return ;
	if (node->token == LEFT_D_REDIRECTION)
	{
		heredoc(node, env, grbg_collector, heredoc_counter);
		if (env->exit_code == 130)
		{
			if (node->redir_arg)
				unlink(node->redir_arg);
			return ;
		}
		heredoc_counter++;
	}
	swap_heredoc_node(node->left, env, grbg_collector, heredoc_counter);
	swap_heredoc_node(node->right, env, grbg_collector, heredoc_counter);
}
