/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/06/27 20:34:11 by jmeouchy         ###   ########.fr       */
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

static void	exec_script_or_binary(char *file, char **envp, int exit_code)
{
	char	*argv[] = { file, NULL };
	char	*sh_argv[] = { "/bin/sh", file, NULL };

	execve(file, argv, envp);
	if (errno == ENOEXEC)
		execve("/bin/sh", sh_argv, envp);
	perror(file);
	exit(exit_code);
}

static int	wait_for_child(pid_t pid, t_envp *env)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	return (0);
}

int	check_file_executable(t_envp *env, char *file)
{
	pid_t	pid;

	if (!(file[0] == '/' || ft_strncmp(file, "./", 2) == 0 || ft_strncmp(file, "../", 3) == 0))
		return (-1);

	if (access(file, X_OK) != 0)
		return (-1);

	pid = fork();
	if (pid == 0)
		exec_script_or_binary(file, env->environment, env->exit_code);
	else if (pid > 0)
		return (wait_for_child(pid, env));
	return (-1);
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
	tokenize(list, env);
	add_arg_to_redir(list);
	check_and_remove_quotes(list);
	// print_list(list);
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
	env->exit_code = execution(tree->root, env);
	if (list)
		free_list(list);
	if (stack)
		free_stack(stack);
}
