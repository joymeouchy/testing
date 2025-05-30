/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:30:37 by root              #+#    #+#             */
/*   Updated: 2025/05/27 15:23:47 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *get_env_value(const char *name, char **env)
{
	int		i = 0;
	size_t	len = ft_strlen(name);

	while (env && env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char *handle_cd_home(t_envp *env)
{
	char *home = get_env_value("HOME", env->environment);
	if (!home)
	{
		printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	return ft_strdup(home);
}

static char *handle_cd_dash(t_envp *env)
{
	char *oldpwd = get_env_value("OLDPWD", env->environment);
	if (!oldpwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return ft_strdup(oldpwd);
}

static char *handle_cd_tilde(const char *arg, t_envp *env)
{
	char *home = get_env_value("HOME", env->environment);
	if (!home)
	{
		printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	return ft_strjoin(home, arg + 1);
}

static char *resolve_cd_target(const char *arg, t_envp *env)
{
	if (arg == NULL)
		return handle_cd_home(env);
	if (ft_strcmp(arg, "-") == 0)
		return handle_cd_dash(env);
	if (arg[0] == '~')
		return handle_cd_tilde(arg, env);
	return ft_strdup(arg);
}

static int is_valid_directory(const char *path)
{
	DIR *dir;

	dir = opendir(path);
	if (!dir)
	{
		perror("minishell: cd");
		return (0);
	}
	closedir(dir);
	if (access(path, X_OK) != 0)
	{
		perror("minishell: cd");
		return (0);
	}
	return (1);
}

static void update_pwd_vars(t_envp *env, const char *oldpwd)
{
	char *cwd = getcwd(NULL, 0);
	char *old_entry = NULL;
	char *pwd_entry = NULL;

	if (!cwd)
		return;
	old_entry = ft_strjoin("OLDPWD=", oldpwd);
	if (!old_entry)
		return (free(cwd), (void)0);
	pwd_entry = ft_strjoin("PWD=", cwd);
	if (!pwd_entry)
		return (free(cwd), free(old_entry), (void)0);
	update_env(old_entry, env);
	update_env(pwd_entry, env);
	free(cwd);
	free(old_entry);
	free(pwd_entry);
}

int	cd(t_tree_node *root, t_envp *env)
{
	char		*target;
	const char	*arg = NULL;
	char		*oldpwd;

	if (root && root->right)
		arg = root->right->data;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	target = resolve_cd_target(arg, env);
	if (!target)
		return (free(oldpwd), 1);
	if (!is_valid_directory(target) || chdir(target) != 0)
	{
		perror("minishell: cd");
		free(target);
		free(oldpwd);
		return (1);
	}
	update_pwd_vars(env, oldpwd);
	free(target);
	free(oldpwd);
	return (0);
}
