/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:06:11 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/19 16:01:05 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_var_name(const char *name)
{
	int	i;

	i = 0;
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	match_key(const char *env_entry, const char *key)
{
	int	i;

	i = 0;
	while (env_entry[i] && key[i] && env_entry[i] == key[i])
		i++;
	return (key[i] == '\0' && env_entry[i] == '=');
}

void	remove_env_entry(char **env, int index)
{
	free(env[index]);
	while (env[index])
	{
		env[index] = env[index + 1];
		index++;
	}
}

void	remove_var_from_env(const char *var_name, t_envp *env)
{
	int	j;

	j = 0;
	if (!env || !env->environment)
		return ;
	while (env->environment[j])
	{
		if (match_key(env->environment[j], var_name))
		{
			remove_env_entry(env->environment, j);
			break ;
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
		if (is_valid_key(arg->data))
			remove_var_from_env(arg->data, env);
		else
		{
			printf("minishell: unset:'");
			return (print_message_and_exit(arg->data, "': not a valid identifier", 1));
		}
		arg = arg->right;
	}
	return (0);
}
