/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:25:07 by helin             #+#    #+#             */
/*   Updated: 2025/07/01 19:44:39 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	context;

	if (argc != 5)
		error_exit("Usage: ./pipex infile \"cmd1\" \"cmd2\" outfile");
	context.infile = argv[1];
	context.cmd1 = argv[2];
	context.cmd2 = argv[3];
	context.outfile = argv[4];
	run_pipex(&context, envp);
	return (0);
}
