/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/06 22:35:57 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_stdin_and_exec(t_tree_node *node, char *file, t_envp *env,
		t_gc_list *grbg_collector)
{
	int			fd;
	long long	exit_code;

	fd = open_file_for_redirect(file, O_RDONLY, env, grbg_collector);
	dup_and_close(fd, STDIN_FILENO, env, grbg_collector);
	if (ft_strncmp(file, "heredoc_temp", 13))
		unlink(file);
	execution(node->right, env, grbg_collector);
	exit_code = env->exit_code;
	ft_free_gc(grbg_collector);
	exit(exit_code);
}

void	redirect_stdout_and_exec(t_tree_node *node, int flag, t_envp *env,
		t_gc_list *grbg_collector)
{
	int			fd;
	long long	exit_code;

	fd = open_file_for_redirect(node->redir_arg, O_WRONLY | O_CREAT | flag, env,
			grbg_collector);
	dup_and_close(fd, STDOUT_FILENO, env, grbg_collector);
	execution(node->right, env, grbg_collector);
	exit_code = env->exit_code;
	ft_free_gc(grbg_collector);
	exit(exit_code);
}

int	redir_input(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		redirect_stdin_and_exec(node, node->redir_arg, env, grbg_collector);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			env->exit_code = WEXITSTATUS(status);
	}
	return (env->exit_code);
}

int	redir_output(t_tree_node *node, t_envp *env, int open_fd_flag,
		t_gc_list *grbg_collector)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		redirect_stdout_and_exec(node, open_fd_flag, env, grbg_collector);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			env->exit_code = WEXITSTATUS(status);
	}
	return (env->exit_code);
}

int	handle_recirections(t_tree_node *node, t_envp *env,
		t_gc_list *grbg_collector)
{
	if (node->right && !(node->right->token >= 1 || node->right->token <= 6))
	{
		return (env->exit_code = print_message_and_exit("minishell: ",
				"syntax error near unexpected token `newline'", 2));
	}
	else if (node->redir_arg == NULL)
		return (env->exit_code = print_message_and_exit("minishell: ",
				"syntax error near unexpected token `newline'", 2));
	if (node->token == LEFT_REDIRECTION)
		return (redir_input(node, env, grbg_collector));
	else if (node->token == RIGHT_REDIRECTION)
		return (redir_output(node, env, O_TRUNC, grbg_collector));
	else if (node->token == RIGHT_D_REDIRECTION)
		return (redir_output(node, env, O_APPEND, grbg_collector));
	return (0);
}
