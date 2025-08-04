/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:33:52 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/04 21:09:27 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char *expanded_section(char *data, int dollar_pos, t_gc_list *grbg_collector)
{
    if (data[dollar_pos + 1] == '$')
    {
        return (ft_itoa(get_pid_from_proc(), grbg_collector));
    }
    else if (data[dollar_pos + 1] == '?')
        //exit_code
    else if (data[dollar_pos + 1] == 34) //double quotes
        //dquotes
    else if (data[dollar_pos + 1] == 39) //single quotes
        //squotes
    else
        //expand character and replace with env value if exists
    return (data);
}


char *expand(char *data, t_envp *env, t_gc_list *grbg_collector)
{
    // int i = 0;
    int dollar_pos;

    if (ft_strcmp("$", data) == 0)
        return (data);
    dollar_pos = ft_strchr(data, '$') - data;
    (void)env;
    return (expanded_section(data, dollar_pos, grbg_collector));
}