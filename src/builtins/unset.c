/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:06:11 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/15 13:41:47 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	match_key_env(const char *env_entry, const char *key)
{
	int	i;

	i = 0;
	while (env_entry[i] && key[i] && env_entry[i] == key[i])
		i++;
	return (key[i] == '\0' && env_entry[i] == '=');
}
int	match_key(const char *env_entry, const char *key)
{
	int	i;

	i = 0;
	while (env_entry[i] && key[i] && env_entry[i] == key[i])
		i++;
	return (key[i] == '\0');
}

void	remove_env_entry(char **env, int index)
{
	free(env[index]);
	while (env[index + 1])
	{
		env[index] = env[index + 1];
		index++;
	}
	env[index] = '\0';
}


void	remove_var_from_env(const char *var_name, t_envp *env)
{
	int j;

	if (!env || !env->environment)
		return ;
	j = 0;
	while (env->environment[j])
	{
		if (match_key_env(env->environment[j], var_name))
		{
			remove_env_entry(env->environment, j);
			break;
		}
		j++;
	}
	if (!env->export_only)
		return;
	j = 0;
	while (env->export_only[j])
	{
		if (match_key(env->export_only[j], var_name))
		{
			remove_env_entry(env->export_only, j);
			break;
		}
		j++;
	}
}


int	unset(t_tree_node *root, t_envp *env)
{
	t_tree_node	*arg;

	if (!root || !root->right || !env)
		return (0);
	arg = root->right;
	while (arg)
	{
		if (arg->data[0] == '-')
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(arg->data, 2);
			ft_putstr_fd("': invalid option\n", 2);
			return (2);
		}
		remove_var_from_env(arg->data, env);
		if (ft_strcmp(arg->data, "PATH") == 0)
			env->split_path = NULL;
		arg = arg->right;
	}
	return (0);
}
