/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:51:42 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/09 15:25:57 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	child_process(t_tree_node *node, int std_fd, t_pipe_info *info)
{
	long long	code;

	dup2(info->pipefd[std_fd], std_fd);
	close(info->pipefd[0]);
	close(info->pipefd[1]);
	info->env->exit_code = execution(node, info->env, info->grbg);
	code = info->env->exit_code;
	exit(code);
}

static void	create_pipe_child(t_tree_node *node, pid_t *pid,
		int std_fd, t_pipe_info *info)
{
	*pid = fork();
	if (*pid == -1)
		return ;
	if (*pid == 0)
		child_process(node, std_fd, info);
}

static void	handle_pipe_logic(t_tree_node *node, int pipe_count,
		t_pipe_info *info)
{
	if (pipe_count == 1)
	{
		create_pipe_child(node->right, &info->write_pid, STDOUT_FILENO, info);
		create_pipe_child(node->left, &info->read_pid, STDIN_FILENO, info);
	}
	else
	{
		create_pipe_child(node->left, &info->write_pid, STDOUT_FILENO, info);
		create_pipe_child(node->right, &info->read_pid, STDIN_FILENO, info);
	}
}

int	pipe_exec(t_tree_node *node, int pipe_count,
		t_envp *env, t_gc_list *grbg)
{
	t_pipe_info	info;
	int			status;

	if (!node->right || (!node->left && !node->redir_arg))
		return (env->exit_code = print_message_and_exit(
				"minishell: syntax error near unexpected token `|'",
				"", 2));
	if (node->redir_arg)
		return (env->exit_code = print_message_and_exit(
				node->redir_arg,
				": command not found", 127));
	if (pipe(info.pipefd) == -1)
		return (1);
	info.env = env;
	info.grbg = grbg;
	handle_pipe_logic(node, pipe_count, &info);
	close(info.pipefd[0]);
	close(info.pipefd[1]);
	waitpid(info.write_pid, &status, 0);
	waitpid(info.read_pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	return (env->exit_code);
}
