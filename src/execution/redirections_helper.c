/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/06 22:42:09 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_fd_error(char *prefix, char *name, t_envp *env,
		t_gc_list *grbg_collector)
{
	env->exit_code = print_message_and_exit(prefix, name, 1);
	ft_free_gc(grbg_collector);
	exit(1);
}

int	open_file_for_redirect(char *filename, int flags, t_envp *env,
		t_gc_list *grbg_collector)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd == -1)
		handle_fd_error("minishell: ", strerror(errno), env, grbg_collector);
	return (fd);
}

void	dup_and_close(int fd, int std_fd, t_envp *env,
		t_gc_list *grbg_collector)
{
	if (dup2(fd, std_fd) == -1)
	{
		close(fd);
		handle_fd_error("minishell: dup2 failed: ", strerror(errno), env,
			grbg_collector);
	}
	close(fd);
}
