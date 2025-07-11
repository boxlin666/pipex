/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:03:32 by helin             #+#    #+#             */
/*   Updated: 2025/07/09 19:26:02 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*extract_line(char **stash)
{
	char	*line;
	char	*new_stash;
	char	*newline_ptr;
	size_t	len;

	newline_ptr = ft_strchr(*stash, '\n');
	if (newline_ptr)
		len = newline_ptr - *stash + 1;
	else
		len = ft_strlen(*stash);
	line = ft_substr(*stash, 0, len);
	new_stash = ft_substr(*stash, len, ft_strlen(*stash) - len);
	free(*stash);
	*stash = new_stash;
	if (line && *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static int	read_to_stash(int fd, char **stash_ptr)
{
	ssize_t	bytes_read;
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	bytes_read = 1;
	while (!ft_strchr(*stash_ptr, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buf);
			return (-1);
		}
		buf[bytes_read] = '\0';
		*stash_ptr = ft_strjoin(*stash_ptr, buf);
		if (!*stash_ptr && bytes_read == 0)
		{
			free(buf);
			return (-1);
		}
	}
	free(buf);
	return (0);
}

char	*get_next_line(int fd)
{
	char		*line;
	int			result;
	static char	*stash[1024];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	result = read_to_stash(fd, &stash[fd]);
	if (result == -1)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	if (!stash[fd])
		return (NULL);
	line = extract_line(&stash[fd]);
	if (line && *line == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}
