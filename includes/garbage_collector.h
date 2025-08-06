/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:35:06 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 17:22:34 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc_list
{
	t_gc_node	*head;
}	t_gc_list;

void		*ft_malloc(int size, t_gc_list *grbg_collector);
void		ft_free_gc(t_gc_list *grbg_collector);
t_gc_list	*init_grbg_collector(void);

#endif