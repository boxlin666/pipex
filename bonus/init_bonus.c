/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:18:56 by helin             #+#    #+#             */
/*   Updated: 2025/07/10 16:50:02 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdio.h>

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
	int	i;

	px->here_doc = 1;
	px->limiter = argv[2];
	px->cmd_count = argc - 4;
	px->infile = NULL;
	px->outfile = argv[argc - 1];
	px->envp = envp;
	px->cmds = malloc(sizeof(char *) * (px->cmd_count + 1));
	if (!px->cmds)
		error_exit("malloc cmds");
	for (i = 0; i < px->cmd_count; i++)
		px->cmds[i] = argv[i + 3];
	px->cmds[px->cmd_count] = NULL;
	px->outfile_fd = open(px->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (px->outfile_fd < 0)
		error_exit(px->outfile);
	if (pipe(px->here_doc_pipe) == -1)
		error_exit("here_doc pipe");
	write_here_doc_input(px->here_doc_pipe[1], px->limiter);
	close(px->here_doc_pipe[1]);
	px->pipes = malloc(sizeof(int *) * (px->cmd_count - 1));
	if (!px->pipes)
		error_exit("malloc pipes");
	for (i = 0; i < px->cmd_count - 1; i++)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error_exit("malloc pipes[i]");
		if (pipe(px->pipes[i]) == -1)
			error_exit("pipe");
	}
	px->pids = malloc(sizeof(pid_t) * px->cmd_count);
	if (!px->pids)
		error_exit("malloc pids");
}

void	init_pipex(t_pipex_bonus *px, int argc, char **argv, char **envp)
{
	int	i;

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
	px->cmds = malloc(sizeof(char *) * (px->cmd_count + 1));
	if (!px->cmds)
		error_exit("malloc cmds");
	for (i = 0; i < px->cmd_count; i++)
		px->cmds[i] = argv[i + 2];
	px->cmds[px->cmd_count] = NULL;
	px->pipes = malloc(sizeof(int *) * (px->cmd_count - 1));
	if (!px->pipes)
		error_exit("malloc pipes");
	for (i = 0; i < px->cmd_count - 1; i++)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error_exit("malloc pipes[i]");
		if (pipe(px->pipes[i]) == -1)
			error_exit("pipe");
	}
	px->pids = malloc(sizeof(pid_t) * px->cmd_count);
	if (!px->pids)
		error_exit("malloc pids");
}
