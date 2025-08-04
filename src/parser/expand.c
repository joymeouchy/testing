/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/08/04 20:50:32 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
char	*replace_variable_word(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos + ft_strlen(var_name) + 1, ft_strlen(str),
			grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	return (result);
}

char	*replace_variable_dollar(char *str, char *var_name, char *replacement,
		int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos + ft_strlen(var_name), ft_strlen(str),
			grbg_collector);
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

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	i = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (i <= 0)
		return (-1);
	buf[i] = '\0';
	i = 0;
	pid = 0;
	while (buf[i] >= '0' && buf[i] <= '9')
	{
		pid = pid * 10 + (buf[i] - '0');
		i++;
	}
	return (pid);
}

static char *process_expansion(char *str, t_envp *env, t_gc_list *grbg_collector)
{
    int     i = 0;
    int     dollar_pos;
    char    *var_name;
    char    *replacement;
    char    *expanded;
    while ((dollar_pos = find_dollar(&str[i])) != -1)
    {
        dollar_pos += i;
        if (!str[dollar_pos + 1])
            return (str);
        if (str[dollar_pos + 1] == '?')
        {
            var_name = "$?";
            replacement = ft_itoa(env->exit_code, grbg_collector);
            expanded = replace_variable_dollar(str, var_name, replacement,
                    dollar_pos, grbg_collector);
            str = expanded;
            i = dollar_pos + ft_strlen(replacement);
        }
        else if (str[dollar_pos + 1] == '$')
        {
            var_name = "$$";
            replacement = ft_itoa(get_pid_from_proc(), grbg_collector);
            expanded = replace_variable_dollar(str, var_name, replacement,
                    dollar_pos, grbg_collector);
            str = expanded;
            i = dollar_pos + ft_strlen(replacement);
        }
		else if (str[dollar_pos +1] == 39)
		{
			int		end = dollar_pos + 2;
			char	*replacement;
			char	*var_name;
			while (str[end] && str[end] != '\'')
				end++;
			if (str[end] != '\'')
				return (str); // malformed $'...'
			replacement = ft_substr(str, dollar_pos + 2, end - (dollar_pos + 2), grbg_collector);
			var_name = ft_substr(str, dollar_pos, end - dollar_pos + 1, grbg_collector); // $'test'
			return (replace_variable_dollar(str, var_name, replacement, dollar_pos, grbg_collector));
		}
		else if (str[dollar_pos + 1] == '"') // ASCII 34
		{
			int		end = dollar_pos + 2;
			char	*replacement;
			char	*var_name;

			// Find closing double quote
			while (str[end] && str[end] != '"')
				end++;
			if (str[end] != '"')
				return (str); // malformed $"..."
			// Extract content inside the quotes (excluding $ and quotes)
			replacement = ft_substr(str, dollar_pos + 2, end - (dollar_pos + 2), grbg_collector);
			// Extract full $"..."
			var_name = ft_substr(str, dollar_pos, end - dollar_pos + 1, grbg_collector);
			// Replace it in the original string
			return (replace_variable_dollar(str, var_name, replacement, dollar_pos, grbg_collector));
		}
        else
        {
            if (!(ft_isalpha(str[dollar_pos + 1]) || str[dollar_pos + 1] == '_' ||
                (str[dollar_pos + 1] >= '0' && str[dollar_pos + 1] <= '9')))
            {
                i = dollar_pos + 1;
                continue;
            }
            var_name = extract_variable_name(&str[dollar_pos], grbg_collector);
            if (!var_name)
                return (NULL);
            replacement = ft_getenv(env->environment, var_name);
            if (!replacement)
                replacement = ft_calloc(1, 1, grbg_collector);
            expanded = replace_variable_word(str, var_name, replacement,
                    dollar_pos, grbg_collector);
            str = expanded;
            i = dollar_pos + ft_strlen(replacement);
        }
    }
    return (str);
}

// static int	is_valid_var_start(char c)
// {
// 	return (ft_isalpha(c) || c == '_' || (c >= '0' && c <= '9'));
// }

