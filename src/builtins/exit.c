/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:51:39 by root              #+#    #+#             */
/*   Updated: 2025/06/14 15:23:00 by jmeouchy         ###   ########.fr       */
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

static int	handle_exit_args(t_tree_node *arg, t_envp *env)
{
	long long	value;
	int			valid;

	valid = is_numeric(arg->data);
	if (!valid || !safe_atoll(arg->data, &value))
	{
		printf("exit: %s: numeric argument required\n", arg->data);
		env->exit_code = 2;
		exit(2);
	}
	if (value < 0)
		value = -value;
	env->exit_code = value % 256;
	exit((unsigned char)(env->exit_code));
	return (0);
}

int	exit_builtin(t_tree_node *node, t_envp *env)
{
	int			argc;
	t_tree_node	*arg;

	printf("exit\n");
	argc = count_args(node);
	if (argc > 1)
	{
		printf("exit: too many arguments\n");
		env->exit_code = 1;
		return (1);
	}
	if (argc == 0)
		exit((unsigned char)env->exit_code);
	arg = node->right;
	return (handle_exit_args(arg, env));
}
