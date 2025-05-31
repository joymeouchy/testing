/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/05/31 14:15:39 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*command_line_input(void)
{
	char	*input;

	input = readline("Minishell$ ");
	if (input && *input)
		add_history(input);
	return (input);
}

void	parsing_main(t_envp *env)
{
	t_list	*list;
	t_stack	*stack;
	t_tree	*tree;

	list = input_to_list(command_line_input());
	if (!list)
	{
		write(1, "exit\n", 5);
		exit(1);
	}
	if (!list->head)
		return ;
	expand_list(list, env->environment);
	check_and_remove_quotes(list);
	tokenize(list, env);
	print_list(list);
	add_arg_to_redir(list);
	stack = shunting_yard(list);
	// print_stack(stack);
	tree = stack_to_tree(stack, env);
	// printf("tree:\n");
	// print_inorder(tree->root);
	execution(tree->root);
	if (list)
		free_list(list);
	if (stack)
		free_stack(stack);
}
