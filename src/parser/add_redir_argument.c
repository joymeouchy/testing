/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_redir_argument.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:26:02 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/18 19:11:17 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_word_to_pipe(t_list_node *node, t_list *list)
{
	if (node && node->next && node->token == PIPE
		&& node->next->token == WORD)
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

void	add_arg_to_redir(t_list *list)
{
	t_list_node	*index;

	index = list->head;
	while (index)
	{
		if (index->next && is_redirection(index->token))
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

static void	swap_node_strings(t_tree_node *a, t_tree_node *b)
{
	char	*tmp_data = a->data;
	t_envp	*tmp_path = a->path;
	char	*tmp_redir_arg = a->redir_arg;

	a->data = b->data;
	a->path = b->path;
	a->redir_arg = b->redir_arg;

	b->data = tmp_data;
	b->path = tmp_path;
	b->redir_arg = tmp_redir_arg;
}

static void	swap_node_flags(t_tree_node *a, t_tree_node *b)
{
	enum s_tokens	tmp_token = a->token;
	int				tmp_heredoc_created = a->heredoc_created;

	a->token = b->token;
	a->heredoc_created = b->heredoc_created;

	b->token = tmp_token;
	b->heredoc_created = tmp_heredoc_created;
}


static void	swap_node_content(t_tree_node *a, t_tree_node *b)
{
	swap_node_strings(a, b);
	swap_node_flags(a, b);
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
