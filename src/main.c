/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:56:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/15 15:09:08 by root             ###   ########.fr       */
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

void	update_shlvl(t_envp *env)
{
	char	*value;
	int		shlvl;
	char	*new_value;
	char	*new_entry;

	value = get_env_value("SHLVL", env->environment);
	shlvl = ft_atoi(value) + 1;
	new_value = ft_itoa(shlvl);
	new_entry = ft_strjoin("SHLVL=", new_value);
	free(new_value);
	remove_var_by_key("SHLVL", &env->environment);
	add_new_var(new_entry, &env->environment);
	free(new_entry);
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
	update_shlvl(env);
	while (1)
	{
		parsing_main(env, command_line_input());
	}
	// free_2darray(env->split_path);
	// free_2darray(env->environment);
	free(env); //TODO Free stuff inside env
	return (0);
}