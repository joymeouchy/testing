/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:04:58 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/30 19:46:14 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void exec_builtin(t_tree_node *node)
{
    if (ft_strcmp(node->data, "echo") == 0)
    {
        echo(node, node->path);
    }
    else if (ft_strcmp(node->data, "cd") == 0)
    {
        cd(node, node->path);
    }
    else if (ft_strcmp(node->data, "export") == 0)
    {
        export(node, node->path);
    }
    else if (ft_strcmp(node->data, "pwd") == 0)
    {
        pwd(); 
    }
    else if (ft_strcmp(node->data, "unset") == 0)
    {
        unset(node, node->path);
    }
    else if (ft_strcmp(node->data, "exit") == 0)
    {
        exit_builtin(node, node->path);
    }
    // else if (ft_strcmp(node->data, "env") == 0)
        // TO-DO: implement env
}

char *get_path_with_command(t_tree_node *node)
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

char **fill_arguments(t_tree_node *node)
{
    t_tree_node *temp_node;
    char        **args;
    int         i;

    temp_node = node;
    i = 0;
    while(temp_node)
    {
        i++;
        temp_node = temp_node->right;
    }
    args = malloc(sizeof(char *) * (i + 2));
    if (!args)
        return (NULL);

    temp_node = node;
    i = 0;
    while(temp_node)
    {
        args[i] = temp_node->data;
        i++;
        temp_node = temp_node->right;
    }
    args[i] = NULL;

    return(args);
}

void    exec_cmd(t_tree_node *node)
{
    char        **args;
    char        *path;
    int         status;
    pid_t       pid;

    path = get_path_with_command(node);
    args = fill_arguments(node);
    pid = fork();
    if (pid == 0)
    {
        execve(path, args, node->path->environment);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
        perror("fork failed");
    free(path);
    free(args);
}


void exec_commands(t_tree_node *node)
{
    if (node->token == BUILT_IN)
        exec_builtin(node);
    if (node->token == COMMAND)
        exec_cmd(node);
}
