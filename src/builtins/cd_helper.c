/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:12:30 by root              #+#    #+#             */
/*   Updated: 2025/08/16 12:00:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(const char *name, char **env, t_gc_list *grbg_collector)
{
	int		i;
	size_t	len;
	char	*env_value;
	char	*value_start;

	i = 0;
	len = ft_strlen(name);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			value_start = env[i] + len + 1;
			env_value = ft_strdup(value_start, grbg_collector);
			return (env_value);
		}
		i++;
	}
	return (NULL);
}

char	*handle_cd_home(t_envp *env, t_gc_list *grbg_collector)
{
	char	*home;

	home = get_env_value("HOME", env->environment, grbg_collector);
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (NULL);
	}
	return (home);
}

char	*handle_cd_dash(t_envp *env, t_gc_list *grbg_collector)
{
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", env->environment, grbg_collector);
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

char	*handle_cd_tilde(const char *arg, t_envp *env
	, t_gc_list *grbg_collector)
{
	char	*home;
	char	*path;

	home = get_env_value("HOME", env->environment, grbg_collector);
	if (!home)
		home = env->home;
	if (!home)
	{
		ft_putendl_fd("minishell: cd: cannot determine home directory", 2);
		return (NULL);
	}
	path = ft_strjoin(home, arg + 1, grbg_collector);
	if (!path)
		return (NULL);
	return (path);
}

char	*resolve_cd_target(const char *arg, t_envp *env
	, t_gc_list *grbg_collector)
{
	if (!arg || arg == NULL)
		return (handle_cd_home(env, grbg_collector));
	if (ft_strcmp(arg, "-") == 0)
		return (handle_cd_dash(env, grbg_collector));
	if (arg[0] == '~')
		return (handle_cd_tilde(arg, env, grbg_collector));
	return (ft_strdup(arg, grbg_collector));
}
