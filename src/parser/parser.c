/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:42:06 by lkhoury           #+#    #+#             */
/*   Updated: 2025/05/16 09:42:59 by jmeouchy         ###   ########.fr       */
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

int	split_symbols(char *input, t_list *list, int start, int *i)
{
	if (input[*i] == ' ')
	{
		if (*i != start)
			insert_at_end_list(list, ft_substr(input, start, *i - start));
		while (input[*i] == ' ')
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


int	double_quotes_to_node(char *input, t_list *list, int start, int *i)
{
	(*i)++;
	while (input[*i] != '"')
		(*i)++;
	(*i)++;
	while (input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
			&& input[*i] != ' ' && input[*i] != '\0')
		(*i)++;
	insert_at_end_list(list, ft_substr(input, start, *i - start));
	start = *i;
	*(i) -= 1;
	return (start);
}

int	single_quotes_to_node(char *input, t_list *list, int start, int *i)
{
	(*i)++;
	while (input[*i] != 39)
		(*i)++;
	(*i)++;
	while (input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
			&& input[*i] != ' ' && input[*i] != '\0')
		(*i)++;
	insert_at_end_list(list, ft_substr(input, start, *i - start));
	start = *i;
	*(i) -= 1;
	return (start);
}

t_list	*input_to_list(char *input)
{
	t_list	*list;
	int		i;
	int		start;

	if (input == NULL)
		return (NULL);
	i = 0;
	list = init_list();
	start = i;
	while (input[i])
	{
		start = split_symbols(input, list, start, &i);
		start = split_redirections(input, list, start, &i);
		if (input[i] == '"')
			start = double_quotes_to_node(input, list, start, &i);
		if (input[i] == 39)
			start = single_quotes_to_node(input, list, start, &i);
		if (input[i])
			i++;
		if (input[i] == '\0' && i != start)
			insert_at_end_list(list, ft_substr(input, start, i - start));
	}
	if (input)
		free(input);
	return (list);
}
