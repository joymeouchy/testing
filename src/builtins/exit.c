/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:51:39 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/20 20:19:08 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_exit_args(t_tree_node *arg, t_envp *env,
		t_gc_list *grbg_collector)
{
	long long	value;

	if (!is_numeric(arg->data) || !safe_atoll(arg->data, &value))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(arg->data, 2);
		ft_putendl_fd(": numeric argument required", 2);
		env->exit_code = 2;
		ft_free_gc(grbg_collector);
		exit(2);
	}
	env->exit_code = (unsigned char)value;
	ft_free_gc(grbg_collector);
	exit((unsigned char)value);
}

static void	handle_non_numeric_exit(t_tree_node *arg, t_envp *env,
		t_gc_list *grbg_collector)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg->data, 2);
	ft_putendl_fd(": numeric argument required", 2);
	env->exit_code = 2;
	ft_free_gc(grbg_collector);
	exit(2);
}

int	exit_builtin(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	t_tree_node	*arg;
	long long	value;
	int			argc;
	long long	exit_code;

	printf("exit\n");
	arg = node->right;
	argc = count_args(node);
	if (argc >= 1 && (!is_numeric(arg->data) || !safe_atoll(arg->data, &value)))
		handle_non_numeric_exit(arg, env, grbg_collector);
	if (argc > 1)
	{
		ft_putendl_fd("exit: too many arguments", 2);
		env->exit_code = 1;
		return (1);
	}
	if (argc == 0)
	{
		exit_code = env->exit_code;
		ft_free_gc(grbg_collector);
		exit(exit_code);
	}
	return (handle_exit_args(arg, env, grbg_collector));
}
