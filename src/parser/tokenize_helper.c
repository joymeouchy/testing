/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:04:17 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/20 20:20:04 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

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

static int	is_executable_file(const char *path)
{
	struct stat	sb;

	if (access(path, X_OK) != 0)
		return (0);
	if (stat(path, &sb) != 0)
		return (0);
	if (!S_ISREG(sb.st_mode))
		return (0);
	return (1);
}

static int	check_path_access(char *cmd, char **paths, int cmd_len)
{
	int		i;
	int		path_len;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]);
		full_path = malloc(path_len + 1 + cmd_len + 1);
		if (!full_path)
			return (0);
		ft_strlcpy(full_path, paths[i], path_len + 1);
		full_path[path_len] = '/';
		ft_strlcpy(full_path + path_len + 1, cmd, cmd_len + 1);
		if (is_executable_file(full_path) == 1)
		{
			free(full_path);
			return (1);
		}
		free(full_path);
		i++;
	}
	return (0);
}

int	is_command(char *cmd, t_envp *envp)
{
	int	cmd_len;

	if (!cmd || !envp || !envp->split_path)
		return (0);
	cmd_len = ft_strlen(cmd);
	return (check_path_access(cmd, envp->split_path, cmd_len));
}
