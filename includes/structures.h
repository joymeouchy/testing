/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 08:16:36 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/31 21:52:31 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <sys/wait.h>  
# include "includes/garbage_collector.h" 

typedef enum e_tokens
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
}	e_tokens;

typedef struct s_envp
{
	char	**split_path;
	char	**environment;
	char	**export_only;
	long long	exit_code;
	char	*home;
}t_envp;

///linked list///
typedef struct s_list_node
{
	struct s_list_node	*next;
	struct s_list_node	*prev;
	char				*data;
	int					index;
	char				*redir_arg;
	enum e_tokens		token;
}	t_list_node;

typedef struct s_list
{
	t_list_node	*head;
	int		list_size;
}	t_list;

//tree//

typedef struct s_tree_node
{
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	struct s_tree_node	*root;
	char				*redir_arg;
	t_envp				*path;
	char				*data;
	enum e_tokens		token;
	
}	t_tree_node;

typedef struct s_tree
{
	t_tree_node	*root;
}	t_tree;

//stack//

typedef struct s_stack_element
{
    char *data;
	char				*redir_arg;
    e_tokens token;
} t_stack_element;

typedef struct s_stack
{
	int top;
	t_stack_element	*stack;
} t_stack;

//free_structure
void	free_list(t_list *list);
void	free_stack(t_stack *stack);
void	free_2darray(char **array);
//linked_list_utils
void	insert_at_end_list(t_list *list, char *new_node_data, t_gc_list *grgb_collector);
void	insert_at_beginning_list(t_list *list, char *new_node_data, t_gc_list *grgb_collector);
void	insert_at_middle_list(t_list *list, char *new_node_data, int index, t_gc_list *grgb_collector);
t_list	*init_list(t_gc_list *grbg_collector);
t_list_node	*create_list_node(char *data, t_gc_list *grgb_collector);

//linked_list_utils_2
void	print_list(t_list *list);
void	update_list_index(t_list_node *temp);
void	delete_node(t_list *list, t_list_node *node);
t_list_node	*get_last_node(t_list *list);
t_list_node	*get_first_node(t_list *list);
t_list_node	*get_node_at_index(t_list *list, int index);
//stack_utils
void	init_stack(t_list *list, t_stack *stack, t_gc_list *grgb_collector);
void	push(char *data, e_tokens token, char *redir_args, t_stack *stack);
void	pop(t_stack *stack);
void	print_stack(t_stack *stack);
//tree_helper
void	print_inorder(t_tree_node *node);
t_tree	*init_tree(t_gc_list *grgb_collector);
t_tree_node	*create_tree_node(t_stack *stack, t_envp *environment, t_gc_list *grgb_collector);
t_tree_node	*build_tree(t_stack *stack, t_envp *environment, t_gc_list *grgb_collector);
//tree_utils
void	swap_redir_to_bottom(t_tree_node **node_ptr);
void	swap_redir_in_tree(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector, int *heredoc_counter);
bool	should_swap(t_tree_node *node);
t_tree	*stack_to_tree(t_stack *stack, t_envp *environment, t_gc_list *grbg_collector);

void	check_and_remove_empty(t_list *list);
#endif