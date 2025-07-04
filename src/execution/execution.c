/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:59 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/02 20:28:14 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execution(t_tree_node *node, t_envp *env)
{
	int	pipe_count;

	if (!node)
		return (-1);
	pipe_count = count_pipes(node);
	if (node->token == PIPE)
		return (pipe_exec(node, pipe_count, env));
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (exec_commands(node, env));
	if (node->token >= 3 && node->token <= 6)
		return (handle_recirections(node, env));
	return (env->exit_code);
}
