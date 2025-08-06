/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 13:16:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 22:28:42 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_quotes_in_string(int *count_quotes, char *string, int *i,
		int quotes_ascii)
{
	(*count_quotes)++;
	(*i)++;
	while (string[*i] && string[*i] != quotes_ascii)
		(*i)++;
	if (string[*i] == quotes_ascii)
	{
		(*count_quotes)++;
		(*i)++;
	}
}

int	check_if_quotes_balanced(char *string)
{
	int	i;
	int	count_double_quotes;
	int	count_single_quotes;

	i = 0;
	count_double_quotes = 0;
	count_single_quotes = 0;
	while (string[i])
	{
		if (string[i] == 34)
			skip_quotes_in_string(&count_double_quotes, string, &i, 34);
		else if (string[i] == 39)
			skip_quotes_in_string(&count_single_quotes, string, &i, 39);
		else
			i++;
	}
	if (count_double_quotes % 2 != 0 || count_single_quotes % 2 != 0)
		return (1);
	return (0);
}

int	check_if_contains_quotes(char *string)
{
	if (ft_strchr(string, 34) || ft_strchr(string, 39))
		return (1);
	else
		return (0);
}

void	remove_quotes_from_substring(char *string, char **copy_string, int *i,
		int quotes_ascii)
{
	(*i)++;
	while (string[*i] && string[*i] != quotes_ascii)
	{
		*copy_string = append_char_to_string(*copy_string, string[*i]);
		(*i)++;
	}
	(*i)++;
}

void	remove_quotes_from_string(char *string)
{
	int		i;
	char	*copy_string;

	i = 0;
	copy_string = malloc(1 * sizeof(char));
	if (copy_string == NULL)
		return ;
	copy_string[0] = '\0';
	while (string[i])
	{
		if (string[i] == 34)
			remove_quotes_from_substring(string, &copy_string, &i, 34);
		else if (string[i] == 39)
			remove_quotes_from_substring(string, &copy_string, &i, 39);
		else
		{
			copy_string = append_char_to_string(copy_string, string[i]);
			i++;
		}
	}
	ft_strlcpy(string, copy_string, ft_strlen(copy_string) + 1);
	free(copy_string);
}
