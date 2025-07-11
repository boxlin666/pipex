/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:21:33 by helin             #+#    #+#             */
/*   Updated: 2025/04/03 18:42:50 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*d;

	len = ft_strlen(s);
	d = malloc(len + 1);
	if (d)
		ft_memcpy(d, s, len + 1);
	return (d);
}
