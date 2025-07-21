/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipex_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:39:53 by helin             #+#    #+#             */
/*   Updated: 2025/07/20 15:52:53 by helin            ###   ########.fr       */
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
		safe_dup2(px->infile_fd, STDIN_FILENO);
		safe_dup2(px->pipes[i][1], STDOUT_FILENO);
		safe_close(&px->infile_fd);
	}
	else if (i == px->cmd_count - 1)
	{
		safe_dup2(px->pipes[i - 1][0], STDIN_FILENO);
		safe_dup2(px->outfile_fd, STDOUT_FILENO);
		safe_close(&px->outfile_fd);
	}
	else
	{
		safe_dup2(px->pipes[i - 1][0], STDIN_FILENO);
		safe_dup2(px->pipes[i][1], STDOUT_FILENO);
	}
}

static void	fork_and_exec_all(t_pipex_bonus *px)
{
	int	i;

	i = 0;
	while (i < px->cmd_count)
	{
		px->pids[i] = fork();
		if (px->pids[i] < 0)
			error_exit("fork");
		if (px->pids[i] == 0)
		{
			if (i == 0)
				open_infile(px);
			else if (i == px->cmd_count - 1)
				open_outfile(px);
			dup_and_redirect(px, i);
			close_all_pipes(px);
			exec_cmd(px->cmds[i], px->envp);
			exit(1);
		}
		i++;
	}
}

static int	wait_for_children(t_pipex_bonus *px)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < px->cmd_count)
	{
		waitpid(px->pids[i], &status, 0);
		if (i == px->cmd_count - 1)
			last_status = status;
		i++;
	}
	return (last_status);
}

void	run_pipex_bonus(t_pipex_bonus *px)
{
	int	last_status;

	fork_and_exec_all(px);
	close_all_pipes(px);
	last_status = wait_for_children(px);
	free_pipex_bonus(px);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else
		exit(1);
}
