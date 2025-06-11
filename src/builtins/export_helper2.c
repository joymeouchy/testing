/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:57:06 by root              #+#    #+#             */
/*   Updated: 2025/06/11 21:58:21 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_var_by_key(char *arg, char ***env)
{
	char	*key;
	int		size;
	char	**new_env;

	if (!env || !(*env))
		return ;
	key = extract_key(arg);
	if (!key)
		return ;
	size = count_env_vars(*env);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
	{
		free(key);
		return ;
	}
	copy_except_key(*env, new_env, key);
	free_env(*env);
	*env = new_env;
	free(key);
}
int	find_key_index(char *key, int key_len, char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if ((ft_strncmp(env[i], key, key_len) == 0) &&
			(env[i][key_len] == '\0' || env[i][key_len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	replace_existing_key(char *arg, char ***env)
{
	char	*equal;
	char	*key;
	int		key_len;
	int		index;

	equal = strchr(arg, '=');
	if (equal)
		key_len = equal - arg;
	else
		key_len = ft_strlen(arg);
	key = ft_substr(arg, 0, key_len);
	if (!key || !env || !(*env))
		return (0);
	index = find_key_index(key, key_len, *env);
	if (index >= 0)
	{
		free((*env)[index]);
		(*env)[index] = ft_strdup(arg);
		free(key);
		return (1);
	}
	free(key);
	return (0);
}

void	add_new_var(char *arg, char ***env)
{
	char	**new_env;
	int		size;
	int		i;

	if (!env)
		return ;
	if (replace_existing_key(arg, env))
		return ;
	size = count_env_vars(*env);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return ;
	i = 0;
	while (*env && (*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free_env(*env);
	*env = new_env;
}

int	is_valid_key(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
