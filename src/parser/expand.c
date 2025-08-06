/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/08/06 17:13:27 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_variable_word(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*result;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos + ft_strlen(var_name) + 1,
			ft_strlen(str), grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	return (result);
}

char	*replace_variable_dollar(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*result;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos + ft_strlen(var_name),
			ft_strlen(str), grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	return (result);
}

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
	while (buf[i] >= '0' && buf[i] <= '9')
	{
		pid = pid * 10 + (buf[i] - '0');
		i++;
	}
	return (pid);
}

char	*replace_segment(char *str, int dollar_pos, int len, char *replacement, t_gc_list *gc)
{
	char	*before;
	char	*after;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, gc);
	after = ft_substr(str, dollar_pos + len, ft_strlen(str), gc);
	temp = ft_strjoin(before, replacement, gc);
	return (ft_strjoin(temp, after, gc));
}

bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*process_expansion(char *str, t_envp *env, t_gc_list *gc)
{
	int		i;
	int		dollar_pos;
	int		len;
	char	c;
	char	*replacement;

	i = 0;
	while ((dollar_pos = find_dollar(&str[i])) != -1)
	{
		dollar_pos += i;
		c = str[dollar_pos + 1];
		replacement = NULL;
		len = 0;
		if (c == '?')
		{
			replacement = ft_itoa(env->exit_code, gc);
			len = 2;
		}
		else if (c == '$')
		{
			replacement = ft_itoa(get_pid_from_proc(), gc);
			len = 2;
		}
		else if (is_valid_var_char(c))
		{
			char *var = extract_variable_name(&str[dollar_pos], gc);
			replacement = ft_getenv(env->environment, var);
			if (!replacement)
				replacement = ft_calloc(1, 1, gc);
			len = ft_strlen(var) + 1;
		}
		else
		{
			i = dollar_pos + 1;
			continue;
		}
		str = replace_segment(str, dollar_pos, len, replacement, gc);
		i = dollar_pos + ft_strlen(replacement);
	}
	return str;
}

char *remove_dollar_from_quoted_strings(char *str, t_gc_list *gc)
{
	int i = 0;
	int in_single_quote = 0;
	int in_double_quote = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (!in_single_quote && !in_double_quote
			&& str[i] == '$' && (str[i+1] == '\'' || str[i+1] == '"'))
		{
			char quote = str[i+1];
			int start = i + 1;
			int end = start + 1;
			while (str[end] && str[end] != quote)
				end++;
			if (!str[end])
				break ;
			char *before = ft_substr(str, 0, i, gc);
			char *quoted = ft_substr(str, start, end - start + 1, gc);
			char *after = ft_substr(str, end + 1, ft_strlen(str) - (end + 1), gc);
			char *temp = ft_strjoin(before, quoted, gc);
			char *new_str = ft_strjoin(temp, after, gc);
			str = new_str;
			i += ft_strlen(quoted);
			continue ;
		}
		else
			i++;
	}
	return (str);
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
		}
		current = current->next;
	}
}
