/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samira <samira@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:59 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/10 21:00:00 by samira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void precreate_output_redirections(t_tree_node *node)
{
	int fd;
	int flags;

	if (!node)
		return;
	precreate_output_redirections(node->left);
	precreate_output_redirections(node->right);
	if ((node->token == RIGHT_REDIRECTION || node->token == RIGHT_D_REDIRECTION)
		&& node->redir_arg)
	{
		if (node->token == RIGHT_REDIRECTION)
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		else
			flags = O_CREAT | O_WRONLY | O_APPEND;
		fd = open(node->redir_arg, flags, 0644);
		if (fd >= 0)
			close(fd);
		else
			perror("minishell: cannot create output file");
	}
}

int	execution(t_tree_node *node, t_envp *env)
{
	int	pipe_count;

	if (!node)
		return (-1);
	precreate_output_redirections(node);
	pipe_count = count_pipes(node);
	if (node->token == PIPE)
		return (pipe_exec(node, pipe_count, env));
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (exec_commands(node, env));
	if (node->token >= 3 && node->token <= 6)
		return (handle_recirections(node, env));
	return (env->exit_code);
}
