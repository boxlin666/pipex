/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:46:46 by helin             #+#    #+#             */
/*   Updated: 2025/07/20 15:11:39 by helin            ###   ########.fr       */
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
}			t_pipex_bonus;

void		error_exit(const char *msg);
void		open_exit(t_pipex_bonus *px, const char *msg);
void		open_infile(t_pipex_bonus *px);
void		open_outfile(t_pipex_bonus *px);
void		init_here_doc(t_pipex_bonus *px, int argc, char **argv,
				char **envp);
void		init_pipex(t_pipex_bonus *px, int argc, char **argv, char **envp);
void		exec_cmd(char *cmd_str, char **envp);
void		error_exit(const char *msg);
void		dup_and_redirect(t_pipex_bonus *px, int i);
void		close_all_pipes(t_pipex_bonus *px);
void		run_pipex_bonus(t_pipex_bonus *px);
void		free_pipex_bonus(t_pipex_bonus *px);
void		free_args(char **args);

void		safe_close(int *fd);
void		safe_dup2(int oldfd, int newfd);
void		setup_pipes_and_pids(t_pipex_bonus *px);

#endif