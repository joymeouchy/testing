/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:18:30 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/29 20:47:31 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	print_message_and_exit(char *message, char *word, int exit_code)
{
	// ft_putstr_fd(" ", 2);
	ft_putstr_fd(message, 2);
	ft_putendl_fd(word, 2);
	// printf("%s %s\n", message, word);
	return (exit_code);
}
