/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipex_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:39:53 by helin             #+#    #+#             */
/*   Updated: 2025/07/11 21:50:54 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	open_infile_if_needed(t_pipex_bonus *px)
{
	if (!px->here_doc)
	{
		px->infile_fd = open(px->infile, O_RDONLY);
		if (px->infile_fd < 0)
			error_exit("infile open");
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
			dup_and_redirect(px, i);
			close_unused_pipes(px, i);
			close(px->outfile_fd);
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

	open_infile_if_needed(px);
	fork_and_exec_all(px);
	close_all_pipes(px);
	close(px->outfile_fd);
	last_status = wait_for_children(px);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else
		exit(1);
}
