/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:38:44 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/19 09:31:46 by jmeouchy         ###   ########.fr       */
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

void	free_2darray(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
