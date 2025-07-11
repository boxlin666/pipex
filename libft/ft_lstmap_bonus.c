/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: helin <helin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:10:34 by helin             #+#    #+#             */
/*   Updated: 2025/05/04 17:19:49 by helin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

static int	ft_lstmap_add_node(t_list **new_list, t_list *current_elem,
		void *(*f)(void *), void (*del)(void *))
{
	void	*new_content;
	t_list	*new_elem;

	new_content = f(current_elem->content);
	if (!new_content)
		return (0);
	new_elem = ft_lstnew(new_content);
	if (!new_elem)
	{
		del(new_content);
		return (0);
	}
	ft_lstadd_back(new_list, new_elem);
	return (1);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		if (!ft_lstmap_add_node(&new_list, lst, f, del))
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		lst = lst->next;
	}
	return (new_list);
}
