/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:05:32 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 17:56:12 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*stack_to_tree(t_stack *stack, t_envp *environment,
		t_gc_list *grbg_collector)
{
	t_tree	*tree;
	int		heredoc_counter;

	heredoc_counter = 0;
	if (!stack || stack->top < 0)
		return (NULL);
	tree = init_tree(grbg_collector);
	tree->root = build_tree(stack, environment, grbg_collector);
	swap_redir_in_tree(tree->root, environment, grbg_collector,
		&heredoc_counter);
	return (tree);
}
