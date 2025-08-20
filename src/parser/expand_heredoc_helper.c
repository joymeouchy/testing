/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 20:39:37 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/20 20:49:28 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_dollar_heredoc(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || *str == '\0')
		return (-1);
	while (str[i])
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

static char	*extract_variable_name_heredoc(char *str, t_gc_list *grbg_collector)
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

static int	handle_named_var_heredoc(char *str, t_envp *env, char **replacement,
		t_gc_list *gc)
{
	char	*var;

	var = extract_variable_name_heredoc(str, gc);
	*replacement = ft_getenv(env->environment, var);
	if (!*replacement)
		*replacement = ft_calloc(1, 1, gc);
	return (ft_strlen(var) + 1);
}

static int	get_replacement_len_and_value_heredoc(t_expansion_state *state,
			t_envp *env, t_gc_list *gc)
{
	int	len;

	len = handle_special_vars(state->str[state->pos + 1], env,
			&state->replacement, gc);
	if (len == 0 && (ft_isalnum(state->str[state->pos + 1])
			|| state->str[state->pos + 1] == '_'))
		len = handle_named_var_heredoc(&state->str[state->pos], env,
				&state->replacement, gc);
	return (len);
}

char	*process_expansion_heredoc(char *str, t_envp *env, t_gc_list *gc)
{
	t_expansion_state	state;
	int					dollar_pos;

	state.str = str;
	state.i = 0;
	dollar_pos = find_dollar_heredoc(&state.str[state.i]);
	while (dollar_pos != -1)
	{
		dollar_pos += state.i;
		state.pos = dollar_pos;
		state.replacement = NULL;
		state.len = get_replacement_len_and_value_heredoc(&state, env, gc);
		if (state.len == 0)
		{
			state.i = dollar_pos + 1;
			dollar_pos = find_dollar_heredoc(&state.str[state.i]);
			continue ;
		}
		state.str = replace_segment(&state, gc);
		state.i = dollar_pos + ft_strlen(state.replacement);
		dollar_pos = find_dollar_heredoc(&state.str[state.i]);
	}
	return (state.str);
}
