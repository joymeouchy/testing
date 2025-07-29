/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkhoury <lkhoury@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/07/29 20:57:17 by lkhoury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*command_line_input(void)
{
	char	*input;

	input = readline("Minishell$ ");
	if(!input)
		exit(0);
	if (input && *input)
		add_history(input);
	return (input);
}

static void	exec_script_or_binary(char *file, char **envp, int exit_code)
{
	char	*argv[] = { file, NULL };
	char	*sh_argv[] = { "/bin/sh", file, NULL };
	// printf("%s, file\n",file);
	// if (!(ft_strncmp(file, "./", 2) == 0))
	// {
		if(is_valid_directory(file)) //fix
		{
			ft_putstr_fd(file, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
	// }
	// if (access(file, X_OK) == 0)
	// {
	// 	write(1, "is a dir\n", 10);
	// 	return ;
	// }
	execve(file, argv, envp);
	if (errno == ENOEXEC)
		execve("/bin/sh", sh_argv, envp);
	perror(file);
	exit(exit_code);
}

static int	wait_for_child(pid_t pid, t_envp *env)
{
	int	status;
	ignore_signals();
	waitpid(pid, &status, 0);
	main_signal();
	if (WIFEXITED(status))
		env->exit_code = WEXITSTATUS(status);
	return (0);
}

int	check_file_executable(t_envp *env, char *file)
{
	pid_t	pid;

	if (!file[0])
		return (1);
	if (!(file[0] == '/' || ft_strncmp(file, "./", 2) == 0 || ft_strncmp(file, "../", 3) == 0))
	{
		return (env->exit_code = print_message_and_exit(file, ":command not found", 127));
	}
	if (access(file, X_OK | R_OK) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
		exec_script_or_binary(file, env->environment, env->exit_code);
	else if (pid > 0)
		return (wait_for_child(pid, env));
	return (-1);
}

void	parsing_main(t_envp *env, char *input, t_gc_list *grbg_collector)
{
	t_list	*list;
	t_stack	*stack;
	t_tree	*tree;
	// (void)env;
	if(g_sigint == 130)
	{
		env->exit_code = 130;
		g_sigint = 0;
	}
	list = input_to_list(input, grbg_collector);
	if (!list || !list->head)
	{
		return ;
	}
	expand_list(list, env, grbg_collector);
	tokenize(list, env);
	// print_list(list);
	check_and_remove_quotes(list);
	add_arg_to_redir(list);
	check_and_remove_empty(list);
	// tokenize(list, env); 
	// print_list(list);
	stack = shunting_yard(list, grbg_collector);
	if(!stack)
		return ;
	// print_stack(stack);
	tree = stack_to_tree(stack, env, grbg_collector);
	if (!tree)
		return ; //something exit code and free
	// printf("\ntree:\n");
	// print_inorder(tree->root);
	if (tree->root->token >= WORD)
	{
		if (check_file_executable(env, tree->root->data) == -1)
			env->exit_code = print_message_and_exit(tree->root->data, ":no such file or directory", 127);
	}
	env->exit_code = execution(tree->root, env, grbg_collector);
}
