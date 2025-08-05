/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:33:52 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/05 20:08:17 by jmeouchy         ###   ########.fr       */
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

char *expanded_section(char *data, t_envp *env, t_gc_list *grbg_collector)
{
    if (!data)
                return (NULL);
    if (data[1] == '$')
    {
        return (ft_itoa(get_pid_from_proc(), grbg_collector));
    }
    else if (data[1] == '?')
        return (ft_itoa(env->exit_code, grbg_collector));
    // else if (data[1] == 34) //double quotes
    //     //dquotes
    // else if (data[1] == 39) //single quotes
    //     //squotes
    else
        return (ft_getenv(env->environment, &(data[1])));
        //expand character and replace with env value if exists
    return (data);
}


// "$hi"$hola$$'$'somethingidk

// 1st
// so i guess if in double quotes rawa2
// removes "$hi"
// $$'$'somethingidk

// 2nd iteration:
//  removes $hola
// "$hi"$$'$'somethingidk
// 3rd iteration
// $$->pid
// "$hi"PID'$'somethingidk

// 4th itertion:
// "$hi"PID$somethingidk


char *expand(char *data, t_envp *env, t_gc_list *grbg_collector)
{
    int dollar_pos;
    int end_of_$substr;
    char *before;
    char *after;
    char *expanded;
    (void)env;

    if (ft_strcmp("$", data) == 0)
        return (data);
    dollar_pos = find_dollar(data);
    if (data[dollar_pos +  1] == '$')
        end_of_$substr = dollar_pos +  2;
    else
    {
        end_of_$substr = dollar_pos + 1;
        while(data[dollar_pos] != '$')
        {
            dollar_pos++;
        }
    }
    
    printf("dollar_pos is:%d\n", dollar_pos);
    printf("string lenght is: %ld\n", ft_strlen(data));
    printf("end od $substr: %d\n", end_of_$substr);
    // while (dollar_pos != -1)
    // {
        before = ft_substr(data, 0, dollar_pos, grbg_collector);
        printf("before is %s\n", before);
        expanded = expanded_section(ft_substr(data, dollar_pos, end_of_$substr - dollar_pos, grbg_collector), env, grbg_collector);
        printf("expanded is %s\n", expanded);
        after = ft_substr (data, end_of_$substr, ft_strlen(data) - end_of_$substr - ft_strlen(before), grbg_collector);
        printf("after is %s\n", after);
        dollar_pos = find_dollar(data);
    // }
    // return (data);
// return (expanded_section(data, dollar_pos, grbg_collector));
}


// while (dollar position != -1)
// {
//     before = str -> dollar pos;
//     expanded = substr (dollar pos->next dollar pos or if quotes open la ysakro whichever beyeje abl);
//     after = substr (new dollar or end or whatever ->end of string);
    
// }