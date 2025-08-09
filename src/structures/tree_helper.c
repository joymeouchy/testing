/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:17:46 by root              #+#    #+#             */
/*   Updated: 2025/08/09 16:33:17 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_inorder(t_tree_node *node)
{
	if (node == NULL)
		return ;
	printf("Data: %s, Token: %d, redurection%s\n", node->data, node->token,
		node->redir_arg);
	if (node->right)
		printf("right:%s\n", node->right->data);
	if (node->left)
		printf("left:%s\n", node->left->data);
	print_inorder(node->left);
	print_inorder(node->right);
}

t_tree	*init_tree(t_gc_list *grgb_collector)
{
	t_tree	*tree;

	tree = ft_malloc(sizeof(t_tree), grgb_collector);
	if (!tree)
		return (NULL);
	tree->root = NULL;
	return (tree);
}

t_tree_node	*create_tree_node(t_stack *stack, t_envp *environment,
		t_gc_list *grgb_collector)
{
	t_tree_node	*new_node;

	new_node = (t_tree_node *)ft_malloc(sizeof(t_tree_node), grgb_collector);
	if (!new_node)
		return (NULL);
	new_node->data = stack->stack[stack->top].data;
	new_node->token = stack->stack[stack->top].token;
	new_node->redir_arg = stack->stack[stack->top].redir_arg;
	stack->top--;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->path = environment;
	return (new_node);
}
