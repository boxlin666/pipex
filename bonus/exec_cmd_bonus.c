/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:24:39 by helin             #+#    #+#             */
/*   Updated: 2025/07/09 18:47:55 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <errno.h>
#include <stdio.h>

void	exec_cmd(char *cmd_str, char **envp)
{
	char	*argv[4];

	argv[0] = "sh";
	argv[1] = "-c";
	argv[2] = cmd_str;
	argv[3] = NULL;
	execve("/bin/sh", argv, envp);
	perror("execve failed");
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES)
		exit(126);
	else
		exit(1);
}
