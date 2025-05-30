/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/05/26 11:40:33 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *ft_getenv(char **envp, const char *var_name)
{
	int		len;
	char	*equal_sign;

	if (!envp || !var_name)
		return (NULL);
	len = ft_strlen(var_name);
	while (*envp)
	{
		if (ft_strncmp(*envp, var_name, len) == 0 && (*envp)[len] == '=')
		{
			equal_sign = ft_strchr(*envp, '=');
			if (equal_sign)
				return (equal_sign + 1);
		}
		envp++;
	}
	return (NULL);
}

int	find_dollar(char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	if (str == NULL || *str == '\0')
		return (-1);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (str[i] == '$' && !in_single_quote)
			return (i);
		i++;
	}
	return (-1);
}

bool	check_if_dollar_to_print(char *str)
{
	int	i;
	int	in_double_quote;

	i = 0;
	in_double_quote = 0;
	while (str[i] != '$')
	{
		if (str[i] == '"' )
			in_double_quote = !in_double_quote;
		i++;
	}
	i += 1;
	if (str[i] == '\0' || ((ft_isalnum(str[i]) == 0 || str[i] == '0' || str[i] == '$') && in_double_quote == 0))
		return (true);
	return (false);
}

char	*extract_variable_name(char *str)
{
	int	i;
	int	start;

	i = find_dollar(str);
	i++;
	start = i;
	if (str[i] >= '1' && str[i] <= '9')
		return (ft_substr(str, start, i + 1 - start));
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, start, i - start));
}

char	*replace_variable(char *str, char *var_name,
	char *replacement, int dollar_pos)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos);
	after = ft_substr(str, dollar_pos
			+ ft_strlen(var_name) + 1, ft_strlen(str));
	temp = ft_strjoin(before, replacement);
	result = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	return (result);
}

char *expand(char *str, char **envp)
{
	int		dollar_pos;
	char	*var_name;
	char	*replacement;
	char	*expanded_str;

	if (check_if_dollar_to_print(str))
		return (str);
	dollar_pos = find_dollar(str);
	while ((dollar_pos) != -1)
	{
		var_name = extract_variable_name(&str[dollar_pos]);
		if (!var_name)
			return (free(str),NULL);
		replacement = ft_getenv(envp, var_name);
		if (!replacement)
			replacement = "";
		expanded_str = replace_variable(str, var_name, replacement, dollar_pos);
		free(str);
		str = expanded_str;
		free(var_name);
		dollar_pos = find_dollar(str);
	}
	return (str);
}

void expand_list(t_list *list, char **envp)
{
	t_list_node	*current;

	current = list->head;
	while (current)
	{
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, envp);
			if (!current->data)
				return ;
		}
		current = current->next;
	}
}
