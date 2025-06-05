/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:18:30 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/05 21:23:31 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int print_message_and_exit(char *message, char *word, int exit_code)
{
    printf("%s %s\n", message, word);
    return (exit_code);
}
