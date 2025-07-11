/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_redir_argument.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:26:02 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/19 09:33:42 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_arg_to_redir(t_list *list)
{
	t_list_node	*index;

	index = list->head;
	while (index)
	{
		if (index->next
			&& (index->token >= LEFT_REDIRECTION
				&& index->token <= RIGHT_D_REDIRECTION))
		{
			if (index && index->next && index->next->next
				&& index->next->token == PIPE
				&& index->next->next->token >= WORD)
			{
				index->redir_arg = index->next->next->data;
				delete_node(list, index->next->next);
			}
			else if (index && index->next && index->next->token >= WORD)
			{
				index->redir_arg = index->next->data;
				delete_node(list, index->next);
			}
		}
		index = index->next;
	}
}

static void	swap_node_content(t_tree_node *a, t_tree_node *b)
{
	char	*tmp_data;
	t_envp	*tmp_path;
	char	*tmp_redir_arg;
	int		tmp_token;

	tmp_data = a->data;
	tmp_path = a->path;
	tmp_redir_arg = a->redir_arg;
	tmp_token = a->token;
	a->data = b->data;
	a->path = b->path;
	a->redir_arg = b->redir_arg;
	a->token = b->token;
	b->data = tmp_data;
	b->path = tmp_path;
	b->redir_arg = tmp_redir_arg;
	b->token = tmp_token;
}

void	swap_red(t_tree_node *node1, t_tree_node *node2)
{
	int	tok1;
	int	tok2;

	if (!node1 || !node2)
		return ;
	tok1 = node1->token;
	tok2 = node2->token;
	if ((tok1 == COMMAND || tok1 == BUILT_IN || tok1 == WORD)
		&& (tok2 >= LEFT_REDIRECTION && tok2 <= RIGHT_D_REDIRECTION))
		swap_node_content(node1, node2);
}
