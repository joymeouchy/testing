/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:04:58 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/24 10:07:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_builtin(t_tree_node *node, t_envp *env)
{
	if (ft_strcmp(node->data, "echo") == 0)
		return (echo(node));
	else if (ft_strcmp(node->data, "cd") == 0)
		return (cd(node, env));
	else if (ft_strcmp(node->data, "export") == 0)
		return (export(node, env));
	else if (ft_strcmp(node->data, "pwd") == 0)
		return (pwd());
	else if (ft_strcmp(node->data, "unset") == 0)
		return (unset(node, env));
	else if (ft_strcmp(node->data, "exit") == 0)
		return (exit_builtin(node, env));
	else if (ft_strcmp(node->data, "env") == 0)
		return (env_getter(env));
	return (env->exit_code);
}

char	*get_path_with_command(t_tree_node *node)
{
	char	*path_command;
	int		i;

	i = 0;
	while (node->path->split_path[i])
	{
		path_command = ft_strjoin(node->path->split_path[i], node->data);
		if (access(path_command, X_OK | F_OK) == 0)
		{
			return (path_command);
		}
		free(path_command);
		i++;
	}
	return (NULL);
}

char	**fill_arguments(t_tree_node *node)
{
	t_tree_node	*temp_node;
	char		**args;
	int			i;

	temp_node = node;
	i = 0;
	while (temp_node)
	{
		i++;
		temp_node = temp_node->right;
	}
	args = malloc(sizeof(char *) * (i + 2));
	if (!args)
		return (NULL);
	temp_node = node;
	i = 0;
	while (temp_node)
	{
		args[i] = temp_node->data;
		i++;
		temp_node = temp_node->right;
	}
	args[i] = NULL;
	return (args);
}

int	exec_cmd(t_tree_node *node, t_envp *env)
{
	char	**args;
	char	*path;
	int		status;
	pid_t	pid;

	path = get_path_with_command(node);
	args = fill_arguments(node);
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, node->path->environment);
		perror("execve failed");
		env->exit_code = 1;
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork failed");
	free(path);
	free(args);
	return (0);
}

int	exec_commands(t_tree_node *node, t_envp *env)
{
	if (node->token == BUILT_IN)
		return (exec_builtin(node, env));
	if (node->token == COMMAND)
		return (exec_cmd(node, env));
	return (print_message_and_exit(node->data, " : command not found", 127));
}
