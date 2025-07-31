/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:30:37 by root              #+#    #+#             */
/*   Updated: 2025/07/31 22:41:45 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_directory(const char *path)
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

static void	update_pwd_vars(t_envp *env, const char *oldpwd,
		t_gc_list *grbg_collector)
{
	char	*cwd;
	char	*old_entry;
	char	*pwd_entry;

	cwd = getcwd(NULL, 0);
	old_entry = NULL;
	pwd_entry = NULL;
	if (!cwd)
		return ;
	old_entry = ft_strjoin("OLDPWD=", oldpwd, grbg_collector);
	if (!old_entry)
		return ((void)0);
	pwd_entry = ft_strjoin("PWD=", cwd, grbg_collector);
	if (!pwd_entry)
		return ((void)0);
	update_env(old_entry, env, grbg_collector);
	update_env(pwd_entry, env, grbg_collector);
}

int	cd(t_tree_node *root, t_envp *env, t_gc_list *grbg_collector)
{
	char		*target;
	const char	*arg = NULL;
	char		*oldpwd;

	if (root && root->right)
		arg = root->right->data;
	if (root->right && root->right->right)
		return (env->exit_code = print_message_and_exit("cd:",
				"too many arguments", 1));
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	target = resolve_cd_target(arg, env, grbg_collector);
	if (!target)
		return (1);
	if (!is_valid_directory(target) || chdir(target) != 0)
	{
		env->exit_code = 1;
		return (1);
	}
	update_pwd_vars(env, oldpwd, grbg_collector);
	return (env->exit_code);
}
