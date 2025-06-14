/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:56:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/14 12:43:01 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char **dup_env(char **envp)
{
	int		count;
	char	**copy;
	int		i;

	if (!envp)
		return (NULL);
	count = 0;
	i = 0;
	while (envp && envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
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
		return (1);

	env->split_path = get_split_path(envp);
	env->environment = dup_env(envp);
	while (1)
	{
		parsing_main(env, command_line_input());
	}
	// free_2darray(env->split_path);
	// free_2darray(env->environment);
	free(env); //TODO Free stuff inside env
	return (0);
}