/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 18:11:51 by helin             #+#    #+#             */
/*   Updated: 2025/07/20 16:16:46 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	open_exit(t_pipex_bonus *px, const char *msg)
{
	if (px->here_doc)
		unlink(px->infile);
	close_all_pipes(px);
	error_exit(msg);
}

void	open_infile(t_pipex_bonus *px)
{
	px->infile_fd = open(px->infile, O_RDONLY);
	if (px->infile_fd < 0)
		open_exit(px, "infile open");
}

void	open_outfile(t_pipex_bonus *px)
{
	if (px->here_doc == 1)
		px->outfile_fd = open(px->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		px->outfile_fd = open(px->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (px->outfile_fd < 0)
		open_exit(px, "outfile open");
}
