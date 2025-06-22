/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:24:28 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/22 20:34:20 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

void	ft_lstpop(t_list **head, t_list *to_pop)
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
	free(to_pop->content);
	free(to_pop);
}

bool	check_pattern(char *str, char **patterns, char *raw_pattern)
{
	int	i;

	i = 0;
	if (*str == '.' && *raw_pattern != '.')
		return (false);
	if (*raw_pattern != '*')
	{
		if (ft_strncmp(*patterns, str, ft_strlen(*patterns)))
			return (false);
		str += ft_strlen(*patterns);
		i++;
	}
	while (patterns[i])
	{
		str = ft_strnstr(str, patterns[i], ft_strlen(str));
		if (!str)
			return (false);
		str += ft_strlen(patterns[i]);
		i++;
	}
	if (raw_pattern[ft_strlen(raw_pattern) - 1] != '*' && *str)
		return (false);
	return (true);
}

void	lst_swap(t_list *a, t_list *b)
{
	void	*tmp;

	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

int	str_val(char *str)
{
	int	val;

	val = 0;
	while (*str)
		val += *(str++);
	return (val);
}

void	sort_lst(t_list *lst)
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
		else if (!cmp && str_val(lst->content) < str_val(tmp->content))
		{
			lst_swap(lst, tmp);
			lst = head;
		}
		else
			lst = lst->next;
	}
}

t_list	*get_a_file(const char *name)
{
	t_list	*lst;
	char	*dup;

	dup = ft_strdup(name);
	if (!dup)
		return (NULL);
	lst = ft_lstnew(dup);
	if (!lst)
		return (free(dup), NULL);
	return (lst);
}

t_list	*get_all_files(void)
{
	DIR				*dir;
	struct dirent	*file;
	t_list			*head;
	t_list			*lst;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	file = readdir(dir);
	head = NULL;
	while (file)
	{
		if (!ft_strcmp(file->d_name, ".") || !ft_strcmp(file->d_name, ".."))
		{
			file = readdir(dir);
			continue ;
		}
		lst = get_a_file(file->d_name);
		if (!lst)
			return (ft_lstclear(&head, free), closedir(dir), NULL);
		ft_lstadd_back(&head, lst);
		file = readdir(dir);
	}
	return (sort_lst(head), closedir(dir), head);
}

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

void	take_off_hidden_files(t_list **head)
{
	t_list	*lst;
	t_list	*next;

	if (!head || !*head)
		return ;
	lst = *head;
	while (lst)
	{
		next = lst->next;
		if (*(char *)lst->content == '.')
			ft_lstpop(head, lst);
		lst = next;
	}
}

void	pop_non_matching_files(t_list **head, char **patterns, char *raw_p)
{
	t_list	*lst;
	t_list	*next;

	lst = *head;
	while (lst)
	{
		next = lst->next;
		if (!check_pattern(lst->content, patterns, raw_p))
			ft_lstpop(head, lst);
		lst = next;
	}
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
