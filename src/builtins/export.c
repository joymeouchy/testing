/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:50:44 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/11 22:03:46 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_print_mode(t_envp *env)
{
	char	**merged;
	int		i;

	merged = merge_env_vars(env);
	if (!merged)
		return (1);
	sort_env(merged);
	i = 0;
	while (merged[i])
	{
		print_key_value(merged[i]);
		i++;
	}
	free_env(merged);
	return (0);
}

int	export(t_tree_node *root, t_envp *env)
{
	t_tree_node	*arg;
	int			error;

	error = 0;
	if (!root || !env)
		return (1);
	arg = root->right;
	if (!arg)
		return (export_print_mode(env));
	while (arg)
	{
		if (is_valid_key(arg->data))
			update_env(arg->data, env);
		else
		{
			printf("minishell: export: `%s': not a valid identifier\n", arg->data);
			error = 1;
			printf("error = '%d'\n",error);
		}
		arg = arg->right;
	}
	env->exit_code = error;
	return (error);
}
