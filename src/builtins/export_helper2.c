/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:57:06 by root              #+#    #+#             */
/*   Updated: 2025/08/13 20:39:17 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_var_by_key(char *arg, char ***env, t_gc_list *grgb_collector)
{
	char	*key;
	int		size;
	char	**new_env;

	if (!env || !(*env))
		return ;
	key = extract_key(arg, grgb_collector);
	if (!key)
		return ;
	size = count_env_vars(*env);
	new_env = ft_malloc(sizeof(char *) * (size + 1), grgb_collector);
	if (!new_env)
	{
		return ;
	}
	copy_except_key(*env, new_env, key, grgb_collector);
	*env = new_env;
}

int	find_key_index(char *key, int key_len, char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if ((ft_strncmp(env[i], key, key_len) == 0)
			&& (env[i][key_len] == '\0' || env[i][key_len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	replace_existing_key(char *arg, char ***env, t_gc_list *grbg_collector)
{
	char	*equal;
	char	*key;
	int		key_len;
	int		index;

	equal = ft_strchr(arg, '=');
	if (equal)
		key_len = equal - arg;
	else
		key_len = ft_strlen(arg);
	key = ft_substr(arg, 0, key_len, grbg_collector);
	if (!key || !env || !(*env))
		return (0);
	index = find_key_index(key, key_len, *env);
	if (index >= 0)
	{
		free((*env)[index]);
		(*env)[index] = ft_strdup(arg, grbg_collector);
		return (1);
	}
	return (0);
}

void	add_new_var(char *arg, char ***env, t_gc_list *grgb_collector)
{
	char	**new_env;
	int		size;
	int		i;

	if (!env)
		return ;
	if (replace_existing_key(arg, env, grgb_collector))
		return ;
	size = count_env_vars(*env);
	new_env = ft_malloc(sizeof(char *) * (size + 2), grgb_collector);
	if (!new_env)
		return ;
	i = 0;
	while (*env && (*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i], grgb_collector);
		i++;
	}
	new_env[i] = ft_strdup(arg, grgb_collector); // leak cause we're not filling it with anything
	new_env[i + 1] = NULL;
	*env = new_env;
}

int	is_valid_key(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_' ))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
