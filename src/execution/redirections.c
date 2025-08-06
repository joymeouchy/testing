/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/06 16:48:56 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env,
		t_gc_list *grbg_collector)
{
	int			fd;
	long long	exit_code;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		env->exit_code = print_message_and_exit("minishell: ",
				file_name, 1);
		print_message_and_exit(": ", strerror(errno), 1);
		ft_free_gc(grbg_collector);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		env->exit_code = print_message_and_exit("minishell: dup2 failed: ",
				strerror(errno), 1);
		close(fd);
		ft_free_gc(grbg_collector);
		exit(1);
	}
	close(fd);
	//unlink(file_name); // hay bas nshila bibatil 3ende norm error
	execution(node->right, env, grbg_collector);
	exit_code = env->exit_code;
	ft_free_gc(grbg_collector);
	exit(exit_code);
}

static void	redirect_stdout_and_exec(t_tree_node *node,
		int open_flag, t_envp *env, t_gc_list *grbg_collector)
{
	int			fd;
	long long	exit_code;

	fd = open(node->redir_arg, O_WRONLY | O_CREAT | open_flag, 0644);
	if (fd == -1)
	{
		env->exit_code = print_message_and_exit("minishell: ",
				node->redir_arg, 1);
		print_message_and_exit(": ", strerror(errno), 1);
		ft_free_gc(grbg_collector);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		env->exit_code = print_message_and_exit("minishell: dup2 failed: ",
				strerror(errno), 1);
		close(fd);
		ft_free_gc(grbg_collector);
		exit(1);
	}
	close(fd);
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

int	redir_output(t_tree_node *node, t_envp *env, int open_fd_flag, t_gc_list *grbg_collector)
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
