/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:16:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/09 16:03:24 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_and_remove_quotes(t_list *list)
{
	t_list_node	*temp;
	int			flag_balanced_quotes;

	temp = list->head;
	flag_balanced_quotes = 0;
	if (!list || !list->head)
		return (1);
	while (temp)
	{
		if (check_if_contains_quotes(temp->data) == 1)
			flag_balanced_quotes = check_if_quotes_balanced(temp->data);
		if (flag_balanced_quotes == 1)
		{
			ft_putendl_fd("syntax error: unclosed quote", 2);
			return (2);
		}
		remove_quotes_from_string(temp->data);
		if (temp->token != LEFT_D_REDIRECTION && temp->redir_arg)
			remove_quotes_from_string(temp->redir_arg);
		temp = temp->next;
	}
	return (0);
}
