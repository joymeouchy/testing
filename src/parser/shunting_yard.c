/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:39:06 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/01 13:22:41 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	push_stack_to_other(t_stack *src, t_stack *dst)
{
	while (src->top > -1)
	{
		push(src->stack[src->top].data, src->stack[src->top].token,
			src->stack[src->top].redir_arg, dst);
		pop(src);
	}
}

t_stack	*malloc_and_init_stack(t_list *list, t_gc_list *grgb_collector)
{
	t_stack	*stack;

	stack = ft_malloc(sizeof(t_stack), grgb_collector);
	init_stack(list, stack, grgb_collector);
	return (stack);
}

static void	process_token(t_list_node *temp, t_stack *word, t_stack *cmd,
		t_stack *output)
{
	if (temp->token > 0)
		push(temp->data, temp->token, temp->redir_arg, word);
	else
	{
		if (cmd && cmd->stack && cmd->top > -1
			&& cmd->stack[cmd->top].token == PIPE)
		{
			push_stack_to_other(word, output);
			push_stack_to_other(cmd, output);
		}
		push(temp->data, temp->token, temp->redir_arg, cmd);
	}
}

static void	push_remaining_stacks(t_stack *word, t_stack *cmd, t_stack *output)
{
	while (word->top > -1)
		push_stack_to_other(word, output);
	while (cmd->top > -1)
		push_stack_to_other(cmd, output);
}

t_stack	*shunting_yard(t_list *list, t_gc_list *grgb_collector)
{
	t_stack		*stack;
	t_stack		*cmd_stack;
	t_stack		*word_stack;
	t_list_node	*temp;

	if (!list || !list->head)
		return (NULL);
	temp = list->head;
	stack = malloc_and_init_stack(list, grgb_collector);
	cmd_stack = malloc_and_init_stack(list, grgb_collector);
	word_stack = malloc_and_init_stack(list, grgb_collector);
	while (temp)
	{
		process_token(temp, word_stack, cmd_stack, stack);
		temp = temp->next;
	}
	push_remaining_stacks(word_stack, cmd_stack, stack);
	return (stack);
}
