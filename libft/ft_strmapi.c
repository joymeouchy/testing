/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 12:17:04 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/16 20:20:15 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_len(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char), t_gc_list *grbg_collector)
{
	int		size;
	int		index;
	char	*ptr;

	size = get_len(s);
	ptr = (char *)ft_malloc(sizeof(char) * (size + 1), grbg_collector);
	if (ptr == NULL)
		return (NULL);
	index = 0;
	while (index < size)
	{
		ptr[index] = (*f)(index, s[index]);
		index++;
	}
	ptr[index] = '\0';
	return (ptr);
}
