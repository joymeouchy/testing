/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:18:12 by jmeouchy          #+#    #+#             */
/*   Updated: 2024/06/14 16:10:07 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_set(char c, const char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str) - 1;
	while (str[i] != '\0' && i <= len)
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		i;
	char	*str;

	end = ft_strlen(s1) - 1;
	start = 0;
	while (ft_is_set(s1[start], set) && s1[start])
		start++;
	while (ft_is_set(s1[end], set) && end > start)
		end--;
	end = end - start + 1;
	str = (char *)malloc(end + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < end)
		str[i++] = s1[start++];
	str[i] = '\0';
	return (str);
}
