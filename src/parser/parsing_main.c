/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/08 17:13:37 by jmeouchy         ###   ########.fr       */
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

#include <errno.h>

int	check_file_executable(t_envp *env, char *file)
{
	char	*argv[] = { file, NULL };
	pid_t	pid;
	int		status;

	if (ft_strncmp(file, "./", 2) != 0)
		return (-1);
	if (access(file, X_OK) == 0)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(file, argv, env->environment);
			if (errno == ENOEXEC)
			{
				char *sh_argv[] = {"/bin/sh", file, NULL };
				execve("/bin/sh", sh_argv, env->environment);
			}
			perror(file);
			exit(126);
		}
		else if (pid > 0)
			waitpid(pid, &status, 0);
	}
	else
		return (-1);
	return (0);
}


void	parsing_main(t_envp *env, char *input)
{
	t_list	*list;
	t_stack	*stack;
	t_tree	*tree;

	list = input_to_list(input);
	if (!list)
	{
		write(1, "exit\n", 5);
		exit(1);
	}
	if (!list->head)
		return ;
	expand_list(list, env);
	tokenize(list, env); //reorganized this section needs retesting
	add_arg_to_redir(list);
	check_and_remove_quotes(list);
	tokenize(list, env); 
	// print_list(list);
	stack = shunting_yard(list);
	// print_stack(stack);
	tree = stack_to_tree(stack, env);
	// printf("tree:\n");
	// print_inorder(tree->root);
	if (tree->root->token >= WORD)
	{
		if (check_file_executable(env, tree->root->data) == -1)
			env->exit_code = print_message_and_exit(tree->root->data, ":command not found", 127);
	}
	execution(tree->root, env);
	if (list)
		free_list(list);
	if (stack)
		free_stack(stack);
}
