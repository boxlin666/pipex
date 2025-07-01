/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:25:03 by helin             #+#    #+#             */
/*   Updated: 2025/07/01 19:52:29 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void	run_child1(int infile_fd, int pipe_write, char *cmd1, char **envp)
{
	if (infile_fd < 0)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(cmd1);
		exit(1);
	}
	dup2(infile_fd, STDIN_FILENO);
	dup2(pipe_write, STDOUT_FILENO);
	close(infile_fd);
	close(pipe_write);
	exec_cmd(cmd1, envp);
}

static void	run_child2(int outfile_fd, int pipe_read, char *cmd2, char **envp)
{
	if (outfile_fd < 0)
	{
		perror("pipex: outfile");
		exit(1);
	}
	dup2(pipe_read, STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	close(pipe_read);
	exec_cmd(cmd2, envp);
}

static void	close_and_exit(t_pipex *context, pid_t pid1, pid_t pid2)
{
	int	status;

	close(context->infile_fd);
	close(context->outfile_fd);
	close(context->pipefd[0]);
	close(context->pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	exit(1);
}

void	run_pipex(t_pipex *context, char **envp)
{
	context->infile_fd = open(context->infile, O_RDONLY);
	context->outfile_fd = open(context->outfile, O_CREAT | O_WRONLY | O_TRUNC,
			0644);
	if (pipe(context->pipefd) == -1)
		error_exit("pipe");
	context->pid1 = fork();
	if (context->pid1 < 0)
		error_exit("fork");
	if (context->pid1 == 0)
	{
		close(context->pipefd[0]);
		run_child1(context->infile_fd, context->pipefd[1], context->cmd1, envp);
	}
	context->pid2 = fork();
	if (context->pid2 < 0)
		error_exit("fork");
	if (context->pid2 == 0)
	{
		close(context->pipefd[1]);
		run_child2(context->outfile_fd, context->pipefd[0], context->cmd2,
			envp);
	}
	close_and_exit(context, context->pid1, context->pid2);
}
