/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_here_doc_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:26:27 by helin             #+#    #+#             */
/*   Updated: 2025/07/20 15:03:47 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdio.h>

static void	setup_here_doc_cmds(t_pipex_bonus *px, char **argv);

void	write_here_doc_file(t_pipex_bonus *px)
{
	char	*line;

	px->infile_fd = open(px->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, px->limiter, ft_strlen(px->limiter)) == 0
			&& line[ft_strlen(px->limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(px->infile_fd, line, ft_strlen(line));
		free(line);
	}
	safe_close(&px->infile_fd);
}

void	init_here_doc(t_pipex_bonus *px, int argc, char **argv, char **envp)
{
	px->here_doc = 1;
	px->infile = ft_strdup(".heredoc_tmp");
	px->outfile = argv[argc - 1];
	px->limiter = argv[2];
	px->cmd_count = argc - 4;
	px->envp = envp;
	setup_here_doc_cmds(px, argv);
	write_here_doc_file(px);
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
