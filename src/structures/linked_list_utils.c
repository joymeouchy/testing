/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 06:42:48 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/16 20:55:57 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*init_list(t_gc_list *grbg_collector)
{
	t_list	*list;

	list = ft_malloc(sizeof(t_list), grbg_collector);
	list->head = NULL;
	list->list_size = 0;
	return (list);
}

t_list_node	*create_list_node(char *data, t_gc_list *grgb_collector)
{
	t_list_node	*new_node;

	new_node = ft_malloc(sizeof(t_list_node), grgb_collector);
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->index = 0;
	new_node->redir_arg = NULL;
	return (new_node);
}

void	insert_at_end_list(t_list *list, char *new_node_data, t_gc_list *grgb_collector)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	if (*new_node_data == '\0')
	{
		// free(new_node_data);
		return ;
	}
	new_node = create_list_node(new_node_data, grgb_collector);
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
	}
	list->list_size++;
}

void	insert_at_beginning_list(t_list *list, char *new_node_data, t_gc_list *grgb_collector)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	new_node = create_list_node(new_node_data, grgb_collector);
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

void	insert_at_middle_list(t_list *list, char *new_node_data, int index, t_gc_list *grgb_collector)
{
	t_list_node	*new_node;
	t_list_node	*temp;

	if (index == 0)
		return (insert_at_beginning_list(list, new_node_data, grgb_collector));
	new_node = create_list_node(new_node_data, grgb_collector);
	if (list->head == NULL)
		list->head = new_node;
	else
	{
		temp = list->head;
		while (temp->index < index && temp->next)
			temp = temp->next;
		if (!temp)
			return (insert_at_end_list(list, new_node_data, grgb_collector));
		new_node->next = temp;
		new_node->prev = temp->prev;
		temp->prev->next = new_node;
		temp->prev = new_node;
		new_node->index = new_node->prev->index + 1;
		update_list_index(temp);
	}
	list->list_size++;
}
