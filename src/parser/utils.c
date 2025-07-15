/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 08:03:35 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/15 20:16:52 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_char_to_string(char *src, char c)
{
	int		len;
	int		i;
	char	*new_str;

	len = ft_strlen(src) + 2;
	new_str = malloc(len * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (src[i])
	{
		new_str[i] = src[i];
		i++;
	}
	new_str[i] = c;
	i++;
	new_str[i] = '\0';
	free(src);
	return (new_str);
}
