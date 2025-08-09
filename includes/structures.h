/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 08:16:36 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/08/09 19:11:27 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include "includes/garbage_collector.h"
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum s_tokens
{
	null = -1,
	PIPE = 0,
	COMMAND = 1,
	BUILT_IN = 2,
	LEFT_REDIRECTION = 3,
	RIGHT_REDIRECTION = 4,
	LEFT_D_REDIRECTION = 5,
	RIGHT_D_REDIRECTION = 6,
	WORD = 7,
	TILDE = 8,
}						t_tokens;

typedef struct s_envp
{
	char				**split_path;
	char				**environment;
	char				**export_only;
	long long			exit_code;
	char				*home;
	bool				syntax_error;
}						t_envp;

typedef struct s_expansion_state
{
	char				*str;
	int					pos;
	int					len;
	char				*replacement;
	int					i;
	int					end;
	int					in_single;
	int					in_double;
}						t_expansion_state;

typedef struct s_parse_state
{
	char				*input;
	int					i;
	int					start;
}						t_parse_state;

/// linked list///
typedef struct s_list_node
{
	struct s_list_node	*next;
	struct s_list_node	*prev;
	char				*data;
	int					index;
	char				*redir_arg;
	enum s_tokens		token;
}						t_list_node;

typedef struct s_list
{
	t_list_node			*head;
	int					list_size;
}						t_list;


// stack//

typedef struct s_stack_element
{
	char				*data;
	char				*redir_arg;
	t_tokens			token;
}						t_stack_element;

typedef struct s_stack
{
	int					top;
	t_stack_element		*stack;
}						t_stack;

// tree//

typedef struct s_tree_node
{
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	struct s_tree_node	*root;
	char				*redir_arg;
	t_envp				*path;
	char				*data;
	enum s_tokens		token;
	int					heredoc_created;
}						t_tree_node;

typedef struct s_tree
{
	t_tree_node			*root;
}						t_tree;


typedef struct s_pipe_info
{
	int				pipefd[2];
	pid_t			read_pid;
	pid_t			write_pid;
	t_envp			*env;
	t_gc_list		*grbg;
}	t_pipe_info;

// free_structure
void					free_list(t_list *list);
void					free_stack(t_stack *stack);
void					free_2darray(char **array);
// linked_list_utils
void					insert_at_end_list(t_list *list, char *new_node_data,
							t_gc_list *grgb_collector);
void					insert_at_beginning_list(t_list *list,
							char *new_node_data, t_gc_list *grgb_collector);
void					insert_at_middle_list(t_list *list, char *new_node_data,
							int index, t_gc_list *grgb_collector);
t_list					*init_list(t_gc_list *grbg_collector);
t_list_node				*create_list_node(char *data,
							t_gc_list *grgb_collector);

// linked_list_utils_2
void					print_list(t_list *list);
void					update_list_index(t_list_node *temp);
void					delete_node(t_list *list, t_list_node *node);
t_list_node				*get_last_node(t_list *list);
t_list_node				*get_first_node(t_list *list);
t_list_node				*get_node_at_index(t_list *list, int index);
// stack_utils
void					init_stack(t_list *list, t_stack *stack,
							t_gc_list *grgb_collector);
void					push(char *data, t_tokens token, char *redir_args,
							t_stack *stack);
void					pop(t_stack *stack);
void					print_stack(t_stack *stack);
// tree_helper
void					print_inorder(t_tree_node *node);
t_tree					*init_tree(t_gc_list *grgb_collector);
t_tree_node				*create_tree_node(t_stack *stack, t_envp *environment,
							t_gc_list *grgb_collector);
t_tree_node				*build_tree(t_stack *stack, t_envp *environment,
							t_gc_list *grgb_collector);
// tree_utils
void					swap_redir_to_bottom(t_tree_node **node_ptr);
void					swap_redir_in_tree(t_tree_node *node, t_envp *env,
							t_gc_list *grbg_collector, int *heredoc_counter);
bool					should_swap(t_tree_node *node);
t_tree					*stack_to_tree(t_stack *stack, t_envp *environment,
							t_gc_list *grbg_collector);
void					check_and_remove_empty(t_list *list);
#endif