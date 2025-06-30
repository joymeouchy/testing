/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/06/30 19:04:16 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	*process_expansion(char *str, char **envp)
{
	int		dollar_pos;
	char	*var_name;
	char	*replacement;
	char	*expanded;

	dollar_pos = find_dollar(str);
	while (dollar_pos != -1)
	{
		var_name = extract_variable_name(&str[dollar_pos]);
		if (!var_name)
			return (free(str), NULL);
		replacement = ft_getenv(envp, var_name);
		if (!replacement)
			replacement = "";
		expanded = replace_variable(str, var_name, replacement, dollar_pos);
		free(str);
		str = expanded;
		free(var_name);
		dollar_pos = find_dollar(str);
	}
	return (str);
}

char	*expand(char *str, char **envp)
{
	char	*result;

	if (!envp)
		return (NULL);
	if (check_if_dollar_to_print(str))
		return (str);
	result = process_expansion(str, envp);
	return (result);
}

void	expand_list(t_list *list, t_envp *env)
{
	t_list_node	*current;

	current = list->head;
	while (current)
	{
		if (ft_strcmp(current->data, "$?") == 0)
		{
			current->data = ft_itoa(env->exit_code);
		}
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, env->environment);
			if (!current->data)
				return ;
		}
		current = current->next;
	}
}
