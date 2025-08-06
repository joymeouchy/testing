/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:18:30 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 17:17:52 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	print_message_and_exit(char *message, char *word, int exit_code)
{
	ft_putstr_fd(message, 2);
	ft_putendl_fd(word, 2);
	return (exit_code);
}
