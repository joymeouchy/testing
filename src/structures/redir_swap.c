/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_swap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:05:32 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/15 13:54:42 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	should_swap(t_tree_node *node)
{
	if ((node->token == COMMAND || node->token == BUILT_IN
			|| node->token == WORD) && (node->right->token >= LEFT_REDIRECTION
			&& node->right->token <= RIGHT_D_REDIRECTION))
		return (true);
	return (false);
}

void	swap_redir_to_bottom(t_tree_node **node_ptr)
{
	t_tree_node	*node;

	node = *node_ptr;
	while (node)
	{
		if (node->left && should_swap(node))
		{
			swap_red(node, node->left);
			node = node->left;
		}
		else if (node->right && should_swap(node))
		{
			swap_red(node, node->right);
			node = node->right;
		}
		else
			break ;
	}
}

void	swap_redir_in_tree(t_tree_node *node, t_envp *env,
		t_gc_list *grbg_collector)
{
	if (node == NULL)
		return ;
	swap_redir_in_tree(node->left, env, grbg_collector);
	swap_redir_in_tree(node->right, env, grbg_collector);
	swap_redir_to_bottom(&node);
}
