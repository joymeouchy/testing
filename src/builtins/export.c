/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:50:44 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/10 21:59:33 by root             ###   ########.fr       */
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

// static void	duplicate_environment(t_envp *env)
// {
// 	int		size;
// 	int		i;
// 	char	**copy;

// 	size = count_env_vars(env->environment);
// 	i = 0;
// 	copy = malloc(sizeof(char *) * (size + 1));
// 	if (!copy)
// 		return ;
// 	while (i < size)
// 	{
// 		copy[i] = ft_strdup(env->environment[i]);
// 		i++;
// 	}
// 	copy[i] = NULL;
// 	if (env->is_malloced)
// 		free_env(env->environment);
// 	env->environment = copy;
// 	env->is_malloced = 1;
// }

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

// static void	add_new_var(char *arg, t_envp *env)
// {
// 	int		size;
// 	int		i;
// 	char	**new_env;

// 	size = count_env_vars(env->environment);
// 	i = 0;
// 	new_env = malloc(sizeof(char *) * (size + 2));
// 	if (!new_env)
// 		return ;
// 	while (env->environment && env->environment[i])
// 	{
// 		new_env[i] = ft_strdup(env->environment[i]);
// 		i++;
// 	}
// 	new_env[i++] = ft_strdup(arg);
// 	new_env[i] = NULL;
// 	// if (env->is_malloced)
// 	// 	free_env(env->environment);
// 	env->environment = new_env;
// 	// env->is_malloced = 1;
// }

static void	add_new_var(char *arg, t_envp *env, int to_export_only)
{
	char	***target;
	int		size;
	int		i;
	char	**new_env;

	target = to_export_only ? &env->export_only : &env->environment;
	size = count_env_vars(*target);
	i = 0;
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return ;
	while (*target && (*target)[i])
	{
		new_env[i] = ft_strdup((*target)[i]);
		i++;
	}
	new_env[i++] = ft_strdup(arg);
	new_env[i] = NULL;
	free_env(*target);
	*target = new_env;
}

// void	update_env(char *arg, t_envp *env)
// {
// 	// if (env->environment && !ft_strchr(env->environment[0], '='))
// 	// 	duplicate_environment(env);
// 	if (!ft_strchr(arg, '='))
// 	{
// 		if (!var_exists(arg, env->environment))
// 			add_new_var(arg, env);
// 	}
// 	else
// 		if (!update_existing_var(arg, env->environment))
// 			add_new_var(arg, env);
// }

void	update_env(char *arg, t_envp *env)
{
	if (!ft_strchr(arg, '='))
	{
		// Only add to export_only if not already there
		if (!var_exists(arg, env->export_only))
			add_new_var(arg, env, 1); // 1 means export_only
	}
	else
	{
		// update/add to environment
		if (!update_existing_var(arg, env->environment))
			add_new_var(arg, env, 0); // 0 means environment

		// also ensure it's in export_only
		char *key = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
		if (key && !var_exists(key, env->export_only))
			add_new_var(key, env, 1);
		free(key);
	}
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

// static char **duplicate_environment(t_envp *env)
// {
// 	int		size;
// 	int		i;
// 	char	**copy;

// 	size = count_env_vars(env->environment);
// 	i = 0;
// 	copy = malloc(sizeof(char *) * (size + 1));
// 	if (!copy)
// 		return (NULL);
// 	while (i < size)
// 	{
// 		copy[i] = ft_strdup(env->environment[i]);
// 		i++;
// 	}
// 	copy[i] = NULL;
// 	// if (env->is_malloced)
// 	// 	free_env(env->environment);
// 	// env->is_malloced = 1;
// 	return (copy);
// }

// int	export(t_tree_node *root, t_envp *env)
// {
// 	t_tree_node	*arg;
// 	int			i;
// 	char		**env_duplicate;

// 	i = 0;
// 	if (!root || !env || !env->environment)
// 		return (1);
// 	arg = root->right;
// 	if (!arg)
// 	{
// 		env_duplicate = duplicate_environment(env);
// 		sort_env(env_duplicate);
// 		while (env_duplicate[i])
// 		{
// 			print_key_value(env_duplicate[i]);
// 			i++;
// 		}
// 		free_env(env_duplicate);
// 		return (0);
// 	}
// 	while (arg)
// 	{
// 		if (is_valid_key(arg->data))
// 			update_env(arg->data, env);
// 		else
// 			printf("minishell: export: `%s': not a valid identifier\n",
// 				arg->data);
// 		arg = arg->right;
// 	}
// 	return (0);
// }

int	export(t_tree_node *root, t_envp *env)
{
	t_tree_node	*arg;
	int			i = 0;
	char		**merged;
	int			env_count = count_env_vars(env->environment);
	int			exp_count = count_env_vars(env->export_only);

	if (!root || !env)
		return (1);

	arg = root->right;
	if (!arg)
	{
		// Print mode
		merged = malloc(sizeof(char *) * (env_count + exp_count + 1));
		if (!merged)
			return (1);

		for (i = 0; i < env_count; i++)
			merged[i] = ft_strdup(env->environment[i]);
		for (int j = 0; j < exp_count; j++)
			merged[i++] = ft_strdup(env->export_only[j]);
		merged[i] = NULL;

		sort_env(merged);
		i = 0;
		while (merged[i])
			print_key_value(merged[i++]);
		free_env(merged);
		return (0);
	}

	// Export args
	while (arg)
	{
		if (is_valid_key(arg->data))
			update_env(arg->data, env);
		else
			printf("minishell: export: `%s': not a valid identifier\n", arg->data);
		arg = arg->right;
	}
	return (0);
}
