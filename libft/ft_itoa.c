/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 10:30:35 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/16 20:15:37 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		n *= -1;
		n = 147483648;
		count += 2;
	}
	if (n < 0)
		count++;
	if (n == 0)
		count++;
	while (n != 0)
	{
		count ++;
		n /= 10;
	}
	return (count);
}

static int	is_neg(int n, char *ptr)
{
	if (n == -2147483648)
	{
		ptr[0] = '-';
		ptr[1] = '2';
		n = 147483648;
	}
	if (n < 0)
	{
		n *= -1;
		ptr[0] = '-';
	}
	return (n);
}

char	*ft_itoa(int n, t_gc_list *grbg_collector)
{
	int		size;
	char	*ptr;

	size = count_digits(n);
	ptr = (char *)ft_malloc(sizeof(char) * (size + 1), grbg_collector);
	if (ptr == NULL)
		return (NULL);
	n = is_neg(n, ptr);
	ptr[size] = '\0';
	if (n == 0)
		ptr[0] = '0';
	while (n > 0)
	{
		size--;
		ptr[size] = (n % 10) + '0';
		n /= 10;
	}
	return (ptr);
}
