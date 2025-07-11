/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:05:21 by helin             #+#    #+#             */
/*   Updated: 2025/07/11 21:54:09 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	dup_and_redirect(t_pipex_bonus *px, int i)
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

void	close_all_pipes(t_pipex_bonus *px)
{
	int	i;

	i = 0;
	while (i < px->cmd_count - 1)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
		i++;
	}
	close(px->here_doc_pipe[0]);
	close(px->here_doc_pipe[1]);
}

void	close_unused_pipes(t_pipex_bonus *px, int i)
{
	int	j;

	j = 0;
	while (j < px->cmd_count - 1)
	{
		if (j != i - 1)
			close(px->pipes[j][0]);
		if (j != i)
			close(px->pipes[j][1]);
		j++;
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

void	free_pipex_bonus(t_pipex_bonus *px)
{
	int	i;

	if (px->cmds)
	{
		i = 0;
		while (i < px->cmd_count)
			free(px->cmds[i++]);
		free(px->cmds);
	}
	if (px->pipes)
	{
		i = 0;
		while (i < px->cmd_count - 1)
			free(px->pipes[i++]);
		free(px->pipes);
	}
	free(px->pids);
	if (px->here_doc)
	{
		close(px->here_doc_pipe[0]);
		close(px->here_doc_pipe[1]);
	}
}
