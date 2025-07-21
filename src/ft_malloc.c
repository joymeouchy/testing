/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:00:37 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/21 19:35:18 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void *ft_malloc(int size, t_gc_list *grbg_collector)
{
	t_gc_node	*new_node;
	void		*ptr;
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = malloc(sizeof(t_gc_node));
	if(!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = grbg_collector->head;
	grbg_collector->head = new_node;
	return (ptr);
}

t_gc_list *init_grbg_collector(void)
{
	t_gc_list *grbg_collector;
	grbg_collector = malloc(sizeof(t_gc_list));
	if (!grbg_collector)
		return (NULL);
	grbg_collector->head = NULL;
	return (grbg_collector);
}

void ft_free_gc(t_gc_list *grbg_collector)
{
	t_gc_node   *current;
	t_gc_node   *next;

	if(!grbg_collector || !grbg_collector->head)
		return ;
	current = grbg_collector->head;
	while (current)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	grbg_collector->head = NULL;
	free(grbg_collector);
	rl_clear_history();
}
