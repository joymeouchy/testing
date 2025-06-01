/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/01 12:26:15 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env)
{
	int	fd;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening temporary heredoc file for reading");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Error redirecting stdin to heredoc");
		close(fd);
		exit(1);
	}
	close(fd);
	execution(node->right, env);
	exit(1);
}

static void	redirect_stdout_and_exec(t_tree_node *node, char *file_name, int open_flag, t_envp *env)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | open_flag, 0777);
	if (fd == -1)
	{
		perror("Error opening file for writing");
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error redirecting stdout to file");
		close(fd);
		exit(1);
	}
	close(fd);
	execution(node->right, env);
	exit(1);
}


void	redir_input(t_tree_node *node, t_envp *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdin_and_exec(node, node->redir_arg, env);
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	redir_output(t_tree_node *node, t_envp *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdout_and_exec(node, node->redir_arg, O_TRUNC, env);
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	redir_output_append(t_tree_node *node, t_envp *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		redirect_stdout_and_exec(node, node->redir_arg, O_APPEND, env);
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	handle_recirections(t_tree_node *node, t_envp *env)
{
	if (node->token == LEFT_REDIRECTION)
		redir_input(node, env);
	else if (node->token == RIGHT_REDIRECTION)
		redir_output(node, env);
	else if (node->token == RIGHT_D_REDIRECTION)
		redir_output_append(node, env);
	else if (node->token == LEFT_D_REDIRECTION)
		heredoc(node, env);
}
