/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 06:42:48 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/14 19:46:23 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/structures.h"

void	init_stack(t_list *list, t_stack *stack)
{
	if (list == NULL || stack == NULL || list->list_size <= 0)
		return ;
	stack->stack = malloc(sizeof(t_stack_element) * (list->list_size + 1));
	if (!stack->stack)
		return ;
	stack->top = -1;
}

void	push(char *data, e_tokens token, char *redir_args, t_stack *stack)
{
	stack->top += 1;
	stack->stack[stack->top].data = data;
	stack->stack[stack->top].token = token;
	stack->stack[stack->top].redir_arg = redir_args;
}

void	pop(t_stack *stack)
{
	if (stack->top == -1)
		printf("\nUnderflow!!\n");
	else
		stack->top = stack->top - 1;
}

void	print_stack(t_stack *stack)
{
	int	i;

	i = 0;
	printf("\nstack:\n");
	while (i <= stack->top)
	{
		printf("stack element is:%s, token is: %d redirection: %s\n",
			stack->stack[i].data, stack->stack[i].token,
			stack->stack[i].redir_arg);
		i++;
	}
}
	