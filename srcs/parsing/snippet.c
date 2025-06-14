/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   snippet.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:49:29 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/13 21:02:57 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_snippet	*new_snip(enum e_token token, char *ptr)
{
	t_snippet	*new;

	new = malloc(sizeof(t_snippet));
	if (!new)
		return (NULL);
	new->token = token;
	new->ptr = ptr;
	new->next = NULL;
	new->expand_snip = NULL;
	return (new);
}

static t_snippet	*get_last_snip(t_snippet *lst)
{
	if (!lst)
		ft_putstr_fd("Check your code dumbass (get_last_snip)\n", 2);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

bool	add_to_snip_lst(t_snippet **head, enum e_token token, char *ptr)
{
	t_snippet	*new;

	new = new_snip(token, ptr);
	if (!new)
		return (ft_putstr_fd("Error malloc\n", 2), false);
	if (!*head)
		*head = new;
	else
		get_last_snip(*head)->next = new;
	return (true);
}

void	insert_snip(t_snippet *node, t_snippet *to_insert)
{
	if (!node)
		ft_putstr_fd("Check your code dumbass (insert_snip)\n", 2);
	to_insert->next = node->next;
	node->next = to_insert;
}

void	free_snip_lst(t_snippet *lst)
{
	void	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->ptr);
		free(lst);
		lst = tmp;
	}
}
