/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_pipe_argument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:26:02 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/19 19:25:31 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_word_to_pipe(t_list_node *node, t_list *list)
{
	if (node && node->next && node->token == PIPE && node->next->token == WORD)
	{
		node->redir_arg = node->next->data;
		delete_node(list, node->next);
	}
}

void	add_arg_to_pipe(t_list *list)
{
	t_list_node	*index;

	index = list->head;
	while (index)
	{
		add_word_to_pipe(index, list);
		index = index->next;
	}
}
