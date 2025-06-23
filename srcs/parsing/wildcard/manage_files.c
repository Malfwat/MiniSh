/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:49:39 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 09:55:38 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

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
	return (ft_lstsort(head), closedir(dir), head);
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
			ft_lstdelpop(head, lst, free);
		lst = next;
	}
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

void	pop_non_matching_files(t_list **head, char **patterns, char *raw_p)
{
	t_list	*lst;
	t_list	*next;

	lst = *head;
	while (lst)
	{
		next = lst->next;
		if (!check_pattern(lst->content, patterns, raw_p))
			ft_lstdelpop(head, lst, free);
		lst = next;
	}
}
