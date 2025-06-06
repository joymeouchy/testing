/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:58:23 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/06 14:22:34 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void echo(t_tree_node *echo_node)
{
	t_tree_node *arg;
	int newline;

	arg = echo_node->right;
	newline = 1;
	while (arg && arg->data && ft_strcmp(arg->data, "-n") == 0)
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
}
