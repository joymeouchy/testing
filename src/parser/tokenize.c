/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:23:04 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/09 14:20:22 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tokenize_redirections(t_list_node *temp)
{
	if (temp->data[0] == '<' && temp->data[1] == '<')
		temp->token = LEFT_D_REDIRECTION;
	else if (temp->data[0] == '>' && temp->data[1] == '>')
		temp->token = RIGHT_D_REDIRECTION;
	else if (temp->data[0] == '<')
		temp->token = LEFT_REDIRECTION;
	else if (temp->data[0] == '>')
		temp->token = RIGHT_REDIRECTION;
}

static void	assign_token_type(t_list_node *node, t_envp *envp,
		int *flag_command)
{
	if (!node || node->data == NULL)
		return ;
	if (node->data[0] == '|')
	{
		node->token = PIPE;
		*flag_command = false;
	}
	else if (node->data[0] == '<' || node->data[0] == '>')
		tokenize_redirections(node);
	else if (node->data[0] == '~')
		node->token = TILDE;
	else if (!(*flag_command) && check_builtin(node->data))
	{
		node->token = BUILT_IN;
		*flag_command = true;
	}
	else if (!(*flag_command) && ft_strcmp(node->data, "")
		&& is_command(node->data, envp))
	{
		node->token = COMMAND;
		*flag_command = true;
	}
	else
		node->token = WORD;
}

void	tokenize(t_list *list, t_envp *envp)
{
	t_list_node	*temp;
	int			flag_command;

	flag_command = false;
	temp = list->head;
	while (temp)
	{
		assign_token_type(temp, envp, &flag_command);
		temp = temp->next;
	}
}

void	flag_after_pipe(t_list_node *node, int *flag_command)
{
	if (node->token == PIPE)
	{
		if (node->redir_arg)
			*flag_command = true;
		else
			*flag_command = false;
	}
}

void	tokenize_after_quotes(t_list *list, t_envp *envp)
{
	t_list_node	*temp;
	int			flag_command;

	flag_command = false;
	temp = list->head;
	while (temp)
	{
		if (!temp || temp->data == NULL)
			return ;
		flag_after_pipe(temp, &flag_command);
		if (!flag_command && check_builtin(temp->data))
		{
			temp->token = BUILT_IN;
			flag_command = true;
		}
		else if (!flag_command && ft_strcmp(temp->data, "")
			&& is_command(temp->data, envp))
		{
			temp->token = COMMAND;
			flag_command = true;
		}
		temp = temp->next;
	}
}
