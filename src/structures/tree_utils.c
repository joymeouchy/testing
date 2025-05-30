/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:05:32 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/22 11:25:57 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	print_inorder(t_tree_node *node)
{
	if (node == NULL)
		return ;
	printf("Data: %s, Token: %d, redurection%s\n", node->data, node->token, node->redir_arg);
	if (node->right )
		printf("right:%s\n", node->right->data);
	if(node->left )
		printf("left:%s\n", node->left->data);
	print_inorder(node->left);
	print_inorder(node->right);
}

t_tree	*init_tree(void)
{
	t_tree	*tree;

	tree = malloc(sizeof(t_tree));
	if (!tree)
		return (NULL) ;
	tree->root = NULL;
	return (tree);
}

t_tree_node	*create_tree_node(t_stack *stack, t_envp *environment)
{
	t_tree_node	*new_node;

	new_node = (t_tree_node *)malloc(sizeof(t_tree_node));
	if (!new_node)
		return(NULL);
	new_node->data = stack->stack[stack->top].data;
	new_node->token = stack->stack[stack->top].token;
	new_node->redir_arg = stack->stack[stack->top].redir_arg;
	stack->top--;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->path = environment;
	return (new_node);
}

t_tree_node *build_tree(t_stack *stack, t_envp *environment)
{
	t_tree_node *new_node;

	if (stack->top < 0)
		return (NULL);
	new_node = create_tree_node(stack, environment);
	if (!new_node)
		return (NULL);
	if ((new_node->token == COMMAND || new_node->token == BUILT_IN) && stack->stack[stack->top].token != PIPE
		&& stack->stack[stack->top].token != BUILT_IN && stack->stack[stack->top].token != COMMAND)
	{
		new_node->right = build_tree(stack, environment);
	}
	if ((new_node->token >= LEFT_REDIRECTION && new_node->token <= RIGHT_D_REDIRECTION) && stack->stack[stack->top].token != PIPE)
	{
		new_node->right = build_tree(stack, environment);
	}
	if (new_node->token == WORD && stack->stack[stack->top].token > 3)
		new_node->right = build_tree(stack, environment);
	if (new_node->token == PIPE)
	{
		new_node->right = build_tree(stack, environment);
		new_node->left = build_tree(stack, environment);
	}
	return (new_node);
}

bool should_swap(t_tree_node *node)
{
	if((node->token == COMMAND || node->token == BUILT_IN || node->token == WORD)
	&& (node->right->token >= LEFT_REDIRECTION && node->right->token <= RIGHT_D_REDIRECTION))
		return (true);
	return (false);
}

void	swap_redir_to_bottom(t_tree_node **node_ptr)
{
	t_tree_node *node = *node_ptr;

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
			break;
	}
}
void	swap_redir_in_tree(t_tree_node *node)
{
	if (node == NULL)
		return ;

	swap_redir_in_tree(node->left);
	swap_redir_in_tree(node->right);

	swap_redir_to_bottom(&node); // push this node as far down as it can go
}


t_tree	*stack_to_tree(t_stack *stack, t_envp *environment)
{
	t_tree	*tree;

	tree = init_tree();
	tree->root = build_tree(stack, environment);
	swap_redir_in_tree(tree->root);
	return (tree);
}
