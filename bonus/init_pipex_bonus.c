/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:18:56 by helin             #+#    #+#             */
/*   Updated: 2025/07/11 21:30:08 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdio.h>

static void	setup_cmds(t_pipex_bonus *px, char **argv);
static void	setup_pipes_and_pids(t_pipex_bonus *px);

void	init_pipex(t_pipex_bonus *px, int argc, char **argv, char **envp)
{
	px->here_doc = 0;
	px->infile = argv[1];
	px->outfile = argv[argc - 1];
	px->cmd_count = argc - 3;
	px->limiter = NULL;
	px->envp = envp;
	px->infile_fd = open(px->infile, O_RDONLY);
	if (px->infile_fd < 0)
		perror(px->infile);
	px->outfile_fd = open(px->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->outfile_fd < 0)
		error_exit(px->outfile);
	setup_cmds(px, argv);
	setup_pipes_and_pids(px);
}

static void	setup_cmds(t_pipex_bonus *px, char **argv)
{
	int	i;

	px->cmds = malloc(sizeof(char *) * (px->cmd_count + 1));
	if (!px->cmds)
		error_exit("malloc cmds");
	i = 0;
	while (i < px->cmd_count)
	{
		px->cmds[i] = argv[i + 2];
		i++;
	}
	px->cmds[px->cmd_count] = NULL;
}

static void	setup_pipes_and_pids(t_pipex_bonus *px)
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
