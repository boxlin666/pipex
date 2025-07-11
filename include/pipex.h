/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:41:28 by helin             #+#    #+#             */
/*   Updated: 2025/07/10 16:41:30 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		pipefd[2];
	int		infile_fd;
	int		outfile_fd;
	char	*infile;
	char	*outfile;
	char	*cmd1;
	char	*cmd2;
	pid_t	pid1;
	pid_t	pid2;
}			t_pipex;

void		error_exit(const char *msg);

void		exec_cmd(char *cmd_str, char **envp);
void		run_pipex(t_pipex *context, char **envp);
void		free_args(char **args);

#endif
