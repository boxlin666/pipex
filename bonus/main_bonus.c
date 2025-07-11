/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:05:25 by helin             #+#    #+#             */
/*   Updated: 2025/07/09 19:46:13 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include "libft.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex_bonus	px;

	if (argc < 5)
		error_exit("Usage: ./pipex infile cmd1 cmd2 ... outfile\n\
or: ./pipex here_doc LIMITER cmd1 cmd2 ... outfile");
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		init_here_doc(&px, argc, argv, envp);
	else
		init_pipex(&px, argc, argv, envp);
	run_pipex_bonus(&px);
	free_pipex_bonus(&px);
	return (0);
}
