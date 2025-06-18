/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 22:18:56 by amouflet          #+#    #+#             */
/*   Updated: 2025/06/18 16:14:39 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libftprintf.h"
#include "get_next_line.h"

char	*join_list(t_list *lst)
{
	int		size;
	char	*line;
	char	*ptr;

	if (!lst)
		return (NULL);
	size = ft_lstsize(lst);
	line = malloc(sizeof(char) * ((BUF_SIZE + 1) * size));
	if (!line)
		return (NULL);
	ptr = line;
	while (lst)
	{
		ft_strlcpy(ptr, lst->content, BUF_SIZE + 1);
		ptr += ft_strlen(lst->content);
		lst = lst->next;
	}
	return (line);
}

void	rewrite_buffer(char buffer[], char *line, int pos_in_line)
{
	if (line[pos_in_line] == '\n')
	{
		pos_in_line++;
		ft_strlcpy(buffer, line + pos_in_line, BUF_SIZE + 1);
		line[pos_in_line] = 0;
	}
	else
		buffer[0] = 0;
}

static void	update_stash(char *line, char buffer[])
{
	int		i;
	int		bracket;
	char	quote;

	quote = 0;
	bracket = 0;
	i = 0;
	if (!line)
		return ;
	while (line[i] && !(line[i] == '\n' && !quote && bracket <= 0 && is_line_terminated(line, i)))
	{
		if (quote && quote == line[i])
			quote = 0;
		else if (!quote)
		{
			if (line[i] == '\'' || line[i] == '\"')
				quote = line[i];
			else if (line[i] == '(' || line[i] == ')')
				bracket += (int []){1, -1}[line[i] == ')'];
		}
		i++;
	}
	rewrite_buffer(buffer, line, i);
}

char	*concate_and_update_stash(char buffer[], t_list **head)
{
	char	*str;

	str = join_list(*head);
	update_stash(str, buffer);
	ft_lstclear(head, free);
	return (str);
}

char	*gnl(int fd)
{
	static char	buffer[BUF_SIZE + 1];
	char		quote;
	int			bracket;
	int			nb_bytes;
	t_list		*head;

	if (fd < 0)
		return (NULL);
	quote = 0;
	bracket = 0;
	head = NULL;
	nb_bytes = 1;
	if (buffer[0] && !add_in_list(buffer, &head))
		return (NULL);
	while (nb_bytes > 0 && !closed_word(buffer, &quote, &bracket))
	{
		nb_bytes = read_null_terminated(fd, buffer, BUF_SIZE);
		if (nb_bytes < 0 || (!buffer[0] && !nb_bytes && !head))
			return (ft_lstclear(&head, free), NULL);
		if (!add_in_list(buffer, &head))
			return (NULL);
	}
	return (concate_and_update_stash(buffer, &head));
}
