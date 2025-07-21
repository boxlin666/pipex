/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:52:43 by helin             #+#    #+#             */
/*   Updated: 2025/07/18 19:54:22 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

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
