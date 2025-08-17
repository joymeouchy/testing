/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:22:42 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/17 23:04:43 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_pid_from_proc(void)
{
	int		fd;
	char	buf[256];
	int		i;
	int		pid;

	i = 0;
	pid = 0;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	i = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (i <= 0)
		return (-1);
	buf[i] = '\0';
	i = 0;
	while (buf[i] >= '0' && buf[i] <= '9')
	{
		pid = pid * 10 + (buf[i] - '0');
		i++;
	}
	return (pid);
}

char	*replace_segment(t_expansion_state *state, t_gc_list *gc)
{
	char	*before;
	char	*after;
	char	*temp;

	before = ft_substr(state->str, 0, state->pos, gc);
	after = ft_substr(state->str, state->pos + state->len, ft_strlen(state->str)
			- (state->pos + state->len), gc);
	temp = ft_strjoin(before, state->replacement, gc);
	return (ft_strjoin(temp, after, gc));
}

bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	handle_special_vars(char c, t_envp *env, char **replacement,
		t_gc_list *gc)
{
	if (c == '?')
	{
		*replacement = ft_itoa(env->exit_code, gc);
		return (2);
	}
	if (c == '$')
	{
		*replacement = ft_itoa(get_pid_from_proc(), gc);
		return (2);
	}
	return (0);
}

int	handle_named_var(char *str, t_envp *env, char **replacement,
		t_gc_list *gc)
{
	char	*var;

	var = extract_variable_name(str, gc);
	*replacement = ft_getenv(env->environment, var);
	if (!*replacement)
		*replacement = ft_calloc(1, 1, gc);
	return (ft_strlen(var) + 1);
}
