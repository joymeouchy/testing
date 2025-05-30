/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:56:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/25 15:26:03 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char **dup_env(char **envp)
{
	int count = 0;
	char **copy;
	int i = 0;

	while (envp && envp[count])
		count++;

	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return NULL;

	while (i < count) {
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;

	return copy;
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    t_envp *env;

    signals();
    env = malloc(sizeof(t_envp));
    if (!env)
    {
        return (1);
    }
	env->split_path = get_split_path(envp);
    env->environment = dup_env(envp);
    env->is_malloced = 1;
	
    while (1)
	{
        parsing_main(env);
    }
    free(env);
    return (0);
}