/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 08:12:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/06/14 14:57:13 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "structures.h"
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
# define _GNU_SOURCE

///parser.c///

void    one_element_input_to_list(char *input, t_list *list, int *start, int *i);
void    two_element_input_to_list(char *input, t_list *list, int *start, int *i);
int split_redirections(char *input, t_list *list, int start, int *i);
int split_symbols(char *input, t_list *list, int start, int *i);
int double_quotes_to_node(char *input, t_list *list, int start, int *i);
int single_quotes_to_node(char *input, t_list *list, int start, int *i);
t_list  *input_to_list(char *input);
int pwd(void);
int	is_valid_key(const char *str);
void	update_env(char *arg, t_envp *env);
int	cd(t_tree_node *root, t_envp *env);
int	exit_builtin(t_tree_node *node, t_envp *env);
///remove_quotes.c///

void	skip_quotes_in_string(int *count_quotes, char *string, int *i, int quotes_ascii);
int     check_if_quotes_balanced(char *string);
int     check_if_contains_quotes(char *string);
void	remove_quotes_from_substring(char *string, char **copy_string, int *i, int quotes_ascii);
void	remove_quotes_from_string(char *string);
int     check_and_remove_quotes(t_list *list);

///expand.c///
void	expand_list(t_list *list, t_envp *env);
char	*expand(char *str, char **envp);

///utils.c///
char	*append_char_to_string(char *src, char c);

///shunting_yard.c//
t_stack *shunting_yard(t_list *list);


////tokenization.c//
void	tokenize(t_list *list, t_envp *envp);
int ft_strcmp(const char *s1, const char *s2);
//get_env.c
char **get_split_path(char **envp);
int  env_getter(t_envp *env);

///main.c///
char	*command_line_input(void);
t_tree	*stack_to_tree(t_stack *stack, t_envp *environment);
void	print_inorder(t_tree_node *node);
int echo(t_tree_node *echo_node);
void myhandler(int sigtype);
int export(t_tree_node *root, t_envp *env);
int	unset(t_tree_node *root, t_envp *env);
void signals();
void	parsing_main(t_envp *env, char *input);

//exec
int execution(t_tree_node *node, t_envp *env);
int exec_commands(t_tree_node *node, t_envp *env);
void add_arg_to_redir(t_list *list);
void swap_red(t_tree_node *node1, t_tree_node *node2);
// void pipe_exec(t_tree_node *node);
int pipe_exec(t_tree_node *node, int pipe_count, t_envp *env);
// static char *get_env_value(const char *key, t_envp *env);
void pipe_exec_for_multiple(t_tree_node *node);
void pipe_exec_for_single(t_tree_node *node);
int count_pipes(t_tree_node *node);
int handle_recirections(t_tree_node *node, t_envp *env);
void	heredoc(t_tree_node *node, t_envp *env);
void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env);

int print_message_and_exit(char *message, char *word, int exit_code);
char	*command_line_input(void);
void	free_env(char **env);
int	count_env_vars(char **env);
int	var_exists(char *arg, char **env);
char	*extract_key(const char *arg);
void	copy_except_key(char **old_env, char **new_env, char *key);
void	remove_var_by_key(char *arg, char ***env);
int	find_key_index(char *key, int key_len, char **env);
int	replace_existing_key(char *arg, char ***env);
void	add_new_var(char *arg, char ***env);
int	is_valid_key(const char *str);
void	update_env(char *arg, t_envp *env);
void	print_key_value(char *env_var);
void	sort_env(char **env);
void	copy_env_vars(char **src, char **dst, int *dst_index);
char	**merge_env_vars(t_envp *env);
char *get_env_value(const char *name, char **env);
char *handle_cd_home(t_envp *env);
char *handle_cd_dash(t_envp *env);
char *handle_cd_tilde(const char *arg, t_envp *env);
char *resolve_cd_target(const char *arg, t_envp *env);

#endif