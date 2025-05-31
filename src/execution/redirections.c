/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeouchy <jmeouchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:31:09 by lkhoury           #+#    #+#             */
/*   Updated: 2025/05/31 14:28:39 by jmeouchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	handle_redir(int *tokens, char **files, int redir_count)
// {
// 	int		fd;
// 	int		i;
// 	int		temp_fd;
// 	char	*delimiter;
// 	char	*line;

// 	i = 0;
// 	while (i < redir_count)
// 	{
// 		if (tokens[i] == 3)
// 		{
// 			fd = open(files[i], O_RDONLY);
// 			if (fd == -1)
// 			{
// 				perror("Error opening input file");
// 				return (1);
// 			}
// 			if (dup2(fd, STDIN_FILENO) == -1)
// 			{
// 				perror("Error redirecting input");
// 				close(fd);
// 				return (1);
// 			}
// 			close(fd);
// 		}
// 		else if (tokens[i] == 4)
// 		{
// 			fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd == -1)
// 			{
// 				perror("Error opening output file");
// 				return (1);
// 			}
// 			if (dup2(fd, STDOUT_FILENO) == -1)
// 			{
// 				perror("Error redirecting output");
// 				close(fd);
// 				return (1);
// 			}
// 			close(fd);
// 		}
// 		else if (tokens[i] == 6)
// 		{
// 			fd = open(files[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			if (fd == -1)
// 			{
// 				perror("Error opening output file for append");
// 				return (1);
// 			}
// 			if (dup2(fd, STDOUT_FILENO) == -1)
// 			{
// 				perror("Error redirecting output");
// 				close(fd);
// 				return (1);
// 			}
// 			close(fd);
// 		}
// 		else if (tokens[i] == 5)
// 		{
// 			delimiter = files[i];
// 			temp_fd = open("heredoc_temp.txt",
// 					O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (temp_fd == -1)
// 			{
// 				perror("Error creating temporary file for heredoc");
// 				return (1);
// 			}
// 			while (1)
// 			{
// 				line = readline("> ");
// 				if (!line)
// 					break ;
// 				if (ft_strcmp(line, delimiter) == 0)
// 				{
// 					free(line);
// 					break ;
// 				}
// 				write(temp_fd, line, ft_strlen(line));
// 				write(temp_fd, "\n", 1);
// 				free(line);
// 			}
// 			close(temp_fd);
// 			temp_fd = open("heredoc_temp.txt", O_RDONLY);
// 			if (temp_fd == -1)
// 			{
// 				perror("Error opening temporary heredoc file for reading");
// 				return (1);
// 			}
// 			if (dup2(temp_fd, STDIN_FILENO) == -1)
// 			{
// 				perror("Error redirecting stdin to heredoc");
// 				close(temp_fd);
// 				return (1);
// 			}
// 			close(temp_fd);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

void	redir_input(t_tree_node *node)
{
	int		file_fd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		file_fd = open(node->redir_arg, O_RDONLY);
		if (file_fd == -1)
		{
			perror("Error opening input file");
			exit(1);
		}
		if (dup2(file_fd, STDIN_FILENO) == -1)
		{
			perror("Error redirecting input");
			close(file_fd);
			exit(1);
		}
		close(file_fd);
		execution(node->right);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	redir_output(t_tree_node *node)
{
	int		file_fd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		file_fd = open(node->redir_arg, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (file_fd == -1)
		{
			perror("Error opening input file");
			exit(1);
		}
		if (dup2(file_fd, STDOUT_FILENO) == -1)
		{
			perror("Error redirecting input");
			close(file_fd);
			exit(1);
		}
		close(file_fd);
		execution(node->right);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	redir_output_append(t_tree_node *node)
{
	int		file_fd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		file_fd = open(node->redir_arg, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (file_fd == -1)
		{
			perror("Error opening input file");
			exit(1);
		}
		if (dup2(file_fd, STDOUT_FILENO) == -1)
		{
			perror("Error redirecting input");
			close(file_fd);
			exit(1);
		}
		close(file_fd);
		execution(node->right);
		exit(1);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
}

void	handle_recirections(t_tree_node *node)
{
	if (node->token == LEFT_REDIRECTION)
		redir_input(node);
	else if (node->token == RIGHT_REDIRECTION)
		redir_output(node);
	else if (node->token == RIGHT_D_REDIRECTION)
		redir_output_append(node);
}
