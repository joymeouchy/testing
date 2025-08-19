/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:33:17 by root              #+#    #+#             */
/*   Updated: 2025/08/19 15:51:14 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int pwd(t_envp *env, t_gc_list *grbg_collector)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        cwd = get_env_value("PWD", env->environment, grbg_collector);
        if (!cwd)
        {
            perror("pwd");
            return 1;
        }
        printf("%s\n", cwd);
        return 0;
    }
    printf("%s\n", cwd);
    free(cwd);
    return 0;
}
