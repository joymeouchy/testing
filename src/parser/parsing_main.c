/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 11:46:19 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/05 21:26:49 by jmeouchy         ###   ########.fr       */
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

int	is_valid_director1y(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (!dir)
	{
		return (0);
	}
	closedir(dir);
	if (access(path, X_OK) != 0)
	{
		return (0);
	}
	return (1);
}

static void	exec_script_or_binary(t_tree_node *node, char **envp, int exit_code, t_gc_list *grbg_collector)
{
	char	**argv =  fill_arguments(node, grbg_collector);
	char	*sh_argv[] = { "/bin/sh", node->data, NULL };
	if(is_valid_director1y(node->data))
	{
		ft_putstr_fd(node->data, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	execve(node->data, argv, envp);
	if (errno == ENOEXEC)
		execve("/bin/sh", sh_argv, envp);
	perror(node->data);
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

static int	check_file_executable(t_envp *env, t_tree_node *node, t_gc_list *grbg_collector)
{
	pid_t	pid;

	if (!node->data[0])
		return (1);
	if (!(node->data[0] == '/' || ft_strncmp(node->data, "./", 2) == 0 || ft_strncmp(node->data, "../", 3) == 0))
	{
		return (env->exit_code = print_message_and_exit(node->data, ":command not found", 127));
	}
	if (access(node->data, R_OK) == -1)
		return (-1);
	if (access(node->data, X_OK | R_OK) == -1)
	{
		return (env->exit_code = print_message_and_exit(node->data, ": Permission denied", 126));
	}
	pid = fork();
	if (pid == 0)
		exec_script_or_binary(node, env->environment, env->exit_code, grbg_collector);
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
	add_arg_to_redir(list);
	env->exit_code = check_and_remove_quotes(list);
	if (env->exit_code != 0)
		return ;
	check_and_remove_empty(list);
	tokenize_after_quotes(list, env);
	stack = shunting_yard(list, grbg_collector);
	if(!stack)
		return ;
	// print_stack(stack);
	tree = stack_to_tree(stack, env, grbg_collector);
	if (!tree)
		return ; //something exit code and free
	if (tree->root->token >= WORD)
	{
		if (check_file_executable(env, tree->root, grbg_collector) == -1)
			env->exit_code = print_message_and_exit(tree->root->data, ":no such file or directory", 127);
	}
	// printf("tree:\n");
	// print_inorder(tree->root);
	env->exit_code = execution(tree->root, env, grbg_collector);
}
