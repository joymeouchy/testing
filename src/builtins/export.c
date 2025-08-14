/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:50:44 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/14 17:34:38 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	export_print_mode(t_envp *env, t_gc_list *grbg_collector)
{
	char	**merged;
	int		i;

	merged = merge_env_vars(env, grbg_collector);
	if (!merged)
		return (1);
	sort_env(merged);
	i = 0;
	while (merged[i])
	{
		print_key_value(merged[i], grbg_collector);
		i++;
	}
	return (0);
}

void	handle_invalid_key(char *key)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(key, 2);
	ft_putendl_fd(": not a valid identifier", 2);
}

int	export(t_tree_node *root, t_envp *env, t_gc_list *grbg_collector)
{
	t_tree_node	*arg;
	int			ret;

	if (!root || !env)
		return (1);
	arg = root->right;
	if (!arg)
		return (export_print_mode(env, grbg_collector));
	ret = 0;
	while (arg)
	{
		if (is_valid_key(arg->data))
		{
			update_env(arg->data, env, grbg_collector);
			env->exit_code = 0;
		}
		else
		{
			handle_invalid_key(arg->data);
			ret = 1;
		}
		arg = arg->right;
	}
	env->exit_code = ret;
	return (ret);
}
