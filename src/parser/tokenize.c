/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:23:04 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/28 14:38:59 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    int     i;

    i = 0;
     while (s1[i] != '\0' && s1[i] == s2[i])
            i++;
    return (s1[i] - s2[i]);
}

int	check_builtin(char *data)
{
	char	*builtins[8];
	int		i;

	i = 0;
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	while (builtins[i])
	{
		if (ft_strcmp(data, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}


// int is_command(char *cmd, t_envp *envp)
// {
// 	char	*full_path;
// 	int		i = 0;

// 	while (envp->split_path[i])
// 	{
// 		full_path = malloc(strlen(envp->split_path[i]) + strlen(cmd) + 1);
// 		if (!full_path)
// 			return (0);
// 		strcpy(full_path, envp->split_path[i]);
// 		strcat(full_path, cmd);
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			free(full_path);
// 			return (1);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	return (0);
// }

int is_command(char *cmd, t_envp *envp)
{
    char *full_path;
    int i = 0;
    int cmd_len = strlen(cmd);

    while (envp->split_path[i])
    {
        int path_len = strlen(envp->split_path[i]);
        full_path = malloc(path_len + 1 + cmd_len + 1); // +1 for '/' +1 for '\0'
        if (!full_path)
            return 0; // or handle error

        strcpy(full_path, envp->split_path[i]);
        full_path[path_len] = '/';
        strcpy(full_path + path_len + 1, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(full_path);
            return 1;
        }
        free(full_path);
        i++;
    }
    return 0;
}

void tokenize(t_list *list, t_envp *envp)
{
    t_list_node *temp;
	int flag_command = false;
	temp = list->head;
	
    while (temp)
    {
        if (!temp->data)
            return;
        if (temp->data[0] == '|')
		{
            temp->token = PIPE;
			flag_command = false;
		
		}
		else if (temp->data[0] == '<' && temp->data[1] == '<')
            temp->token = LEFT_D_REDIRECTION;
        else if (temp->data[0] == '>' && temp->data[1] == '>')
            temp->token = RIGHT_D_REDIRECTION;
        else if (temp->data[0] == '<')
            temp->token = LEFT_REDIRECTION;
        else if (temp->data[0] == '>')
            temp->token = RIGHT_REDIRECTION;
        else if (temp->data[0] == '~')
            temp->token = TILDE;
        else if (!flag_command && check_builtin(temp->data))
		{
            temp->token = BUILT_IN;
			flag_command = true;
		}
		else if (!flag_command && is_command(temp->data, envp))
		{
            temp->token = COMMAND;
			flag_command = true;
		}
        else
            temp->token = WORD;
        temp = temp->next;
    }
}


