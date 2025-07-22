/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:54:18 by root              #+#    #+#             */
/*   Updated: 2025/07/22 20:39:15 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	one_element_input_to_list(char *input, t_list *list, int *start, int *i, t_gc_list *grbg_collector)
{
	insert_at_end_list(list, ft_substr(input, *start, *i - *start, grbg_collector), grbg_collector);
	insert_at_end_list(list, ft_substr(input, *i, 1, grbg_collector), grbg_collector);
	*start = *i + 1;
}

void	two_element_input_to_list(char *input, t_list *list, int *start, int *i, t_gc_list *grbg_collector)
{
	insert_at_end_list(list, ft_substr(input, *start, *i - *start, grbg_collector), grbg_collector);
	insert_at_end_list(list, ft_substr(input, *i, 2, grbg_collector), grbg_collector);
	*start = ++*i + 1;
}

int	split_redirections(char *input, t_list *list, int start, int *i, t_gc_list *grbg_collector)
{
	if ((input[*i] == '<' && input[*i + 1] == '<')
		|| (input[*i] == '>' && input[*i + 1] == '>'))
		two_element_input_to_list(input, list, &start, i, grbg_collector);
	else if (input[*i] == '<' || input[*i] == '>')
		one_element_input_to_list(input, list, &start, i, grbg_collector);
	return (start);
}

bool	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (true);
	return (false);
}

void skip_spaces(char *input, int *i, int *start)
{
    while (is_space(input[*i]))
        (*i)++;
    *start = *i;
}

int	split_symbols(char *input, t_list *list, int start, int *i, t_gc_list *grbg_collector)
{
	if (is_space(input[*i]))
	{
		if (*i > start)
			insert_at_end_list(list, ft_substr(input, start, *i - start, grbg_collector), grbg_collector);
		skip_spaces(input, i, &start);
	}
	if (input[*i] == '|')
	{
		insert_at_end_list(list, ft_substr(input, start, *i - start, grbg_collector), grbg_collector);
		insert_at_end_list(list, ft_substr(input, *i, 1, grbg_collector), grbg_collector);
		start = *i + 1;
	}
	return (start);
}
		