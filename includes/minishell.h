/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 08:12:20 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/31 22:04:17 by jmeouchy         ###   ########.fr       */
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
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# define _GNU_SOURCE

// extern volatile sig_atomic_t g_sigint;
extern int g_sigint;
///Builtins
char *get_env_value(const char *name, char **env, t_gc_list *grbg_collector);
char *handle_cd_home(t_envp *env, t_gc_list *grbg_collector);
char *handle_cd_dash(t_envp *env, t_gc_list *grbg_collector);
char *handle_cd_tilde(const char *arg, t_envp *env, t_gc_list *grbg_collector);
char *resolve_cd_target(const char *arg, t_envp *env, t_gc_list *grbg_collector);
char	*extract_key(const char *arg, t_gc_list *grbg_collector);
char	**merge_env_vars(t_envp *env, t_gc_list *grgb_collector);
int	cd(t_tree_node *root, t_envp *env, t_gc_list *grbg_collector);
int echo(t_tree_node *echo_node);
int  env_getter(t_envp *env);
int	is_numeric(const char *str);
int	safe_atoll(const char *str, long long *out);
int	count_args(t_tree_node *node);
int	count_env_vars(char **env);
int	var_exists(char *arg, char **env);
int	exit_builtin(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
int	find_key_index(char *key, int key_len, char **env);
int	replace_existing_key(char *arg, char ***env, t_gc_list *grbg_collector);
int	is_valid_key(const char *str);
int	export(t_tree_node *root, t_envp *env, t_gc_list *grbg_collector);
int	pwd(void);
int	is_valid_var_name(const char *name);
int	match_key(const char *env_entry, const char *key);
int	unset(t_tree_node *root, t_envp *env);
void	free_env(char **env);
void	copy_except_key(char **old_env, char **new_env, char *key, t_gc_list *grbg_collector);
void	remove_var_by_key(char *arg, char ***env, t_gc_list *grgb_collector);
void	add_new_var(char *arg, char ***env, t_gc_list *grgb_collector);
void	update_env(char *arg, t_envp *env, t_gc_list *grgb_collector);
void	print_key_value(char *env_var, t_gc_list *grbg_collector);
void	sort_env(char **env);
void	copy_env_vars(char **src, char **dst, int *dst_index, t_gc_list *grbg_collector);
void	remove_env_entry(char **env, int index);
void	remove_var_from_env(const char *var_name, t_envp *env);
//Execution
char	*get_path_with_command(t_tree_node *, t_gc_list *grbg_collectornode);
char	**fill_arguments(t_tree_node *node, t_gc_list *grgb_collector);
int	exec_builtin(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
int	exec_cmd(t_tree_node *node, t_envp *env, t_gc_list *grgb_collector);
int	exec_commands(t_tree_node *node, t_envp *env, t_gc_list *grgb_collector);
int	execution(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
int	count_pipes(t_tree_node *node);
int	pipe_exec(t_tree_node *node, int pipe_count, t_envp *env, t_gc_list *grbg_collector);
int     handle_recirections(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
void	heredoc(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector, int heredoc_counter);
void	read_from_pipe(t_tree_node *node, pid_t *read_pid, int pipefd[2], t_envp *env, t_gc_list *grbg_collector);
void	write_to_pipe(t_tree_node *node, pid_t *write_pid, int pipefd[2],t_envp *env, t_gc_list *grbg_collector);
void	redirect_stdin_and_exec(t_tree_node *node, char *file_name, t_envp *env, t_gc_list *grbg_collector);
int     redir_input(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
int	    redir_output(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
int	    redir_output_append(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector);
void	sig_handler(int sigtype);
void	signals(void);
//Parsing
void	add_arg_to_redir(t_list *list);
void	swap_red(t_tree_node *node1, t_tree_node *node2);
char	*ft_getenv(char **envp, const char *var_name);
void	expand_list(t_list *list, t_envp *env, t_gc_list *grbg_collector);
void	one_element_input_to_list(char *input, t_list *list, int *start, int *i, t_gc_list *grbg_collector);
void	two_element_input_to_list(char *input, t_list *list, int *start, int *i, t_gc_list *grbg_collector);
void	parsing_main(t_envp *env, char *input, t_gc_list *grbg_collector);
void	skip_quotes_in_string(int *count_quotes,
    char *string, int *i, int quotes_ascii);
void	remove_quotes_from_substring(char *string,
        char **copy_string, int *i, int quotes_ascii);
void	remove_quotes_from_string(char *string);
void tokenize_redirections(t_list_node *temp);
void	tokenize(t_list *list, t_envp *envp);
void	tokenize_after_quotes(t_list *list, t_envp *envp);
void	push_stack_to_other(t_stack *src, t_stack *dst);
int	find_dollar(char *str);
int	split_redirections(char *input, t_list *list, int start, int *i, t_gc_list *grbg_collector);
int	split_symbols(char *input, t_list *list, int start, int *i, t_gc_list *grbg_collector);
int	double_quotes_to_node(char *input, t_list *list, int start, int *i);
int	single_quotes_to_node(char *input, t_list *list, int start, int *i);
int	check_file_executable(t_envp *env, char *file);
int	check_if_quotes_balanced(char *string);
int	check_if_contains_quotes(char *string);
int	check_and_remove_quotes(t_list *list);
int	ft_strcmp(const char *s1, const char *s2);
int	check_builtin(char *data);
int	is_command(char *cmd, t_envp *envp);
char	*extract_variable_name(char *str, t_gc_list *grbg_collector);
char	*command_line_input(void);
char	*replace_variable(char *str, char *var_name,
	char *replacement, int dollar_pos, t_gc_list *grbg_collector);
char	*expand(char *str, t_envp *env, t_gc_list *grbg_collector);
char	**get_split_path(char **envp, t_gc_list *grbg_collector);
char	*append_char_to_string(char *src, char c);
bool	check_if_dollar_to_print(char *str);
t_list	*input_to_list(char *input, t_gc_list *grbg_collector);
t_stack	*malloc_and_init_stack(t_list *list, t_gc_list *grgb_collector);
t_stack	*shunting_yard(t_list *list, t_gc_list *grgb_collector);

//main
char **dup_env(char **envp, t_gc_list *grgb_collector);
void	update_shlvl(t_envp *env, t_gc_list *grgb_collector);
//syntax error
int print_message_and_exit(char *message, char *word, int exit_code);
void signal_in_child(void);
bool	is_space(char c);

void    main_signal(void);
void    ignore_signals(void);
void    restore_signals(void);
void    restore_signals_heredoc(void);
int     is_valid_directory(const char *path);
#endif