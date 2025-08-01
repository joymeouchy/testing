/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:42:18 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/01 12:44:51 by lkhoury          ###   ########.fr       */
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

static int	parse_number(const char *str, long long *out, int i, int sign)
{
	long long	result;
	int			digit;

	result = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		digit = str[i] - '0';
		if (result > (9223372036854775807LL - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*out = result * sign;
	return (1);
}

int	safe_atoll(const char *str, long long *out)
{
	int	sign;
	int	i;

	sign = 1;
	i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	if (ft_strcmp(str, "-9223372036854775808") == 0)
	{
		*out = LLONG_MIN;
		return (1);
	}
	return (parse_number(str, out, i, sign));
}

int	count_args(t_tree_node *node)
{
	int			count;
	t_tree_node	*arg;

	count = 0;
	arg = node->right;
	while (arg)
	{
		count++;
		arg = arg->right;
	}
	return (count);
}
