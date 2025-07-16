/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:33:37 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/16 20:15:15 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	s_zero(void *s, size_t n)
{
	while (n--)
		*(unsigned char *)s++ = 0;
}

void	*ft_calloc(size_t nitems, size_t size, t_gc_list *grbg_collector)
{
	void	*ptr;

	if (nitems && size && nitems > (2147483648 / size))
		return (NULL);
	ptr = ft_malloc(nitems * size, grbg_collector);
	if (ptr == NULL)
		return (NULL);
	s_zero(ptr, nitems * size);
	return (ptr);
}
