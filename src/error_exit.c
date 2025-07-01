/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:40:24 by helin             #+#    #+#             */
/*   Updated: 2025/07/01 19:44:17 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
