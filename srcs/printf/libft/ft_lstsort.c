/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:42:55 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 09:47:50 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lst_swap(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

void	ft_lstsort(t_list *lst)
{
	t_list	*head;
	t_list	*tmp;
	int		cmp;

	head = lst;
	while (lst->next)
	{
		tmp = lst->next;
		cmp = ft_strcmp_icase(lst->content, tmp->content);
		if (cmp > 0)
		{
			lst_swap(lst, tmp);
			lst = head;
		}
		else if (!cmp && ft_strval(lst->content) < ft_strval(tmp->content))
		{
			lst_swap(lst, tmp);
			lst = head;
		}
		else
			lst = lst->next;
	}
}
