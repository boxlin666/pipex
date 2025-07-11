/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:46:46 by helin             #+#    #+#             */
/*   Updated: 2025/07/10 16:48:05 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex_bonus
{
	int		cmd_count;
	char	**cmds;

	int		**pipes;

	char	*infile;
	char	*outfile;

	int		infile_fd;
	int		outfile_fd;

	int		here_doc;
	char	*limiter;

	pid_t	*pids;

	char	**envp;
	int		here_doc_pipe[2];
}			t_pipex_bonus;

void		error_exit(const char *msg);

void		init_here_doc(t_pipex_bonus *px, int argc, char **argv,
				char **envp);
void		init_pipex(t_pipex_bonus *px, int argc, char **argv, char **envp);
void		exec_cmd(char *cmd_str, char **envp);
void		error_exit(const char *msg);
void		run_pipex_bonus(t_pipex_bonus *px);
void		free_pipex_bonus(t_pipex_bonus *px);
void		free_args(char **args);

#endif