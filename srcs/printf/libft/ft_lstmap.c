/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:35:34 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 11:55:32 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_all(t_list *begin, void (*del)(void *))
{
	t_list	*tmp;

	while (begin != NULL)
	{
		tmp = begin;
		begin = begin->next;
		del(tmp);
		free(tmp);
	}
	return (begin);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;

	new = NULL;
	while (lst)
	{
		tmp = ft_lstnew(f(lst->content));
		if (!tmp)
			return (free_all(new, del));
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}
