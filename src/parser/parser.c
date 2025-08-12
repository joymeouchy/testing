/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:42:06 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/12 19:56:07 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_strjoin_free(char *s1, char *s2, t_gc_list *grbg_collector)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2, grbg_collector));
	if (!s2)
		return (ft_strdup(s1, grbg_collector));
	joined = ft_strjoin(s1, s2, grbg_collector);
	return (joined);
}

static char	*quoted_section(t_parse_state *state, char *quote,
		t_gc_list *grbg_collector)
{
	char	*temp;

	*quote = state->input[state->i++];
	state->start = state->i;
	while (state->input[state->i] && state->input[state->i] != *quote)
		state->i++;
	temp = ft_substr(state->input, state->start - 1,
			(state->i - state->start) + 2, grbg_collector);
	if (state->input[state->i] == *quote)
		state->i++;
	return (temp);
}

static char	*unquoted_section(t_parse_state *state, t_gc_list *grbg_collector)
{
	char	*temp;

	state->start = state->i;
	while (state->input[state->i] && !is_space(state->input[state->i])
		&& state->input[state->i] != '|' && state->input[state->i] != '<'
		&& state->input[state->i] != '>' && state->input[state->i] != '"'
		&& state->input[state->i] != '\'')
		state->i++;
	temp = ft_substr(state->input, state->start,
			state->i - state->start, grbg_collector);
	return (temp);
}

static void	quoted_word_to_node(t_parse_state *state, t_list *list,
		t_gc_list *grbg_collector)
{
	char	*result;
	char	*temp;
	char	quote;

	result = ft_strdup("", grbg_collector);
	while (state->input[state->i] && !is_space(state->input[state->i])
		&& state->input[state->i] != '|' && state->input[state->i] != '<'
		&& state->input[state->i] != '>')
	{
		if (state->input[state->i] == '"' || state->input[state->i] == '\'')
			temp = quoted_section(state, &quote, grbg_collector);
		else
			temp = unquoted_section(state, grbg_collector);
		result = ft_strjoin_free(result, temp, grbg_collector);
	}
	insert_at_end_list(list, result, grbg_collector);
	state->start = state->i;
}

t_list	*input_to_list(char *input, t_gc_list *grbg_collector)
{
	t_list			*list;
	t_parse_state	state;
	int				input_len;

	if (!input)
		return (NULL);
	list = init_list(grbg_collector);
	state.input = input;
	state.i = 0;
	state.start = 0;
	input_len = ft_strlen(input);
	while (state.input && state.i < input_len && state.input[state.i])
	{
		split_symbols(&state, list, grbg_collector);
		split_redirections(&state, list, grbg_collector);
		if (state.input[state.i] && !is_space(state.input[state.i])
			&& state.input[state.i] != '|' && state.input[state.i] != '<'
			&& state.input[state.i] != '>')
			quoted_word_to_node(&state, list, grbg_collector);
		else
			state.i++;
	}
	if (state.i != state.start)
		insert_at_end_list(list, ft_substr(state.input, state.start,
				state.i - state.start, grbg_collector), grbg_collector);
	return (list);
}