// static char	*expand_exit_code(char *str, int pos, t_envp *env, t_gc_list *grbg_collector)
// {
// 	char *replacement = ft_itoa(env->exit_code, grbg_collector);
// 	return (replace_variable_dollar(str, "$?", replacement, pos, grbg_collector));
// }

// static char	*expand_pid(char *str, int pos, t_gc_list *grbg_collector)
// {
// 	char *replacement = ft_itoa(get_pid_from_proc(), grbg_collector);
// 	return (replace_variable_dollar(str, "$$", replacement, pos, grbg_collector));
// }

// static char	*expand_env_variable(char *str, int pos, t_envp *env, t_gc_list *grbg_collector)
// {
// 	char	*var_name = extract_variable_name(&str[pos], grbg_collector);
// 	char	*replacement;

// 	if (!var_name)
// 		return (NULL);
// 	replacement = ft_getenv(env->environment, var_name);
// 	if (!replacement)
// 		replacement = ft_calloc(1, 1, grbg_collector); // empty string if var not found
// 	return (replace_variable_word(str, var_name, replacement, pos, grbg_collector));
// }

// static char	*expand_dollar_single_quote(char *str, int pos, t_gc_list *grbg_collector)
// {
// 	int		end = pos + 2;
// 	char	*replacement;
// 	char	*var_name;
// 	while (str[end] && str[end] != '\'')
// 		end++;
// 	if (str[end] != '\'')
// 		return (str); // malformed $'...'
// 	replacement = ft_substr(str, pos + 2, end - (pos + 2), grbg_collector);
// 	var_name = ft_substr(str, pos, end - pos + 1, grbg_collector); // $'test'
// 	printf("varname is %s\n", var_name);
// 	return (replace_variable_dollar(str, var_name, replacement, pos, grbg_collector));
// }

// static char	*handle_dollar_type(char *str, int pos, t_envp *env, t_gc_list *grbg_collector)
// {
// 	if (str[pos + 1] == '?')
// 		return (expand_exit_code(str, pos, env, grbg_collector));
// 	else if (str[pos + 1] == '$')
// 		return (expand_pid(str, pos, grbg_collector));
// 	else if (str[pos + 1] == '\'')
// 		return (expand_dollar_single_quote(str, pos, grbg_collector));
// 	else if (is_valid_var_start(str[pos + 1]))
// 		return (expand_env_variable(str, pos, env, grbg_collector));
// 	return (str);
// }

// static char	*process_expansion(char *str, t_envp *env, t_gc_list *grbg_collector)
// {
// 	int		i = 0;
// 	int		dollar_pos;
// 	char	*expanded;

// 	while ((dollar_pos = find_dollar(&str[i])) > -1)
// 	{
// 		dollar_pos += i;

// 		if (!str[dollar_pos + 1])
// 			return (str);
// 		expanded = handle_dollar_type(str, dollar_pos, env, grbg_collector);
// 		if (!expanded)
// 			return (NULL);
// 		str = expanded;
// 		i = dollar_pos;
// 	}
// 	return (str);
// }


char	*expand(char *str, t_envp *env, t_gc_list *grbg_collector)
{
	char	*result;

	if (!env || !env->environment)
		return (NULL);
	if (check_if_dollar_to_print(str))
	{
		printf("wsshy\n");
		return (str);
	}
	result = process_expansion(str, env, grbg_collector);
	printf("result is %s\n", result);
	return (result);
}
*/
void	expand_list(t_list *list, t_envp *env, t_gc_list *grbg_collector)
{
	t_list_node	*current;

	current = list->head;
	while (current)
	{
		if (ft_strcmp(current->data, "$?") == 0)
		{
			current->data = ft_itoa(env->exit_code, grbg_collector);
		}
		if (current->data && ft_strchr(current->data, '$'))
		{
			current->data = expand(current->data, env, grbg_collector);
			if (!current->data)
				return ;
		}
		current = current->next;
	}
}
