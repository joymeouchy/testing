/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:59 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/02 10:16:02 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execution(t_tree_node *node, t_envp *env)
{
	int	pipe_count;

	if (!node)
		return ;
	pipe_count = count_pipes(node);
	if (node->token == PIPE)
		return (pipe_exec(node, pipe_count, env));
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (exec_commands(node, env));
	if (node->token >= 3 && node->token <= 6)
		return (handle_recirections(node, env));
}
