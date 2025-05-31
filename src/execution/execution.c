/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:59 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/31 13:25:27 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execution(t_tree_node *node)
{
	int	pipe_count;

	if (!node)
		return ;
	pipe_count = count_pipes(node);
	// printf("current node is: %s, redir arg:%s \n", node->data, node->redir_arg);
	if (node->token == PIPE)
		return (pipe_exec(node, pipe_count));
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (exec_commands(node));
	if (node->token >= 3 && node->token <= 6)
		return (handle_recirections(node));
}
