/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:30:37 by root              #+#    #+#             */
/*   Updated: 2025/06/19 09:49:57 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_directory(const char *path)
{
	DIR	*dir;

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

static void	update_pwd_vars(t_envp *env, const char *oldpwd)
{
	char	*cwd;
	char	*old_entry;
	char	*pwd_entry;

	cwd = getcwd(NULL, 0);
	old_entry = NULL;
	pwd_entry = NULL;
	if (!cwd)
		return ;
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
		env->exit_code = 1;
		free(target);
		free(oldpwd);
		return (1);
	}
	update_pwd_vars(env, oldpwd);
	free(target);
	free(oldpwd);
	return (0);
}
