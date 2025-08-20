/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:10:08 by lkhoury           #+#    #+#             */
/*   Updated: 2025/08/20 20:29:50 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*open_heredoc_file(int *temp_fd, t_gc_list *grbg_collector,
		int heredoc_counter)
{
	char	*filename;
	char	*base;
	int		fd;
	int		i;

	i = heredoc_counter;
	while (1)
	{
		base = ft_strjoin(ft_strdup("heredoc_temp", grbg_collector), ft_itoa(i,
					grbg_collector), grbg_collector);
		filename = ft_strjoin(base, ".txt", grbg_collector);
		fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd != -1)
		{
			*temp_fd = -1;
			break ;
		}
		if (errno != EEXIST)
			return (perror("Error creating temporary file for heredoc"), NULL);
		i++;
	}
	*temp_fd = fd;
	return (filename);
}

void	replace_heredoc_node(t_tree_node *node, char *filename)
{
	node->data = "<";
	node->token = LEFT_REDIRECTION;
	node->redir_arg = filename;
	node->heredoc_created = 1;
}

void	write_heredoc_to_file(int temp_fd, char *delimiter, t_envp *env,
		t_gc_list *grbg_collector)
{
	char	*line;
	int		quotes_in_delimiter;

	quotes_in_delimiter = check_if_contains_quotes(delimiter);
	remove_quotes_from_string(delimiter);
	while (1)
	{
		line = readline("> ");
		if (quotes_in_delimiter == 0)
			line = expand_heredoc(line, env, grbg_collector);
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, temp_fd);
		free(line);
	}
}

void	ctrl_c_heredoc(int sig)
{
	(void)sig;
	printf("\n");
	g_sigint = 130;
	exit(130);
}

void	set_heredoc_signals(void)
{
	signal(SIGINT, ctrl_c_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
