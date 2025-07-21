/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:25:03 by helin             #+#    #+#             */
/*   Updated: 2025/07/18 19:54:54 by helin            ###   ########.fr       */
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

static void	fork_exit(t_pipex *context, const char *msg)
{
	safe_close(&context->pipefd[0]);
	safe_close(&context->pipefd[1]);
	error_exit(msg);
}

static void	fork_child(t_pipex *context, int id, char **envp)
{
	if (id == 1)
	{
		safe_dup2(context->infile_fd, STDIN_FILENO);
		safe_close(&context->infile_fd);
		safe_dup2(context->pipefd[1], STDOUT_FILENO);
		safe_close(&context->pipefd[0]);
		safe_close(&context->pipefd[1]);
		exec_cmd(context->cmd1, envp);
		error_exit("pipex: exec cmd1 failed");
	}
	else
	{
		safe_dup2(context->outfile_fd, STDOUT_FILENO);
		safe_close(&context->outfile_fd);
		safe_dup2(context->pipefd[0], STDIN_FILENO);
		safe_close(&context->pipefd[0]);
		safe_close(&context->pipefd[1]);
		exec_cmd(context->cmd2, envp);
		error_exit("pipex: exec cmd2 failed");
	}
}

static void	close_and_exit(t_pipex *context)
{
	int	status;

	safe_close(&context->pipefd[0]);
	safe_close(&context->pipefd[1]);
	waitpid(context->pid1, NULL, 0);
	waitpid(context->pid2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
}

void	run_pipex(t_pipex *context, char **envp)
{
	if (pipe(context->pipefd) == -1)
		error_exit("pipex: pipe failed");
	context->pid1 = fork();
	if (context->pid1 == -1)
		fork_exit(context, "pipex: fork failed");
	if (context->pid1 == 0)
	{
		context->infile_fd = open(context->infile, O_RDONLY);
		if (context->infile_fd < 0)
			fork_exit(context, "pipex: open failed");
		fork_child(context, 1, envp);
	}
	context->pid2 = fork();
	if (context->pid2 == -1)
		fork_exit(context, "pipex: fork failed");
	if (context->pid2 == 0)
	{
		context->outfile_fd = open(context->outfile,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (context->outfile_fd < 0)
			fork_exit(context, "pipex: open failed");
		fork_child(context, 2, envp);
	}
	close_and_exit(context);
}
