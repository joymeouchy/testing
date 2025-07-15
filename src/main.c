/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:56:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/15 20:30:14 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	**dup_env(char **envp, 	t_gc_list *grgb_collector)
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
	copy = ft_malloc(sizeof(char *) * (count + 1), grgb_collector);
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	update_shlvl(t_envp *env, t_gc_list *grgb_collector)
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
	remove_var_by_key("SHLVL", &env->environment, grgb_collector);
	add_new_var(new_entry, &env->environment, grgb_collector);
	free(new_entry);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_envp	*env;
	t_gc_list *grgb_collector;

	signals();
    grgb_collector = init_grbg_collector();
	env = ft_malloc(sizeof(t_envp), grgb_collector);
	if (!env)
		return (1);
	env->split_path = get_split_path(envp);
	env->environment = dup_env(envp, grgb_collector);
	env->home = get_env_value("HOME", env->environment);
	// printf("env->home %s\n", env->home);
	update_shlvl(env, grgb_collector);
	// printf("env->home hii%s\n", env->home);
	while (1)
	{
		if (g_sigint)
		{
			g_sigint = 0;
			continue;
		}
		parsing_main(env, command_line_input(), grgb_collector);
	}
	ft_free_gc(grgb_collector);
	return (0);
}
