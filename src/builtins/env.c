/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:32:14 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/02 10:14:25 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void  env_getter(t_envp *env)
{
    int i;

    i = 0;
    while(env->environment[i])
    {
        printf("%s\n", env->environment[i]);
        i++;
    }
}