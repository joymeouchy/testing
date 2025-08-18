/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/08/18 19:02:51 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_replacement_len_and_value(t_expansion_state *state, t_envp *env,
		t_gc_list *gc)
{
	int	len;

	len = handle_special_vars(state->str[state->pos + 1], env,
			&state->replacement, gc);
	if (len == 0 && is_valid_var_char(state->str[state->pos + 1]))
		len = handle_named_var(&state->str[state->pos], env,
				&state->replacement, gc);
	return (len);
}

char	*process_expansion(char *str, t_envp *env, t_gc_list *gc)
{
	t_expansion_state	state;
	int					dollar_pos;

	state.str = str;
	state.i = 0;
	dollar_pos = find_dollar(&state.str[state.i]);
	while (dollar_pos != -1)
	{
		dollar_pos += state.i;
		state.pos = dollar_pos;
		state.replacement = NULL;
		state.len = get_replacement_len_and_value(&state, env, gc);
		if (state.len == 0)
		{
			state.i = dollar_pos + 1;
			dollar_pos = find_dollar(&state.str[state.i]);
			continue ;
		}
		state.str = replace_segment(&state, gc);
		state.i = dollar_pos + ft_strlen(state.replacement);
		dollar_pos = find_dollar(&state.str[state.i]);
	}
	return (state.str);
}

char	*expand(char *str, t_envp *env, t_gc_list *gc)
{
	if (!str || !env || !env->environment)
		return (NULL);
	str = remove_dollar_from_quoted_strings(str, gc);
	return (process_expansion(str, env, gc));
}

void	expand_list(t_list *list, t_envp *env, t_gc_list *grbg_collector)
{
	t_list_node	*current;

	current = list->head;
	while (current)
	{
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, env, grbg_collector);
			if (!current->data)
				return ;
			if (current->data[0] == '\0')
				delete_node(list, current);
		}
		current = current->next;
	}
}
