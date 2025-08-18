/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:26:35 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/18 21:42:01 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	stack_is_valid(t_stack *stack)
{
	if (stack && stack->stack && stack->top > -1)
		return (true);
	return (false);
}

bool	not_cmd_pipe(t_stack *stack)
{
	if (stack->stack[stack->top].token != PIPE
		&& stack->stack[stack->top].token != BUILT_IN
		&& stack->stack[stack->top].token != COMMAND)
		return (true);
	return (false);
}

bool	is_redirection(t_tokens token)
{
	if (token >= LEFT_REDIRECTION && token <= RIGHT_D_REDIRECTION)
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
	if (stack_is_valid(stack) && new_node && is_redirection(new_node->token)
	&& not_cmd_pipe(stack))
	{
		// fprintf(stdout, "node->data is:%s\n", new_node->data);
	new_node->right = build_tree(stack, environment, grgb_collector);}
	if (stack_is_valid(stack) && new_node && is_cmd(new_node)
	&& not_cmd_pipe(stack))
	{
	new_node->right = build_tree(stack, environment, grgb_collector);
	}
	if (new_node && new_node->token == PIPE)
	{
		new_node->right = build_tree(stack, environment, grgb_collector);
		new_node->left = build_tree(stack, environment, grgb_collector);
	}
	if (new_node && stack_is_valid(stack) && new_node->token == WORD
		&& not_cmd_pipe(stack))
		new_node->right = build_tree(stack, environment, grgb_collector);
	return (new_node);
}
