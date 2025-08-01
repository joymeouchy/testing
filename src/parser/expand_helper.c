/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:47:40 by root              #+#    #+#             */
/*   Updated: 2025/08/01 13:18:12 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getenv(char **envp, const char *var_name)
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
	while (str[i] && str[i] != '$')
	{
		if (str[i] == '"')
			in_double_quote = !in_double_quote;
		i++;
	}
	if (str[i] == '\0')
		return (true);
	i++;
	if (str[i] == '?' || str[i] == '$')
		return (false);
	if (str[i] == '\0' || ((ft_isalnum(str[i]) == 0 || str[i] == '0'
				|| str[i] == '$') && in_double_quote == 0))
		return (true);
	return (false);
}

char	*extract_variable_name(char *str, t_gc_list *grbg_collector)
{
	int	i;
	int	start;

	i = find_dollar(str);
	i++;
	start = i;
	if (str[i] == '?' || str[i] == '$' || (str[i] >= '1' && str[i] <= '9'))
		return (ft_substr(str, start, i + 1 - start, grbg_collector));
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, start, i - start, grbg_collector));
}
