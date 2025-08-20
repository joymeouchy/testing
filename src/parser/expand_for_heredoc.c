/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_for_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/20 20:47:54 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_dollar_followed_by_quote(char *str, int i)
{
	return (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '"'));
}

static bool	is_special_var(char *str, int i)
{
	return (str[i] == '$' && (str[i + 1] == '?' || str[i + 1] == '$'));
}

static int	handle_dollar_followed_by_quote_heredoc(t_expansion_state *state,
		t_gc_list *gc)
{
	if (is_dollar_followed_by_quote(state->str, state->i))
	{
		state->end = find_closing_quote(state->str, state->i + 2,
				state->str[state->i + 1]);
		if (state->end == -1)
			return (0);
		state->str = rebuild_str(state->str, state->i, state->end, gc);
		state->i = state->end;
		return (1);
	}
	return (0);
}

static char	*remove_dollar_heredoc(char *str, t_gc_list *gc)
{
	t_expansion_state	state;
	int					len;

	state.str = str;
	state.i = 0;
	len = ft_strlen(str);
	while (state.i && state.i < len && state.str[state.i])
	{
		if (is_special_var(state.str, state.i))
			state.i += 2;
		else if (handle_dollar_followed_by_quote_heredoc(&state, gc))
			continue ;
		else
			state.i++;
	}
	return (state.str);
}

char	*expand_heredoc(char *str, t_envp *env, t_gc_list *gc)
{
	if (!str || !env || !env->environment)
		return (NULL);
	str = remove_dollar_heredoc(str, gc);
	return (process_expansion_heredoc(str, env, gc));
}
