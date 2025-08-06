/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 06:42:48 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 17:16:26 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	print_list(t_list *list)
// {
// 	t_list_node	*temp;

// 	if (!list || !list->head)
// 		return ;
// 	list->head->index = 0;
// 	temp = list->head;
// 	printf("list:\n");
// 	while (temp)
// 	{
// 		if (temp->data)
// 			printf("index %d node:'%s' token:%d redirection: %s\n", temp->index,
// 				temp->data, temp->token, temp->redir_arg);
// 		else if (temp->data == NULL)
// 			printf("index %d node:NULL\n", temp->index);
// 		temp = temp->next;
// 	}
// }

void	update_list_index(t_list_node *temp)
{
	while (temp != NULL)
	{
		temp->index++;
		temp = temp->next;
	}
}

void	delete_node(t_list *list, t_list_node *node)
{
	t_list_node	*current;
	t_list_node	*temp;

	if (list->head == NULL || node == NULL)
		return ;
	if (list->head == node)
	{
		temp = list->head;
		list->head = (list->head)->next;
		free(temp);
		return ;
	}
	current = list->head;
	while (current->next && current->next != node)
		current = current->next;
	if (current->next == NULL)
		return ;
	current->next = node->next;
}

void	check_and_remove_empty(t_list *list)
{
	t_list_node	*temp;
	t_list_node	*next;

	temp = list->head;
	while (temp)
	{
		next = temp->next;
		if (ft_strcmp(temp->data, "") == 0)
			delete_node(list, temp);
		temp = next;
	}
}
