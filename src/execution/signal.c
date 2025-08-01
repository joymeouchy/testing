/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:30:41 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/01 13:17:09 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sig_handler(int sigtype)
{
	if (sigtype == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signals(void)
{
	if (signal(SIGINT, sig_handler) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR)
	{
		write(1, "error\n", 1);
		exit(1);
	}
}

void	signal_in_child(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR || signal(SIGQUIT,
			SIG_IGN) == SIG_ERR)
	{
		write(1, "error\n", 1);
		exit(1);
	}
}
