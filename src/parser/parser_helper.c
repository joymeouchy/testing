/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:54:18 by root              #+#    #+#             */
/*   Updated: 2025/07/14 19:46:01 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	one_element_input_to_list(char *input, t_list *list, int *start, int *i)
{
	insert_at_end_list(list, ft_substr(input, *start, *i - *start));
	insert_at_end_list(list, ft_substr(input, *i, 1));
	*start = *i + 1;
}

void	two_element_input_to_list(char *input, t_list *list, int *start, int *i)
{
	insert_at_end_list(list, ft_substr(input, *start, *i - *start));
	insert_at_end_list(list, ft_substr(input, *i, 2));
	*start = ++*i + 1;
}

int	split_redirections(char *input, t_list *list, int start, int *i)
{
	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
		two_element_input_to_list(input, list, &start, i);
	else if (input[*i] == '<' || input[*i] == '>')
		one_element_input_to_list(input, list, &start, i);
	return (start);
}

bool	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\f' || c == '\r' || c == '\v')
		return (true);
	return (false);
}

int	split_symbols(char *input, t_list *list, int start, int *i)
{
	if (is_space(input[*i]))
	{
		if (*i != start)
			insert_at_end_list(list, ft_substr(input, start, *i - start));
		while (is_space(input[*i]))
			(*i)++;
		start = *i;
	}
	if (input[*i] == '|')
	{
		insert_at_end_list(list, ft_substr(input, start, *i - start));
		insert_at_end_list(list, ft_substr(input, *i, 1));
		start = *i + 1;
	}
	return (start);
}
	