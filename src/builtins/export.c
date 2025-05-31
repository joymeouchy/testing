/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:50:44 by lkhoury           #+#    #+#             */
/*   Updated: 2025/05/31 12:34:30 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
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

static int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

static void	duplicate_environment(t_envp *env)
{
	int		size;
	int		i;
	char	**copy;

	size = count_env_vars(env->environment);
	i = 0;
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return ;
	while (i < size)
	{
		copy[i] = ft_strdup(env->environment[i]);
		i++;
	}
	copy[i] = NULL;
	if (env->is_malloced)
		free_env(env->environment);
	env->environment = copy;
	env->is_malloced = 1;
}

static int	var_exists(char *arg, char **env)
{
	int		i;
	size_t	arg_len;

	i = 0;
	arg_len = ft_strlen(arg);
	while (env && env[i])
	{
		if ((ft_strncmp(env[i], arg, arg_len) == 0)
			&& (env[i][arg_len] == '\0' || env[i][arg_len] == '='))
			return (1);
		i++;
	}
	return (0);
}

static int	update_existing_var(char *arg, char **env)
{
	char	*equal;
	char	*key;
	int		key_len;
	int		i;

	equal = ft_strchr(arg, '=');
	i = 0;
	if (!equal)
		return (0);
	key_len = equal - arg;
	key = ft_substr(arg, 0, key_len);
	if (!key)
		return (0);
	while (env && env[i])
	{
		if ((ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=') ||
			(ft_strcmp(env[i], key) == 0))
		{
			free(env[i]);
			env[i] = ft_strdup(arg);
			free(key);
			return (1);
		}
		i++;
	}
	free(key);
	return (0);
}

static void	add_new_var(char *arg, t_envp *env)
{
	int		size;
	int		i;
	char	**new_env;

	size = count_env_vars(env->environment);
	i = 0;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return ;
	while (env->environment && env->environment[i])
	{
		new_env[i] = ft_strdup(env->environment[i]);
		i++;
	}
	new_env[i++] = ft_strdup(arg);
	new_env[i] = NULL;
	if (env->is_malloced)
		free_env(env->environment);
	env->environment = new_env;
	env->is_malloced = 1;
}

void	update_env(char *arg, t_envp *env)
{
	if (env->environment && !ft_strchr(env->environment[0], '='))
		duplicate_environment(env);
	if (!ft_strchr(arg, '='))
	{
		if (!var_exists(arg, env->environment))
			add_new_var(arg, env);
	}
	else
		if (!update_existing_var(arg, env->environment))
			add_new_var(arg, env);
}

static void	print_key_value(char *env_var)
{
	char	*equal;
	char	*key;
	char	*value;
	int		key_len;

	equal = ft_strchr(env_var, '=');
	if (equal)
	{
		key_len = equal - env_var;
		key = ft_substr(env_var, 0, key_len);
		value = ft_strdup(equal + 1);
		if (key && value)
			printf("declare -x %s=\"%s\"\n", key, value);
		free(key);
		free(value);
	}
	else
		printf("declare -x %s\n", env_var);
}

static void	sort_env(char **env)
{
	int		i;
	int		j;
	int		swapped;
	char	*tmp;

	i = 0;
	swapped = 1;
	while (env[i])
		i++;
	while (swapped)
	{
		swapped = 0;
		j = 0;
		while (j < i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
				swapped = 1;
			}
			j++;
		}
	}
}

int	export(t_tree_node *root, t_envp *env)
{
	t_tree_node	*arg;
	int			i;

	i = 0;
	if (!root || !env || !env->environment)
		return (1);
	arg = root->right;
	if (!arg)
	{
		sort_env(env->environment);
		while (env->environment[i])
		{
			print_key_value(env->environment[i]);
			i++;
		}
		return (0);
	}
	while (arg)
	{
		if (is_valid_key(arg->data))
			update_env(arg->data, env);
		else
			printf("minishell: export: `%s': not a valid identifier\n",
				arg->data);
		arg = arg->right;
	}
	return (0);
}
