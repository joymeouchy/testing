/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:50:26 by root              #+#    #+#             */
/*   Updated: 2025/07/31 22:51:22 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

int	var_exists(char *arg, char **env)
{
	int		i;
	int		key_len;

	key_len = 0;
	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	i = 0;
	while (env && env[i])
	{
		if ((ft_strncmp(env[i], arg, key_len) == 0)
			&& (env[i][key_len] == '\0' || env[i][key_len] == '='))
			return (1);
		i++;
	}
	return (0);
}

char	*extract_key(const char *arg, t_gc_list *grbg_collector)
{
	char	*equal;
	char	*key;
	int		key_len;

	if (!arg)
		return (NULL);
	equal = ft_strchr(arg, '=');
	if (equal)
		key_len = equal - arg;
	else
		key_len = ft_strlen(arg);
	key = ft_substr(arg, 0, key_len, grbg_collector);
	return (key);
}

void	copy_except_key(char **old_env, char **new_env
	, char *key, t_gc_list *grbg_collector)
{
	int	i;
	int	j;
	int	key_len;

	i = 0;
	j = 0;
	key_len = ft_strlen(key);
	while (old_env[i])
	{
		if ((ft_strncmp(old_env[i], key, key_len) != 0)
			|| (old_env[i][key_len] != '\0' && old_env[i][key_len] != '='))
		{
			new_env[j] = ft_strdup(old_env[i], grbg_collector);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
}
