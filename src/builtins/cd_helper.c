/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:12:30 by root              #+#    #+#             */
/*   Updated: 2025/06/27 20:09:15 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(const char *name, char **env)
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
			env_value = ft_strdup(value_start);
			return (env_value);
		}
		i++;
	}
	return (NULL);
}


char	*handle_cd_home(t_envp *env)
{
	char	*home;

	home = get_env_value("HOME", env->environment);
	if (!home)
	{
		printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (ft_strdup(home));
}

char	*handle_cd_dash(t_envp *env)
{
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", env->environment);
	if (!oldpwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (ft_strdup(oldpwd));
}

char	*handle_cd_tilde(const char *arg, t_envp *env)
{
	char	*home;
	char	*path;

	home = get_env_value("HOME", env->environment);
	if (!home)
		home = env->home;
	if (!home)
	{
		printf("minishell: cd: cannot determine home directory\n");
		return (NULL);
	}
	printf("home%s\n", home);
	path = ft_strjoin(home, arg + 1);
	printf("path:%s\n", path);
	if (!path)
		return (NULL);
	// if (home)  //TODO HUUH?????
		// free(home);
	return (path);
}


char	*resolve_cd_target(const char *arg, t_envp *env)
{
	if (arg == NULL)
		return (handle_cd_home(env));
	if (ft_strcmp(arg, "-") == 0)
		return (handle_cd_dash(env));
	if (arg[0] == '~')
		return (handle_cd_tilde(arg, env));
	return (ft_strdup(arg));
}
