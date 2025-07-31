/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:31:01 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/07/31 21:42:05 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *open_heredoc_file(int *temp_fd, t_gc_list *grbg_collector, int heredoc_counter)
{
	char *filename;

	filename = ft_strdup("heredoc_temp", grbg_collector);
	filename = ft_strjoin(filename, ft_itoa(heredoc_counter, grbg_collector), grbg_collector);
	filename = ft_strjoin(filename, ".txt", grbg_collector);
	*temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*temp_fd == -1)
		perror("Error creating temporary file for heredoc");
	return (filename);
}

void replace_heredoc_node(t_tree_node *node, char *filename)
{
	node->data = "<";
	node->token = LEFT_REDIRECTION;
	node->redir_arg = filename;
}

static void	write_heredoc_to_file(int temp_fd, char *delimiter, t_envp *env, t_gc_list *grbg_collector)
{
	char	*line;
	int		quotes_in_delimiter;

	quotes_in_delimiter = check_if_contains_quotes(delimiter);
	remove_quotes_from_string(delimiter);
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (quotes_in_delimiter == 0)
			line = expand(line, env, grbg_collector);
		if (!line)
		{
			ft_putendl_fd("expand returned NULL", 2);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
}

void	heredoc(t_tree_node *node, t_envp *env, t_gc_list *grbg_collector, int heredoc_counter)
{
	int		temp_fd;
	char	*filename;

	filename = open_heredoc_file(&temp_fd, grbg_collector, heredoc_counter);
	if (temp_fd == -1)
	{
		ft_free_gc(grbg_collector);
		exit(1);
	}
	write_heredoc_to_file(temp_fd, node->redir_arg, env, grbg_collector);
	close(temp_fd);
	replace_heredoc_node(node, filename);
}

