/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_split.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:54:18 by root              #+#    #+#             */
/*   Updated: 2025/08/13 19:29:16 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void skip_spaces(t_parse_state *state)
{
    while (state->input[state->i] && is_space(state->input[state->i]))
        state->i++;
    state->start = state->i;
}

void	split_redirections(t_parse_state *state, t_list *list,
		t_gc_list *grbg_collector)
{
	if ((state->input[state->i] == '<' && state->input[state->i + 1] == '<')
		|| (state->input[state->i] == '>' && state->input[state->i + 1] == '>'))
		two_element_input_to_list(state, list, grbg_collector);
	else if (state->input[state->i] == '<' || state->input[state->i] == '>')
		one_element_input_to_list(state, list, grbg_collector);
}

void	split_symbols(t_parse_state *state, t_list *list,
		t_gc_list *grbg_collector)
{
	if (is_space(state->input[state->i]))
	{
		if (state->i > state->start)
			insert_at_end_list(list, ft_substr(state->input, state->start,
					state->i - state->start, grbg_collector), grbg_collector);
		skip_spaces(state);
	}
	if (state->input[state->i] == '|')
	{
		insert_at_end_list(list, ft_substr(state->input, state->start,
				state->i - state->start, grbg_collector), grbg_collector);
		insert_at_end_list(list, ft_substr(state->input, state->i, 1,
				grbg_collector), grbg_collector);
		state->start = state->i + 1;
		state->i++;
	}
}
