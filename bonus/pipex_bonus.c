/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:05:21 by helin             #+#    #+#             */
/*   Updated: 2025/07/10 16:48:58 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	dup_and_redirect(t_pipex_bonus *px, int i)
{
	if (i == 0)
	{
		if (px->here_doc == 1)
			dup2(px->here_doc_pipe[0], STDIN_FILENO);
		else
			dup2(px->infile_fd, STDIN_FILENO);
		dup2(px->pipes[0][1], STDOUT_FILENO);
	}
	else if (i == px->cmd_count - 1)
	{
		dup2(px->pipes[i - 1][0], STDIN_FILENO);
		dup2(px->outfile_fd, STDOUT_FILENO);
	}
	else
	{
		dup2(px->pipes[i - 1][0], STDIN_FILENO);
		dup2(px->pipes[i][1], STDOUT_FILENO);
	}
}

static void	close_all_pipes(t_pipex_bonus *px)
{
	for (int i = 0; i < px->cmd_count - 1; i++)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
	}
	close(px->here_doc_pipe[0]);
	close(px->here_doc_pipe[1]);
}

static void	close_unused_pipes(t_pipex_bonus *px, int i)
{
	for (int j = 0; j < px->cmd_count - 1; j++)
	{
		if (j != i - 1)
			close(px->pipes[j][0]);
		if (j != i)
			close(px->pipes[j][1]);
	}
	if (px->here_doc)
	{
		if (i != 0)
			close(px->here_doc_pipe[0]);
		close(px->here_doc_pipe[1]);
	}
	else
	{
		if (i != 0)
			close(px->infile_fd);
	}
}

void	run_pipex_bonus(t_pipex_bonus *px)
{
	int	last_status;
	int	status;

	if (!px->here_doc)
	{
		px->infile_fd = open(px->infile, O_RDONLY);
		if (px->infile_fd < 0)
			error_exit("infile open");
	}
	for (int i = 0; i < px->cmd_count; i++)
	{
		px->pids[i] = fork();
		if (px->pids[i] < 0)
			error_exit("fork");
		if (px->pids[i] == 0)
		{
			dup_and_redirect(px, i);
			close_unused_pipes(px, i);
			close(px->outfile_fd);
			exec_cmd(px->cmds[i], px->envp);
			exit(1);
		}
	}
	close_all_pipes(px);
	close(px->outfile_fd);
	last_status = 0;
	for (int i = 0; i < px->cmd_count; i++)
	{
		waitpid(px->pids[i], &status, 0);
		if (i == px->cmd_count - 1)
			last_status = status;
	}
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else
		exit(1);
}

void	free_pipex_bonus(t_pipex_bonus *px)
{
	int	i;

	if (px->cmds)
	{
		for (i = 0; i < px->cmd_count; i++)
			free(px->cmds[i]);
		free(px->cmds);
	}
	if (px->pipes)
	{
		for (i = 0; i < px->cmd_count - 1; i++)
			free(px->pipes[i]);
		free(px->pipes);
	}
	free(px->pids);
	if (px->here_doc)
	{
		close(px->here_doc_pipe[0]);
		close(px->here_doc_pipe[1]);
	}
}
