/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_redir_argument.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:26:02 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/22 11:10:53 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void add_arg_to_redir(t_list *list)
{
	t_list_node *index;

	index = list->head;
	while (index)
	{
		if (index->next && (index->token >= LEFT_REDIRECTION && index->token <= RIGHT_D_REDIRECTION))
		{
			index->redir_arg = index->next->data;
			delete_node(list, index->next);
		}
		index = index->next;
	}
}

void swap_red(t_tree_node *node1, t_tree_node *node2)
{
	t_tree_node *temp;
	
	if(!node1 || !node2)
		return ;
	temp = (t_tree_node *)malloc(sizeof(t_tree_node));
	if (!temp)
		return ;
	if(node1->token == COMMAND || node1->token == BUILT_IN || node1->token == WORD)
	{
		if(node2->token >= LEFT_REDIRECTION && node2->token <= RIGHT_D_REDIRECTION)
		{
			temp->data = node1->data;
			node1->data = node2->data;
			node2->data = temp->data;
			temp->token = node1->token;
			node1->token = node2->token;
			node2->token = temp->token;
			temp->path = node1->path;
			node1->path = node2->path;
			node2->path = temp->path;
			temp->redir_arg = node1->redir_arg;
			node1->redir_arg = node2->redir_arg;
			node2->redir_arg = temp->redir_arg;
		}
		free (temp);
	}
}
