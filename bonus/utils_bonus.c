/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:52:43 by helin             #+#    #+#             */
/*   Updated: 2025/07/20 16:07:48 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	safe_close(int *fd)
{
	if (fd && *fd >= 0)
	{
		if (close(*fd) == -1)
			perror("close");
		*fd = -1;
	}
}

void	safe_dup2(int oldfd, int newfd)
{
	if (oldfd < 0 || newfd < 0)
		error_exit("pipex: invalid fd in dup2");
	if (dup2(oldfd, newfd) == -1)
		error_exit("pipex: dup2 failed");
}

void	setup_pipes_and_pids(t_pipex_bonus *px)
{
	int	i;

	px->pipes = malloc(sizeof(int *) * (px->cmd_count - 1));
	if (!px->pipes)
		error_exit("malloc pipes");
	i = 0;
	while (i < px->cmd_count - 1)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error_exit("malloc pipes[i]");
		if (pipe(px->pipes[i]) == -1)
			error_exit("pipe");
		i++;
	}
	px->pids = malloc(sizeof(pid_t) * px->cmd_count);
	if (!px->pids)
		error_exit("malloc pids");
}

void	close_all_pipes(t_pipex_bonus *px)
{
	int	j;

	j = 0;
	while (j < px->cmd_count - 1)
	{
		safe_close(&px->pipes[j][0]);
		safe_close(&px->pipes[j][1]);
		j++;
	}
}

void	free_pipex_bonus(t_pipex_bonus *px)
{
	int	i;

	i = 0;
	free(px->cmds);
	px->cmds = NULL;
	i = 0;
	while (px->pipes && i < px->cmd_count - 1)
	{
		free(px->pipes[i]);
		i++;
	}
	free(px->pipes);
	px->pipes = NULL;
	free(px->pids);
	px->pids = NULL;
	if (px->here_doc && px->infile)
	{
		unlink(px->infile);
		free(px->infile);
	}
}
