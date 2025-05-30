/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 06:42:48 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/22 08:39:46 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/structures.h"

t_list	*init_list(void)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	list->head = NULL;
	list->list_size = 0;
	return (list);
}

t_list_node	*create_list_node(char *data)
{
	t_list_node	*new_node;

	new_node = malloc(sizeof(t_list_node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->index = 0;
	new_node->redir_arg = NULL;
	return (new_node);
}

void	insert_at_end_list(t_list *list, char *new_node_data)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	if (*new_node_data == '\0')
	{
		free(new_node_data);
		return ;
	}
	new_node = create_list_node(new_node_data);
	if (list->head == NULL)
		list->head = new_node;
	else
	{
		temp = list->head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
		new_node->prev = temp;
		new_node->index = temp->index + 1;
		// free(temp);
	}
	list->list_size++;
}
// jhfgdjhdafg|\>>>><<<A>G<>ADS>G<DF>ag|f|AGasgd dsbasdf>,aaf" kgkndfgl adSG" g,n

void	insert_at_beginning_list(t_list *list, char *new_node_data)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	new_node = create_list_node(new_node_data);
	if (list->head == NULL)
		list->head = new_node;
	else
	{
		temp = list->head;
		list->head = new_node;
		new_node->next = temp;
		temp->prev = new_node;
		update_list_index(temp);
	}
	list->list_size++;
}

void	insert_at_middle_list(t_list *list, char *new_node_data, int index)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	if (index == 0)
		return (insert_at_beginning_list(list, new_node_data));
	new_node = create_list_node(new_node_data);
	if (list->head == NULL)
		list->head = new_node;
	else
	{
		temp = list->head;
		while (temp->index < index && temp->next)
			temp = temp->next;
		if (!temp)
			return (insert_at_end_list(list, new_node_data));
		new_node->next = temp;
		new_node->prev = temp->prev;
		temp->prev->next = new_node;
		temp->prev = new_node;
		new_node->index = new_node->prev->index + 1;
		update_list_index(temp);
	}
	list->list_size++;
}
