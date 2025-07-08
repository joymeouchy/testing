/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 12:39:06 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/08 09:29:17 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	push_stack_to_other(t_stack *src, t_stack *dst)
{
	while (src->top > -1)
	{
		push(src->stack[src->top].data,
			src->stack[src->top].token,
			src->stack[src->top].redir_arg, dst);
		pop(src);
	}
}

t_stack	*malloc_and_init_stack(t_list *list)
{
	t_stack	*stack;

	stack = malloc(sizeof(t_stack));
	init_stack(list, stack);
	return (stack);
}

static void	process_token(t_list_node *temp,
	t_stack *word, t_stack *cmd, t_stack *output)
{
	if (temp->token > 0)
		push(temp->data, temp->token, temp->redir_arg, word);
	else
	{
		if (cmd->stack[cmd->top].token == PIPE)
		{
			push_stack_to_other(word, output);
			push_stack_to_other(cmd, output);
		}
		push(temp->data, temp->token, temp->redir_arg, cmd);
	}
}

static void	push_remaining_stacks(t_stack *word, t_stack *cmd, t_stack *output)
{
	if (word->top > -1)
		push_stack_to_other(word, output);
	if (cmd->top > -1)
		push_stack_to_other(cmd, output);
}

t_stack	*shunting_yard(t_list *list)
{
	t_stack		*stack;
	t_stack		*cmd_stack;
	t_stack		*word_stack;
	t_list_node	*temp;

	if (!list || !list->head)
		return (NULL);
	temp = list->head;
	stack = malloc_and_init_stack(list);
	cmd_stack = malloc_and_init_stack(list);
	word_stack = malloc_and_init_stack(list);
	while (temp)
	{
		process_token(temp, word_stack, cmd_stack, stack);
		temp = temp->next;
	}
	push_remaining_stacks(word_stack, cmd_stack, stack);
	free_stack(cmd_stack);
	free_stack(word_stack);
	return (stack);
}
