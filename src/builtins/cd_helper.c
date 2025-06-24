/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:12:30 by root              #+#    #+#             */
/*   Updated: 2025/06/24 09:45:42 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(const char *name, char **env)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
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

static char	*extract_home(char *line)
{
	int		i;
	int		colons;
	char	*start;
	char	*end;

	i = 0;
	colons = 0;
	while (line[i])
	{
		if (line[i] == ':')
			colons++;
		if (colons == 5)
		{
			start = line + i + 1;
			end = ft_strchr(start, ':');
			if (!end)
				return (NULL);
			return (ft_substr(start, 0, end - start));
		}
		i++;
	}
	return (NULL);
}

char	*search_user_home(char *buffer)
{
	char	*user;
	char	*line;
	size_t	user_len;

	user = getenv("USER");
	if (!user)
		return (NULL);
	user_len = ft_strlen(user);
	line = buffer;
	while (line && *line)
	{
		if (ft_strncmp(line, user, user_len) == 0
			&& line[user_len] == ':')
			return (extract_home(line));
		line = ft_strchr(line, '\n');
		if (line)
			line++;
	}
	return (NULL);
}

char	*get_home_from_passwd(void)
{
	int		fd;
	int		len;
	char	buffer[4096];

	fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0)
		return (NULL);
	len = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (len <= 0)
		return (NULL);
	buffer[len] = '\0';
	return (search_user_home(buffer));
}

char	*handle_cd_tilde(const char *arg, t_envp *env)
{
	char	*home;
	char	*path;

	home = get_env_value("HOME", env->environment);
	if (!home)
		home = get_home_from_passwd();
	if (!home)
	{
		printf("minishell: cd: cannot determine home directory\n");
		return (NULL);
	}
	path = ft_strjoin(home, arg + 1);
	if (!path)
		return (NULL);
	if (!get_env_value("HOME", env->environment))
		free(home);
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
