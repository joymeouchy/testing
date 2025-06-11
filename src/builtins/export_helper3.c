/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:04:06 by root              #+#    #+#             */
/*   Updated: 2025/06/11 22:06:02 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env(char *arg, t_envp *env)
{
	char	*equal;
	char	*key;

	if (!env)
		return ;
	equal = strchr(arg, '=');
	if (!equal)
	{
		key = strdup(arg);
		if (!var_exists(key, env->environment) && !var_exists(key, env->export_only))
			add_new_var(key, &env->export_only);
		free(key);
		return ;
	}
	remove_var_by_key(arg, &env->export_only);
	if (!replace_existing_key(arg, &env->environment))
		add_new_var(arg, &env->environment);
}

void	print_key_value(char *env_var)
{
	char	*equal;
	char	*key;
	char	*value;
	int		key_len;

	equal = strchr(env_var, '=');
	if (!equal)
	{
		printf("declare -x %s\n", env_var);
		return ;
	}
	key_len = equal - env_var;
	key = ft_substr(env_var, 0, key_len);
	value = strdup(equal + 1);
	if (key && value)
		printf("declare -x %s=\"%s\"\n", key, value);
	free(key);
	free(value);
}

void	sort_env(char **env)
{
	int		i;
	int		swapped;
	char	*tmp;
	int		size;

	size = count_env_vars(env);
	if (size < 2)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < size - 1)
		{
			if (strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	copy_env_vars(char **src, char **dst, int *dst_index)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		dst[*dst_index] = ft_strdup(src[i]);
		(*dst_index)++;
		i++;
	}
}

char	**merge_env_vars(t_envp *env)
{
	char	**merged;
	int		env_count;
	int		exp_count;
	int		total_count;
	int		index;

	if (!env)
		return (NULL);
	env_count = count_env_vars(env->environment);
	exp_count = count_env_vars(env->export_only);
	total_count = env_count + exp_count;
	merged = malloc(sizeof(char *) * (total_count + 1));
	if (!merged)
		return (NULL);
	index = 0;
	copy_env_vars(env->environment, merged, &index);
	copy_env_vars(env->export_only, merged, &index);
	merged[index] = NULL;
	return (merged);
}
