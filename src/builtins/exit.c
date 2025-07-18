/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:51:39 by root              #+#    #+#             */
/*   Updated: 2025/07/17 19:08:43 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	safe_atoll(const char *str, long long *out)
{
	int			sign;
	int			i;
	long long	result;

	sign = 1;
	i = 0;
	result = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (ft_strcmp(str, "-9223372036854775808") == 0)
		*out = LLONG_MIN;
	else
	{
		while (str[i])
		{
			if (!ft_isdigit(str[i]))
				return (0);
			if (result > (9223372036854775807LL - (str[i] - '0')) / 10)
				return (0);
			result = result * 10 + (str[i] - '0');
			i++;
		}
		*out = result * sign;
	}
	return (1);
}

int	count_args(t_tree_node *node)
{
	int				count;
	t_tree_node		*arg;

	count = 0;
	arg = node->right;
	while (arg)
	{
		count++;
		arg = arg->right;
	}
	return (count);
}

static int	handle_exit_args(t_tree_node *arg, t_envp *env, t_gc_list *grbg_collector)
{
	long long	value;

	if (!is_numeric(arg->data) || !safe_atoll(arg->data, &value))
	{
		printf("exit: %s: numeric argument required\n", arg->data);
		env->exit_code = 2;
		ft_free_gc(grbg_collector);
		exit(2);
	}
	env->exit_code = (unsigned char)value;
	ft_free_gc(grbg_collector);
	exit((unsigned char)value);
}

int	exit_builtin(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	t_tree_node	*arg;
	long long	value;
	int			argc;
	long long			exit_code;

	printf("exit\n");
	arg = node->right;
	argc = count_args(node);
	if (argc >= 1 && (!is_numeric(arg->data) || !safe_atoll(arg->data, &value)))
	{
		printf("exit: %s: numeric argument required\n", arg->data);
		env->exit_code = 2;
		ft_free_gc(grbg_collector);
		exit(2);
	}
	if (argc > 1)
	{
		printf("exit: too many arguments\n");
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
