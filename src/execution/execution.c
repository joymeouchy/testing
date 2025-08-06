/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:59 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/06 22:52:57 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_commands(t_tree_node *node, t_envp *env, t_gc_list *grgb_collector)
{
	if (node->token == BUILT_IN)
		return (exec_builtin(node, env, grgb_collector));
	if (node->token == COMMAND)
		return (exec_cmd(node, env, grgb_collector));
	return (print_message_and_exit(node->data, " : command not found", 127));
}

int	count_pipes(t_tree_node *node)
{
	int	count;
	int	left_count;
	int	right_count;
	int	total;

	if (!node)
		return (0);
	count = 0;
	if (node->token == PIPE)
		count = 1;
	left_count = count_pipes(node->left);
	right_count = count_pipes(node->right);
	total = count + left_count + right_count;
	return (total);
}

int	execution(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector)
{
	int	pipe_count;

	if (!node)
		return (-1);
	pipe_count = count_pipes(node);
	if (node->token == PIPE)
		return (pipe_exec(node, pipe_count, env, grbg_collector));
	if (node->token == COMMAND || node->token == BUILT_IN)
		return (exec_commands(node, env, grbg_collector));
	if (node->token >= 3 && node->token <= 6)
		return (handle_recirections(node, env, grbg_collector));
	return (env->exit_code);
}
