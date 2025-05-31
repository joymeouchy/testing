/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:51:42 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/31 11:30:12 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	read_from_pipe(t_tree_node *node, pid_t *read_pid, int pipefd[2])
{
	*read_pid = fork();
	if (*read_pid == -1)
		return ;
	if (*read_pid == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		execution(node);
		exit(0);
	}
}

void	write_to_pipe(t_tree_node *node, pid_t *write_pid, int pipefd[2])
{
	*write_pid = fork();
	if (*write_pid == -1)
		return ;
	if (*write_pid == 0)
	{
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		execution(node);
		exit(0);
	}
}

int	count_pipes(t_tree_node *node)
{
	int	count;
	int	left_count;
	int	right_count;
	int	total;

	if (!node)
		return (0);
	count = 0;
	if (node->token == PIPE)
		count = 1;
	left_count = count_pipes(node->left);
	right_count = count_pipes(node->right);
	total = count + left_count + right_count;
	return (total);
}

void	pipe_exec(t_tree_node *node, int pipe_count)
{
	int		pipefd[2];
	pid_t	read_pid;
	pid_t	write_pid;

	if (pipe(pipefd) == -1)
		return ;
	if (pipe_count == 1)
	{
		write_to_pipe(node->right, &write_pid, pipefd);
		read_from_pipe(node->left, &read_pid, pipefd);
	}
	else
	{
		write_to_pipe(node->left, &write_pid, pipefd);
		read_from_pipe(node->right, &read_pid, pipefd);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(write_pid, NULL, 0);
	waitpid(read_pid, NULL, 0);
}
