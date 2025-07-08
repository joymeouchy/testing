/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:38:44 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/05 22:33:50 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/structures.h"

void free_list_node(t_list_node *node)
{
	if (!node)
		return ;
	// if (node->data)
	// 	free(node->data);
	if (node->redir_arg)
		free(node->redir_arg);
	free(node);
}

void	free_list(t_list *list)
{
	t_list_node	*current;
	t_list_node	*next;

	if (!list || !list->head)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free_list_node(current);
		current = next;
	}
	list->head = NULL;
	free(list);
}

// void	free_stack(t_stack *stack, int stack_top)
// {
// 	if (!stack)
// 		return ;
// 	stack->top = stack_top;
// 	while (stack->top > 0)
// 		pop(stack);
// 	free(stack->stack);
// 	// free(stack);
// }

void	free_stack(t_stack *stack)
{
	if (!stack)
		return ;
	while (stack->top > 0)
		pop(stack);
	free(stack->stack);
	// free(stack);
}

void	free_2darray(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_tree_node(t_tree_node *node)
{
	if (!node)
		return;
	free_tree_node(node->left);
	free_tree_node(node->right);
	if (node->redir_arg)
		free(node->redir_arg);
	if (node->data)
		free(node->data);
	if (node->path)
	{
		free_2darray(node->path->split_path);
		free_2darray(node->path->environment);
		free_2darray(node->path->export_only);
		free(node->path);
	}
	if (node->root)
		node->root = NULL;
	free(node);
}

void	free_tree(t_tree_node *root)
{
	if (!root)
		return;
	free_tree_node(root);
}

void	free_env_struct(t_envp *env)
{
	free_2darray(env->split_path);
	free_2darray(env->split_path);
	free_2darray(env->export_only);
	free(env->home);
}
