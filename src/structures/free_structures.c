/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:38:44 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/03/18 21:00:16 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/structures.h"

void	free_list(t_list *list)
{
	t_list_node	*current;
	t_list_node	*next;

	current = list->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	list->head = NULL;
	free(list);
}

void	free_stack(t_stack *stack)
{
	while (stack->top > 0)
		pop(stack);
	free(stack->stack);
	free(stack);
}
