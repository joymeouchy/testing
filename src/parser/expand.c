/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/08/05 21:14:54 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*ft_getenv(char **envp, const char *var_name)
// {
// 	int		len;
// 	char	*equal_sign;

// 	if (!envp || !var_name)
// 		return (NULL);
// 	len = ft_strlen(var_name);
// 	while (*envp)
// 	{
// 		if (ft_strncmp(*envp, var_name, len) == 0 && (*envp)[len] == '=')
// 		{
// 			equal_sign = ft_strchr(*envp, '=');
// 			if (equal_sign)
// 				return (equal_sign + 1);
// 		}
// 		envp++;
// 	}
// 	return (NULL);
// }

// int	find_dollar(char *str)
// {
// 	int	i;
// 	int	in_single_quote = 0;
// 	int	in_double_quote = 0;

// 	if (!str || *str == '\0')
// 		return (-1);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !in_double_quote)
// 			in_single_quote = !in_single_quote;
// 		else if (str[i] == '"' && !in_single_quote)
// 			in_double_quote = !in_double_quote;
// 		if (str[i] == '$' && !in_single_quote)
// 			return (i);
// 		i++;
// 	}
// 	return (-1);
// }

// char	*extract_variable_name(char *str, t_gc_list *grbg_collector)
// {
// 	int	i = find_dollar(str) + 1;
// 	int	start;

// 	if (str[i] == '?' || str[i] == '$' || (str[i] >= '1' && str[i] <= '9'))
// 		return (ft_substr(str, i, 1, grbg_collector));

// 	start = i;
// 	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
// 		i++;
// 	return (ft_substr(str, start, i - start, grbg_collector));
// }

#include "../../includes/minishell.h"



char	*replace_variable_word(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before = ft_substr(str, 0, dollar_pos, grbg_collector);
	char	*after = ft_substr(str, dollar_pos + ft_strlen(var_name) + 1,
			ft_strlen(str), grbg_collector);
	char	*temp = ft_strjoin(before, replacement, grbg_collector);
	char	*result = ft_strjoin(temp, after, grbg_collector);
	return (result);
}

char	*replace_variable_dollar(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before = ft_substr(str, 0, dollar_pos, grbg_collector);
	char	*after = ft_substr(str, dollar_pos + ft_strlen(var_name),
			ft_strlen(str), grbg_collector);
	char	*temp = ft_strjoin(before, replacement, grbg_collector);
	char	*result = ft_strjoin(temp, after, grbg_collector);
	return (result);
}

int	get_pid_from_proc(void)
{
	int		fd;
	char	buf[256];
	int		i = 0;
	int		pid = 0;

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

// Helper: Replace a segment [dollar_pos..dollar_pos+len) with replacement in str
char *replace_segment(char *str, int dollar_pos, int len, char *replacement, t_gc_list *gc)
{
	char *before = ft_substr(str, 0, dollar_pos, gc);
	char *after = ft_substr(str, dollar_pos + len, ft_strlen(str), gc);
	char *temp = ft_strjoin(before, replacement, gc);
	return ft_strjoin(temp, after, gc);
}

// Helper: Check if char is valid for variable names
bool is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char *process_expansion(char *str, t_envp *env, t_gc_list *gc)
{
	int i = 0, dollar_pos;

	while ((dollar_pos = find_dollar(&str[i])) != -1)
	{
		dollar_pos += i;
		char c = str[dollar_pos + 1];
		char *replacement = NULL;
		int len = 0;

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
		// else if (c == 39 || c == 34)
		// {
		// 	int end = dollar_pos ;
		// 	while (str[end] && str[end] != c)
		// 		end++;
		// 	// if (str[end] != c)
		// 	// 	break; // malformed, stop expansion

		// 	// Just remove the dollar sign and keep the quote content
		// 	dollar_pos-= 2;
		// 	len = end - dollar_pos;
		// 	replacement = ft_substr(str, dollar_pos + 1, len - 1, gc); // includes the opening quote
		// 	str = replace_segment(str, dollar_pos - 1, len, replacement, gc);
		// 	i = dollar_pos + ft_strlen(replacement);
		// 	continue;
		// }
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
			i = dollar_pos + 1; // skip invalid expansion char
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

        // Only process if NOT inside any quotes
        if (!in_single_quote && !in_double_quote && 
            str[i] == '$' && (str[i+1] == '\'' || str[i+1] == '"'))
        {
            char quote = str[i+1];
            int start = i + 1; // position of quote
            int end = start + 1;

            // find closing quote
            while (str[end] && str[end] != quote)
                end++;

            if (!str[end]) 
                break; // malformed, stop or handle error

            // Remove the $ by copying the string skipping str[i]
            char *before = ft_substr(str, 0, i, gc);
            char *quoted = ft_substr(str, start, end - start + 1, gc);
            char *after = ft_substr(str, end + 1, ft_strlen(str) - (end + 1), gc);

            char *temp = ft_strjoin(before, quoted, gc);
            char *new_str = ft_strjoin(temp, after, gc);

            str = new_str;
            i += ft_strlen(quoted);
            continue; // continue without i++ because we just replaced
        }
        else
        {
            i++;
        }
    }
    return str;
}


char *expand(char *str, t_envp *env, t_gc_list *gc)
{
    if (!str || !env || !env->environment)
        return NULL;

    // Step 1: remove $ from $'...' and $"..."
    str = remove_dollar_from_quoted_strings(str, gc);

    // Step 2: do regular variable expansions
    return process_expansion(str, env, gc);
}



void	expand_list(t_list *list, t_envp *env, t_gc_list *grbg_collector)
{
	t_list_node	*current = list->head;

	while (current)
	{
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, env, grbg_collector);
			if (!current->data)
				return;
		}
		current = current->next;
	}
}
