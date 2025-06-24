/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:50:44 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/24 10:23:15 by root             ###   ########.fr       */
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
	int			ret;

	if (!root || !env)
		return (1);
	arg = root->right;
	if (!arg)
		return (export_print_mode(env));
	ret = 0;
	while (arg)
	{
		if (is_valid_key(arg->data))
		{
			update_env(arg->data, env);
			env->exit_code = 0;
		}
		else
		{
			printf("minishell: export: `%s': not a valid identifier\n", arg->data);
			ret = 1;
		}
		arg = arg->right;
	}
	env->exit_code = ret;
	return (ret);
}

