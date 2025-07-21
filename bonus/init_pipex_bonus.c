/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:18:56 by helin             #+#    #+#             */
/*   Updated: 2025/07/19 18:14:54 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include <stdio.h>

static void	setup_cmds(t_pipex_bonus *px, char **argv);

void	init_pipex(t_pipex_bonus *px, int argc, char **argv, char **envp)
{
	px->here_doc = 0;
	px->infile = argv[1];
	px->outfile = argv[argc - 1];
	px->cmd_count = argc - 3;
	px->limiter = NULL;
	px->envp = envp;
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
