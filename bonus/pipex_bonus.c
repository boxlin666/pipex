/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:05:21 by helin             #+#    #+#             */
/*   Updated: 2025/07/01 21:22:25 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void	error_exit(const char *msg);

static void	dup_and_redirect(t_pipex_bonus *px, int i)
{
	if (i == 0)
	{
		// here_doc 模式首个命令：stdin -> here_doc_pipe[0], stdout -> pipe[0][1]
		dup2(px->here_doc_pipe[0], STDIN_FILENO);
		dup2(px->pipes[0][1], STDOUT_FILENO);
	}
	else if (i == px->cmd_count - 1)
	{
		// 最后一个命令：stdin -> pipe[i-1][0], stdout -> outfile (追加)
		dup2(px->pipes[i - 1][0], STDIN_FILENO);
		dup2(px->outfile_fd, STDOUT_FILENO);
	}
	else
	{
		// 中间命令：stdin -> pipe[i-1][0], stdout -> pipe[i][1]
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
	// 关闭 here_doc pipe（已复制 fd 后）
	close(px->here_doc_pipe[0]);
	close(px->here_doc_pipe[1]);
}

void	run_pipex_bonus(t_pipex_bonus *px)
{
	for (int i = 0; i < px->cmd_count; i++)
	{
		px->pids[i] = fork();
		if (px->pids[i] < 0)
			error_exit("fork");

		if (px->pids[i] == 0)
		{
			dup_and_redirect(px, i);
			close_all_pipes(px);
			close(px->outfile_fd);
			exec_cmd(px->cmds[i], px->envp);
			exit(1); // exec_cmd 失败
		}
	}

	close_all_pipes(px);
	close(px->outfile_fd);

	// 等待所有子进程（你也可以只等待最后一个）
	int status;
	for (int i = 0; i < px->cmd_count; i++)
		waitpid(px->pids[i], &status, 0);

	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	else
		exit(1);
}

void	free_pipex_bonus(t_pipex_bonus *px)
{
	int	i;

	// 释放命令数组（每个字符串 + 整个数组）
	if (px->cmds)
	{
		for (i = 0; i < px->cmd_count; i++)
			free(px->cmds[i]);
		free(px->cmds);
	}

	// 释放管道数组（每个 int[2] + 外层数组）
	if (px->pipes)
	{
		for (i = 0; i < px->cmd_count - 1; i++)
			free(px->pipes[i]);
		free(px->pipes);
	}

	// 释放 pids 数组
	free(px->pids);

	// 如果是 here_doc 模式，关闭管道
	if (px->here_doc)
	{
		close(px->here_doc_pipe[0]);
		close(px->here_doc_pipe[1]);
	}
}