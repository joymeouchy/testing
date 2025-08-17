/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:20:37 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/17 23:28:43 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_closing_quote(char *str, int start, char quote)
{
	int	i;

	i = start;
	while (str[i] && str[i] != quote)
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

static char	*rebuild_str(char *str, int i, int end, t_gc_list *gc)
{
	char	*before;
	char	*quoted;
	char	*after;

	before = ft_substr(str, 0, i, gc);
	quoted = ft_substr(str, i + 1, end - i, gc);
	after = ft_substr(str, end + 1, ft_strlen(str) - (end + 1), gc);
	return (join_parts(before, quoted, after, gc));
}

static bool	is_dollar_followed_by_quote(char *str, int i)
{
	return (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '"'));
}

// static bool	not_in_quotes(t_expansion_state state)
// {
// 	return (!state.in_single && !state.in_double);
// }

char	*remove_dollar_from_quoted_strings(char *str, t_gc_list *gc)
{
	t_expansion_state	state;

	state.str = str;
	state.i = 0;
	state.in_single = 0;
	state.in_double = 0;
	while (state.str[state.i])
	{
		if (state.str[state.i] == '\'' && !state.in_double)
			state.in_single = !state.in_single;
		else if (state.str[state.i] == '"' && !state.in_single)
			state.in_double = !state.in_double;
		if (is_dollar_followed_by_quote(state.str, state.i))
		{
			state.end = find_closing_quote(state.str, state.i + 2,
					state.str[state.i + 1]);
			if (state.end == -1)
				break ;
			state.str = rebuild_str(state.str, state.i, state.i, gc);
			state.i += state.end - state.i;
			continue ;
		}
		state.i++;
	}
	return (state.str);
}
