/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/25 21:47:40 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env)
// {
// 	int	fd;

// 	fd = open(file_name, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		printf("no such file"); //TODO write a better message
// 		env->exit_code = 2;
// 		exit(2);
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		env->exit_code = 2;
// 		close(fd);
// 		exit(2);
// 	}
// 	close(fd);
// 	unlink("heredoc_temp.txt");
// 	execution(node->right, env);
// 	exit(1);
// }
void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env, t_gc_list *grbg_collector)
{
	int	fd;
	long long	exit_code;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		env->exit_code = 1;
		ft_free_gc(grbg_collector);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		env->exit_code = 1;
		ft_free_gc(grbg_collector);
		exit(1);
	}
	close(fd);
	unlink("heredoc_temp.txt");
	execution(node->right, env, grbg_collector);
	exit_code = env->exit_code;
	ft_free_gc(grbg_collector);
	exit(exit_code);
}

// static void	redirect_stdout_and_exec(t_tree_node *node,
// 	char *file_name, int open_flag, t_envp *env)
// {
// 	int	fd;

// 	fd = open(file_name, O_WRONLY | O_CREAT | open_flag, 0777);
// 	if (fd == -1)
// 	{
// 		env->exit_code = 2;
// 		exit(2);
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("Error redirecting stdout to file");
// 		close(fd);
// 		exit(1);
// 	}
// 	close(fd);
// 	execution(node->right, env);
// 	exit(1);
// }
static void	redirect_stdout_and_exec(t_tree_node *node,
	char *file_name, int open_flag, t_envp *env, t_gc_list *grbg_collector)
{
	int	fd;
	long long	exit_code;

	fd = open(file_name, O_WRONLY | O_CREAT | open_flag, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		env->exit_code = 1;
		ft_putstr_fd("hellloooooooooooo\n", 1);
		ft_free_gc(grbg_collector);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2 failed: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		close(fd);
		env->exit_code = 1;
		ft_free_gc(grbg_collector);
		exit(1);
	}
	close(fd);
	execution(node->right, env, grbg_collector);
	exit_code = env->exit_code;
	ft_free_gc(grbg_collector);
	exit(exit_code);
}


void	redir_input(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdin_and_exec(node, node->redir_arg, env, grbg_collector);
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		if (WIFEXITED(status))
            env->exit_code = WEXITSTATUS(status);
	}
}
void	redir_output(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdout_and_exec(node, node->redir_arg, O_TRUNC, env, grbg_collector);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            env->exit_code = WEXITSTATUS(status);
	}
}

void	redir_output_append(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdout_and_exec(node, node->redir_arg, O_APPEND, env, grbg_collector);
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		if (WIFEXITED(status))
            env->exit_code = WEXITSTATUS(status);
	}
}


int	handle_recirections(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	if (node->right && !(node->right->token >= 1 || node->right->token <=6))
	{
		return (env->exit_code = print_message_and_exit(
				"minishell: syntax error near unexpected token `newline'",
				"", 2));
	}
	else if (node->redir_arg == NULL)
		return (env->exit_code = print_message_and_exit(
				"minishell: syntax error near unexpected token `newline'",
				"", 2));
	 if (node->token == LEFT_REDIRECTION)
		redir_input(node, env, grbg_collector);
	else if (node->token == RIGHT_REDIRECTION)
		redir_output(node, env, grbg_collector);
	else if (node->token == RIGHT_D_REDIRECTION)
		redir_output_append(node, env, grbg_collector);
	else if (node->token == LEFT_D_REDIRECTION)
		heredoc(node, env, grbg_collector);
	return (0);
}
