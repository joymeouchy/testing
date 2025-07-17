/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:04:58 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/17 18:28:55 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_builtin(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	if (ft_strcmp(node->data, "echo") == 0)
		return (echo(node));
	else if (ft_strcmp(node->data, "cd") == 0)
		return (cd(node, env, grbg_collector));
	else if (ft_strcmp(node->data, "export") == 0)
		return (export(node, env, grbg_collector));
	else if (ft_strcmp(node->data, "pwd") == 0)
		return (pwd());
	else if (ft_strcmp(node->data, "unset") == 0)
		return (unset(node, env));
	else if (ft_strcmp(node->data, "exit") == 0)
		return (exit_builtin(node, env, grbg_collector));
	else if (ft_strcmp(node->data, "env") == 0)
		return (env_getter(env));
	return (env->exit_code);
}

char	*get_path_with_command(t_tree_node *node, t_gc_list *grbg_collector)
{
	char	*path_command;
	int		i;

	i = 0;
	while (node->path->split_path[i])
	{
		path_command = ft_strjoin(node->path->split_path[i], node->data, grbg_collector);
		if (access(path_command, X_OK | F_OK) == 0)
		{
			return (path_command);
		}
		// free(path_command);
		i++;
	}
	return (NULL);
}

char	**fill_arguments(t_tree_node *node, t_gc_list *grgb_collector)
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
	args = ft_malloc(sizeof(char *) * (i + 2), grgb_collector);
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

int	exec_cmd(t_tree_node *node, t_envp *env, t_gc_list *grgb_collector)
{
	char	**args;
	char	*path;
	int		status;
	pid_t	pid;

	path = get_path_with_command(node, grgb_collector);
	args = fill_arguments(node, grgb_collector);
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, node->path->environment);
		perror("execve failed");
		return (env->exit_code = 1);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork failed");
	// free(path);
	// free(args);
	return (env->exit_code);
}

int	exec_commands(t_tree_node *node, t_envp *env, t_gc_list *grgb_collector)
{
	if (node->token == BUILT_IN)
		return (exec_builtin(node, env, grgb_collector));
	if (node->token == COMMAND)
		return (exec_cmd(node, env, grgb_collector));
	return (print_message_and_exit(node->data, " : command not found", 127));
}
