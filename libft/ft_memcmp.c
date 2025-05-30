/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:22:06 by jmeouchy          #+#    #+#             */
/*   Updated: 2024/06/14 17:10:19 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	char		*s1;
	char		*s2;

	s1 = (char *)str1;
	s2 = (char *)str2;
	if (str1 == NULL || str2 == NULL)
		return (0);
	if (ft_strlen(s1) > ft_strlen(s2) && ft_strlen(s1) <= n
		&& ft_strlen(s2) <= n)
		return (1);
	else if (ft_strlen(s1) < ft_strlen(s2) && ft_strlen(s1) <= n
		&& ft_strlen(s2) <= n)
		return (-1);
	while (n > 0)
	{
		if (*s1 - *s2 != 0)
			return (*s1 - *s2);
		s1++;
		s2++;
		n--;
	}
	return (0);
}
