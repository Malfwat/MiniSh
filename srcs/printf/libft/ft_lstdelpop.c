/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelpop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:39:47 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 09:40:10 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelpop(t_list **head, t_list *to_pop, void (*del)(void *))
{
	t_list	*lst;

	lst = *head;
	if (!to_pop || !head || !*head)
		return ;
	if (*head == to_pop)
		*head = (*head)->next;
	else
	{
		while (lst && lst->next != to_pop)
			lst = lst->next;
		if (lst)
			lst->next = to_pop->next;
	}
	ft_lstdelone(to_pop, del);
}
