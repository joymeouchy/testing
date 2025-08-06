/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_swap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:05:32 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 21:08:44 by lkhoury          ###   ########.fr       */
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
		t_gc_list *grbg_collector, int *heredoc_counter)
{
	if (node == NULL)
		return ;
	if (node->token == LEFT_D_REDIRECTION)
	{
		heredoc(node, env, grbg_collector, *heredoc_counter);
		(*heredoc_counter)++;
	}
	swap_redir_in_tree(node->left, env, grbg_collector, heredoc_counter);
	swap_redir_in_tree(node->right, env, grbg_collector, heredoc_counter);
	swap_redir_to_bottom(&node);
}
