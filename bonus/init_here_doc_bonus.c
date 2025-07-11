/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_here_doc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:26:27 by helin             #+#    #+#             */
/*   Updated: 2025/07/11 21:30:38 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdio.h>

static void	setup_here_doc_cmds(t_pipex_bonus *px, char **argv);
static void	setup_pipes_and_pids(t_pipex_bonus *px);

void	write_here_doc_input(int write_fd, const char *limiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
	}
}

void	init_here_doc(t_pipex_bonus *px, int argc, char **argv, char **envp)
{
	px->here_doc = 1;
	px->limiter = argv[2];
	px->cmd_count = argc - 4;
	px->infile = NULL;
	px->outfile = argv[argc - 1];
	px->envp = envp;
	setup_here_doc_cmds(px, argv);
	px->outfile_fd = open(px->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (px->outfile_fd < 0)
		error_exit(px->outfile);
	if (pipe(px->here_doc_pipe) == -1)
		error_exit("here_doc pipe");
	write_here_doc_input(px->here_doc_pipe[1], px->limiter);
	close(px->here_doc_pipe[1]);
	setup_pipes_and_pids(px);
}

static void	setup_here_doc_cmds(t_pipex_bonus *px, char **argv)
{
	int	i;

	px->cmds = malloc(sizeof(char *) * (px->cmd_count + 1));
	if (!px->cmds)
		error_exit("malloc cmds");
	i = 0;
	while (i < px->cmd_count)
	{
		px->cmds[i] = argv[i + 3];
		i++;
	}
	px->cmds[px->cmd_count] = NULL;
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
