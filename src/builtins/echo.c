/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:58:23 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/31 22:49:37 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(t_tree_node *echo_node)
{
	t_tree_node	*arg;
	int			newline;

	arg = echo_node->right;
	newline = 1;
	while (arg && arg->data && is_valid_n_flag(arg->data))
	{
		newline = 0;
		arg = arg->right;
	}
	while (arg && arg->token == WORD)
	{
		ft_putstr_fd(arg->data, 1);
		if (arg->right && arg->right->token == WORD)
			ft_putstr_fd(" ", 1);
		arg = arg->right;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
