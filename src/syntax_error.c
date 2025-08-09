/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:18:30 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/09 19:03:38 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	print_message_and_exit(char *message, char *word, int exit_code)
{
	ft_putstr_fd(message, 2);
	ft_putendl_fd(word, 2);
	return (exit_code);
}

int print_syntax_error(char *message, char *word, int exit_code, t_envp *env)
{
	env->syntax_error = true;
	return (print_message_and_exit(message, word, exit_code));
}

int check_pipe(t_tree_node *node, t_envp *env)
{
	if (!node->right || (!node->left && !node->redir_arg))
		return (env->exit_code = print_syntax_error(
				"minishell: syntax error near unexpected token `|'",
				"", 2, env));
	return (0);
}
int check_redir_err(t_tree_node *node, t_envp *env)
{
	if (node->right && !(node->right->token >= 1 || node->right->token <= 6))
		return (env->exit_code = print_syntax_error("minishell: ",
				"syntax error near unexpected token `newline'", 2, env));
	else if (node->redir_arg == NULL)
		return (env->exit_code = print_syntax_error("minishell: ",
				"syntax error near unexpected token `newline'", 2, env));
	return (0);
}

int	check_syntax_errors(t_tree_node *node, t_envp *env)
{
	if (node == NULL)
		return (env->exit_code);
	if (node->token == PIPE)
		env->exit_code = check_pipe(node, env);
	if (is_redirection(node->token))
		env->exit_code = check_redir_err(node, env);
	check_syntax_errors(node->left, env);
	check_syntax_errors(node->right, env);
	return (env->exit_code);
}
