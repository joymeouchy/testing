/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:23:04 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/14 11:32:24 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*find_the_word_path_in_envp(char **envp)
{
    if (!envp || !*envp)
        return (NULL);
    while (*envp)
    {
        if (ft_strnstr(*envp, "PATH=", 5))
            return (*envp + 5);
        envp++;
    }
    return (NULL);
}
char **get_split_path(char **envp)
{
    int i;
    char **split_path;

    split_path = ft_split(find_the_word_path_in_envp(envp), ':');
    i = 0;
    while(split_path[i])
    {
        split_path[i] = ft_strjoin(split_path[i], "/");
        i++;
    }
    return(split_path);
}

