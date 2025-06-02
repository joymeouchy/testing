/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:58:23 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/02 10:26:39 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void echo(t_tree_node *echo_node)
{
	t_tree_node *arg;
	int newline;

	arg = echo_node->right;
	newline = 1;
	if (arg && arg->data && ft_strcmp(arg->data, "-n") == 0)
	{
		newline = 0;
		arg = arg->right;
	}
	while (arg && arg->token == WORD)
	{
		// if (ft_strcmp(arg->data, "$?") == 0)
		// 	ft_putnbr_fd(env->exit_code, 1);
		
			ft_putstr_fd(arg->data, 1);

		if (arg->right && arg->right->token == WORD)
			ft_putstr_fd(" ", 1);
		arg = arg->right;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
}
