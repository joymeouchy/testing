/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:56:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/01 13:33:00 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	**dup_env(char **envp, t_gc_list *grgb_collector)
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
		copy[i] = ft_strdup(envp[i], grgb_collector);
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

	value = get_env_value("SHLVL", env->environment, grgb_collector);
	shlvl = ft_atoi(value) + 1;
	new_value = ft_itoa(shlvl, grgb_collector);
	new_entry = ft_strjoin("SHLVL=", new_value, grgb_collector);
	remove_var_by_key("SHLVL", &env->environment, grgb_collector);
	add_new_var(new_entry, &env->environment, grgb_collector);
}

int	main(int argc, char **argv, char **envp)
{
	t_envp		*env;
	t_gc_list	*grgb_collector;

	(void)argc;
	(void)argv;
	(void)envp;
	main_signal();
	grgb_collector = init_grbg_collector();
	env = ft_malloc(sizeof(t_envp), grgb_collector);
	if (!env)
		return (1);
	env->split_path = get_split_path(envp, grgb_collector);
	env->environment = dup_env(envp, grgb_collector);
	env->home = get_env_value("HOME", env->environment, grgb_collector);
	env->export_only = NULL;
	env->exit_code = 0;
	update_shlvl(env, grgb_collector);
	while (1)
	{
		parsing_main(env, command_line_input(), grgb_collector);
	}
	ft_free_gc(grgb_collector);
	return (0);
}
