/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:32:14 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/02 08:46:19 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_getter(t_envp *env)
{
	int	i;

	i = 0;
	while (env->environment[i])
	{
		printf("%s\n", env->environment[i]);
		i++;
	}
	return (0);
}
