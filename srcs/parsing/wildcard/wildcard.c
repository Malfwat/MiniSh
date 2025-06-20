/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:24:28 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/20 18:41:48 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
t_snippet	wildcard(char *str, char *cwd)
{
}*/

#include <sys/types.h>
#include <dirent.h>

void	nothing(void *ptr)
{return (void)ptr;}

void	ft_lstpop(t_list **head, t_list *to_pop, void (*del)(void *))
{
	t_list	*lst;
	void	*next;

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
	if (del)
	{
		next = to_pop->next;
		del(to_pop->content);
		free(to_pop);
	}
}

void	print_str(void *str)
{
	if (!str)
		return ;
	write(1, (char *)str, ft_strlen((char *)str));
	write(1, "\n", 1);
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

int	main(int ac, char **av)
{
	DIR	*folder;
	struct dirent	*file;
	char	**patterns;
	t_list	*head;
	t_list	*lst;

	if (ac != 2)
		return (1);
	folder = opendir(".");
	if (!folder)
		return (0);
	head = NULL;
	file = readdir(folder);
	while (file)
	{
		lst = ft_lstnew(file->d_name);
		if (!lst)
			return (0);
		ft_lstadd_back(&head, lst);
		file = readdir(folder);
	}

	patterns = ft_split(av[1], '*');

	lst = head;
	t_list	*next;
	while (lst)
	{
		next = lst->next;
		if (!check_pattern(lst->content, patterns, av[1]))
			ft_lstpop(&head, lst, nothing);
		lst = next;
	}
	ft_lstiter(head, print_str);
	closedir(folder);
	ft_lstclear(&head, nothing);
	ft_free(patterns);
	return (0);
}
