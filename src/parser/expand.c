/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/07/21 20:42:43 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_variable(char *str, char *var_name,
	char *replacement, int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos
			+ ft_strlen(var_name) + 1, ft_strlen(str), grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	// free(before);
	// free(after);
	// free(temp);
	return (result);
}

static char	*process_expansion(char *str, t_envp *env, t_gc_list *grbg_collector)
{
	int		dollar_pos;
	char	*var_name;
	char	*replacement;
	char	*expanded;

	dollar_pos = find_dollar(str);
	while (dollar_pos != -1)
	{
		if(str[dollar_pos + 1] == '?')
		{
			var_name = "$?";
			replacement = ft_itoa(env->exit_code, grbg_collector);
		}
		else
		{
			if (!(ft_isalpha(str[dollar_pos + 1]) ||
				str[dollar_pos + 1] == '_' ||
				(str[dollar_pos + 1] >= '0' && str[dollar_pos + 1] <= '9')))
			{
				dollar_pos = find_dollar(&str[dollar_pos + 1]);
				continue;
			}
			var_name = extract_variable_name(&str[dollar_pos], grbg_collector);
			if (!var_name)
				return (NULL);
			replacement = ft_getenv(env->environment, var_name);
			if (!replacement)
				replacement = ft_calloc(1, 1, grbg_collector);
		}
		expanded = replace_variable(str, var_name, replacement, dollar_pos, grbg_collector);
		str = expanded;
		dollar_pos = find_dollar(str);
	}
	return (str);
}

char	*expand(char *str, t_envp *env, t_gc_list *grbg_collector)
{
	char	*result;

	if (!env || !env->environment)
		return (NULL);
	if (check_if_dollar_to_print(str))
		return (str);
	result = process_expansion(str, env, grbg_collector);
	return (result);
}

void	expand_list(t_list *list, t_envp *env, t_gc_list *grbg_collector)
{
	t_list_node	*current;

	current = list->head;
	while (current)
	{
		if (ft_strcmp(current->data, "$?") == 0)
		{
			current->data = ft_itoa(env->exit_code, grbg_collector);
		}
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, env, grbg_collector);
			if (!current->data)
				return ;
		}
		current = current->next;
	}
}

