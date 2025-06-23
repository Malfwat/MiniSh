/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:24:28 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 09:52:07 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_only_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '*')
		i++;
	return (!str[i]);
}

t_snippet	*lst_to_snip(t_list *lst, char *raw_pattern)
{
	t_snippet	*head;
	char		*dup;

	head = NULL;
	if (!lst)
	{
		dup = ft_strdup(raw_pattern);
		if (!dup)
			return (NULL);
		head = new_snip(word, dup);
		if (!head)
			free(dup);
		return (head);
	}
	while (lst)
	{
		if (!add_to_snip_lst(&head, word, lst->content))
			return (free_snip_lst(head), NULL);
		lst = lst->next;
	}
	return (head);
}

t_snippet	*wildcard(char *raw_pattern)
{
	char		**patterns;
	t_snippet	*snip;
	t_list		*head;

	head = get_all_files();
	if (!head)
		return (NULL);
	if (*raw_pattern != '.')
		take_off_hidden_files(&head);
	if (is_only_wildcard(raw_pattern))
	{
		snip = lst_to_snip(head, raw_pattern);
		ft_lstclear(&head, NULL);
		return (snip);
	}
	patterns = ft_split(raw_pattern, '*');
	if (!patterns)
		return (ft_lstclear(&head, NULL), NULL);
	pop_non_matching_files(&head, patterns, raw_pattern);
	snip = lst_to_snip(head, raw_pattern);
	ft_lstclear(&head, NULL);
	ft_free(patterns);
	return (snip);
}

bool	replace_one_wildcard(t_snippet **head, t_snippet **lst, t_snippet *prev)
{
	t_snippet	*new;
	t_snippet	*last;

	new = wildcard((*lst)->ptr);
	if (!new)
		return (false);
	last = get_last_snip(new);
	last->next = (*lst)->next;
	if (!prev)
		*head = new;
	else
		prev->next = new;
	free((*lst)->ptr);
	free(*lst);
	prev = last;
	*lst = last->next;
	return (true);
}

bool	replace_wildcards(t_snippet **head)
{
	t_snippet	*lst;
	t_snippet	*prev;

	lst = *head;
	prev = NULL;
	while (lst)
	{
		if (ft_strchr(lst->ptr, '*'))
		{
			if (!replace_one_wildcard(head, &lst, prev))
				return (false);
		}
		else
		{
			prev = lst;
			lst = lst->next;
		}
	}
	return (true);
}
