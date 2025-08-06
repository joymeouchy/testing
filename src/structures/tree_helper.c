/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:17:46 by root              #+#    #+#             */
/*   Updated: 2025/08/06 17:53:13 by jmeouchy         ###   ########.fr       */
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

bool	stack_is_valid(t_stack *stack)
{
	if (stack && stack->stack && stack->top > -1)
		return (true);
	return (false);
}

bool	not_cmd_pipe(t_stack *stack)
{
	if(stack->stack[stack->top].token != PIPE
	&& stack->stack[stack->top].token != BUILT_IN
	&& stack->stack[stack->top].token != COMMAND)
		return (true);
	return (false);
}

bool	is_redirection(t_tree_node *node)
{
	if (node->token >= LEFT_REDIRECTION
		&& node->token <= RIGHT_D_REDIRECTION)
		return (true);
	return (false);
}

bool	is_cmd(t_tree_node *node)
{
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (true);
	return (false);
}


t_tree_node	*build_tree(t_stack *stack, t_envp *environment,
		t_gc_list *grgb_collector)
{
	t_tree_node	*new_node;

	if (stack->top < 0)
		return (NULL);
	new_node = create_tree_node(stack, environment, grgb_collector);
	if (!new_node)
		return (NULL);
	if (stack_is_valid(stack) && new_node && is_cmd(new_node) && not_cmd_pipe(stack))
		new_node->right = build_tree(stack, environment, grgb_collector);
	if (new_node && new_node->token == PIPE)
	{
		new_node->right = build_tree(stack, environment, grgb_collector);
		new_node->left = build_tree(stack, environment, grgb_collector);
	}
	if (stack_is_valid(stack) && new_node && is_redirection(new_node) && not_cmd_pipe(stack))
		new_node->right = build_tree(stack, environment, grgb_collector);
	if (new_node && stack_is_valid(stack) && new_node->token == WORD && stack->stack[stack->top].token >= 3)
		new_node->right = build_tree(stack, environment, grgb_collector);
	return (new_node);
}
