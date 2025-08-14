/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/14 17:33:52 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_initial_state(t_envp *env)
{
	if (g_sigint == 130)
	{
		env->exit_code = 130;
		g_sigint = 0;
		// return (1);
	}
	// return (0);
}

static t_list	*prepare_token_list(char *input, t_envp *env,
		t_gc_list *grbg_collector)
{
	t_list	*list;

	list = input_to_list(input, grbg_collector);
	if (!list || !list->head)
		return (NULL);
	expand_list(list, env, grbg_collector);
	tokenize(list, env);
	add_arg_to_redir(list);
	env->exit_code = check_and_remove_quotes(list);
	if (env->exit_code != 0)
		return (NULL);
	tokenize_after_quotes(list, env);
	return (list);
}

static t_tree	*build_tree_from_list(t_list *list, t_envp *env,
		t_gc_list *grbg_collector)
{
	t_stack	*stack;
	t_tree	*tree;

	stack = shunting_yard(list, grbg_collector);
	if (!stack)
		return (NULL);
	tree = stack_to_tree(stack, env, grbg_collector);
	return (tree);
}

static int	handle_execution_check(t_tree_node *root, t_envp *env,
		t_gc_list *grbg_collector)
{
	if (root->token >= WORD)
	{
		if (check_file_executable(env, root, grbg_collector) == -1)
		{
			env->exit_code = print_message_and_exit(root->data,
					": No such file or directory", 127);
			return (-1);
		}
	}
	return (0);
}

void	parsing_main(t_envp *env, char *input, t_gc_list *grbg_collector)
{
	t_list	*list;
	t_tree	*tree;

	env->syntax_error = false;
	handle_initial_state(env);

	// if (handle_initial_state(env))
	// 	return ;
	list = prepare_token_list(input, env, grbg_collector);
	if (!list)
	{
		free(list);//recheck if this is necessary
		return ;
	}
	// print_list(list);
	tree = build_tree_from_list(list, env, grbg_collector);
	if (!tree)
		return ;
	if (handle_execution_check(tree->root, env, grbg_collector) == -1)
		return ;
	check_syntax_errors(tree->root, env);
	if (env->syntax_error)
		return ;
	env->exit_code = execution(tree->root, env, grbg_collector);
}
