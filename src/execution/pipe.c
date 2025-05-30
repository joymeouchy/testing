/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:51:42 by jmeouchy          #+#    #+#             */
/*   Updated: 2025/05/30 20:20:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char *preserve_fds()
// {
//     char *fd
// }

void left_node(t_tree_node *node, int pipefd[2])
{
	// write(1, "i am in right node\n", 20);
	dup2(pipefd[0], 0);  // Read from pipe
	close(pipefd[0]);
	close(pipefd[1]);
	execution(node->left); // Execute right node command
	exit(0); // Important: exit after execution
}

void right_node(t_tree_node *node, int pipefd[2])
{
	// write(1, "i am in right node\n", 19);
	dup2(pipefd[1], 1);  // Write to pipe
	close(pipefd[0]);
	close(pipefd[1]);
	execution(node->right); // Execute left node command
	exit(0); // Important: exit after execution
}


int count_pipes(t_tree_node *node)
{
    int count = 0;
    
    if (!node)
        return 0;
    if (node->token == PIPE)
    {
        count = 1;
        printf("Found a PIPE node\n");
    }
    int left_count = count_pipes(node->left);
    int right_count = count_pipes(node->right);

    int total = count + left_count + right_count;
    return total;
}

void pipe_exec_for_multiple(t_tree_node *node)
{
    // int pipefd[2];
    pid_t left_pid, right_pid;

    if (pipe(node->fd) == -1)
        return;

    left_pid = fork();
    if (left_pid == -1)
        return;
    if (left_pid == 0)
    {
        // Left: writes to pipe
        dup2(node->fd[1], STDOUT_FILENO);
        close(node->fd[0]);
        close(node->fd[1]);
        // write(1,"marhaba ha fout execution bl left\n",35);
        execution(node->left); // ✅ Left writes to pipe
        // write(1,"marhaba done execution bl left\n",32);
        exit(0);
    }

    right_pid = fork();
    if (right_pid == -1)
        return;
    if (right_pid == 0)
    {
        // Right: reads from pipe
        dup2(node->fd[0], STDIN_FILENO);
        close(node->fd[1]);
        close(node->fd[0]);
        // write(1,"marhaba ha fout execution bl right\n",36);
        execution(node->right); // ✅ Right reads from pipe
        // write(1,"marhaba done execution bl right\n", 33);
        exit(0);
    }
    // Parent: close pipe ends
    close(node->fd[0]);
    close(node->fd[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}


void pipe_exec_for_single(t_tree_node *node)
{
    int pipefd[2];
    pid_t left_pid, right_pid;

    if (pipe(pipefd) == -1)
        return ;

    left_pid = fork();
    if (left_pid == -1)
        return ;
    if (left_pid == 0)
    {
        // Left: write to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execution(node->right); // Can recurse into another pipe or command
        exit(0);
    }

    right_pid = fork();
    if (right_pid == -1)
        return ;
    if (right_pid == 0)
    {
        // Right: read from pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        execution(node->left); // Can recurse into another pipe or command
        exit(0);
    }
    // Parent: close pipe ends
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}

void pipe_exec(t_tree_node *node, int pipe_count)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return;

    if (pipe_count == 1)
        return pipe_exec_for_single(node);
    else
        return pipe_exec_for_multiple(node);
}
