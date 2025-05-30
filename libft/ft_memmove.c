/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 15:24:45 by jmeouchy          #+#    #+#             */
/*   Updated: 2024/06/17 15:56:52 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*tdst;
	char		*tsrc;

	tdst = (char *)dst;
	tsrc = (char *)src;
	i = 0;
	if (tdst < tsrc)
	{
		while (i < n)
		{
			tdst[i] = tsrc[i];
			i++;
		}
	}
	else if (tsrc < tdst)
	{
		i = n;
		while (i > 0)
		{
			tdst[i - 1] = tsrc[i - 1];
			i--;
		}
	}
	return (dst);
}
