/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:42:06 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/15 19:51:10 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char *quoted_section(char *input, int *i, int *segment_start, char *quote)
{
	char	*temp;
	
	*quote = input[(*i)++];
	*segment_start = *i;
	while (input[*i] && input[*i] != *quote)
		(*i)++;
	temp = ft_substr(input, *segment_start - 1, (*i - *segment_start) + 2);
	if (input[*i] == *quote)
		(*i)++;
	return (temp);
}
char *unquoted_section(char *input, int *i, int *segment_start)
{
	char	*temp;

	*segment_start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	temp = ft_substr(input, *segment_start, *i - *segment_start);
	return (temp);
}

int	quoted_word_to_node(char *input, t_list *list, int start, int *i, t_gc_list *grbg_collector)
{
	char	*result;
	char	*temp;
	int		segment_start;
	char	quote;

	result = ft_strdup("");
	while (input[*i] && !(input[*i] == ' ' || input[*i] == '|'
			|| input[*i] == '<' || input[*i] == '>'))
	{
		if (input[*i] == '"' || input[*i] == '\'')
			temp = quoted_section(input, i, &segment_start, &quote);
		else
			temp = unquoted_section(input, i, &segment_start);
		result = ft_strjoin_free(result, temp);
	}
	insert_at_end_list(list, result, grbg_collector);
	start = *i;
	return (start);
}

t_list	*input_to_list(char *input, t_gc_list *grbg_collector)
{
	t_list	*list;
	int		i;
	int		start;

	if (!input)
		return (NULL);
	i = 0;
	list = init_list(grbg_collector);
	start = i;
	while (input[i])
	{
		start = split_symbols(input, list, start, &i, grbg_collector);
		start = split_redirections(input, list, start, &i, grbg_collector);
		if (input[i] && is_space(input[i]) && input[i] != '|'
			&& input[i] != '<' && input[i] != '>')
		{
			start = quoted_word_to_node(input, list, start, &i, grbg_collector);
			continue ;
		}
		if (input[i])
			i++;
		if (input[i] == '\0' && i != start)
			insert_at_end_list(list, ft_substr(input, start, i - start), grbg_collector);
	}
	free(input);
	return (list);
}
	