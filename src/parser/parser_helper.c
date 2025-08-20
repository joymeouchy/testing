/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:54:18 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/20 20:20:14 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	one_element_input_to_list(t_parse_state *state, t_list *list,
		t_gc_list *grbg_collector)
{
	insert_at_end_list(list, ft_substr(state->input, state->start,
			state->i - state->start, grbg_collector), grbg_collector);
	insert_at_end_list(list, ft_substr(state->input, state->i, 1,
			grbg_collector), grbg_collector);
	state->start = state->i + 1;
}

void	two_element_input_to_list(t_parse_state *state, t_list *list,
		t_gc_list *grbg_collector)
{
	insert_at_end_list(list, ft_substr(state->input, state->start,
			state->i - state->start, grbg_collector), grbg_collector);
	insert_at_end_list(list, ft_substr(state->input, state->i, 2,
			grbg_collector), grbg_collector);
	state->i++;
	state->start = state->i + 1;
}

char	*ft_strjoin_free(char *s1, char *s2, t_gc_list *grbg_collector)
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
