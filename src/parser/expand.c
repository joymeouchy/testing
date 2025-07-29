/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:47:40 by root              #+#    #+#             */
/*   Updated: 2025/07/29 21:17:55 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_variable_word(char *str, char *var_name,
	char *replacement, int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos
			+ ft_strlen(var_name) + 1, ft_strlen(str), grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	// free(before);
	// free(after);
	// free(temp);
	return (result);
}char	*replace_variable_dollar(char *str, char *var_name,
	char *replacement, int dollar_pos, t_gc_list *grbg_collector)
{
	char	*before;
	char	*after;
	char	*result;
	char	*temp;

	before = ft_substr(str, 0, dollar_pos, grbg_collector);
	after = ft_substr(str, dollar_pos
			+ ft_strlen(var_name), ft_strlen(str), grbg_collector);
	temp = ft_strjoin(before, replacement, grbg_collector);
	result = ft_strjoin(temp, after, grbg_collector);
	// free(before);
	// free(after);
	// free(temp);
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

static char	*process_expansion(char *str, t_envp *env, t_gc_list *grbg_collector)
{
	int		dollar_pos;
	char	*var_name;
	char	*replacement;
	char	*expanded;

	dollar_pos = find_dollar(str);
	while (dollar_pos != -1)
	{
		if(str[dollar_pos + 1] == '?')
		{
			var_name = "$?";
			replacement = ft_itoa(env->exit_code, grbg_collector);
			expanded = replace_variable_dollar(str, var_name, replacement, dollar_pos, grbg_collector);
			str = expanded;
		}
		else if(str[dollar_pos + 1] == '$')
		{
			var_name = "$$";
			replacement = ft_itoa(get_pid_from_proc(), grbg_collector);
			expanded = replace_variable_dollar(str, var_name, replacement, dollar_pos, grbg_collector);
			str = expanded;
		}
		else
		{
			if (!(ft_isalpha(str[dollar_pos + 1]) ||
				str[dollar_pos + 1] == '_' ||
				(str[dollar_pos + 1] >= '0' && str[dollar_pos + 1] <= '9')))
			{
				dollar_pos = find_dollar(&str[dollar_pos + 1]);
				continue;
			}
			var_name = extract_variable_name(&str[dollar_pos], grbg_collector);
			if (!var_name)
				return (NULL);
			replacement = ft_getenv(env->environment, var_name);
			if (!replacement)
				replacement = ft_calloc(1, 1, grbg_collector);
		expanded = replace_variable_word(str, var_name, replacement, dollar_pos, grbg_collector);
		str = expanded;
		}
		dollar_pos = find_dollar(str);
	}
	return (str);
}

char	*expand(char *str, t_envp *env, t_gc_list *grbg_collector)
{
	char	*result;

	if (!env || !env->environment)
		return (NULL);
	if (check_if_dollar_to_print(str))
		return (str);
	result = process_expansion(str, env, grbg_collector);
	return (result);
}

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

