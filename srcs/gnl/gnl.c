/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 22:18:56 by amouflet          #+#    #+#             */
/*   Updated: 2022/12/04 16:01:14 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libftprintf.h"

bool	closed_word(char const *str, char *quote_ptr, int *bracket_ptr)
{
	int	i;

	if (!str)
		return (false);
	i = 0;
	while (str[i] /*&& !ft_strchr("\'\"()", str[i])*/)
	{
		if (*quote_ptr)
		{
			if (str[i] == *quote_ptr)
				*quote_ptr = 0;
		}
		else if (!*quote_ptr && (str[i] == '\'' || str[i] == '\"'))
			*quote_ptr = str[i];
		else if (!*quote_ptr && str[i] == '(')
			(*bracket_ptr)++;
		else if (!*quote_ptr && str[i] == ')')
			(*bracket_ptr)--;
		else if (str[i] == '\n' && !*quote_ptr && !*bracket_ptr)
			return (true);
		i++;
	}
	return (false);
}

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

static void	update_stash(char *line, char buffer[])
{
	int		i;
	int		bracket;
	char	quote;

	quote = 0;
	bracket = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\n' && !quote && !bracket)
			break ;
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (!quote)
				quote = line[i];
			else if (quote == line[i])
					quote = 0;
		}
		else if (!quote && (line[i] == '(' || line[i] == ')'))
			bracket += (int []){1, -1}[line[i] == ')'];
		i++;
	}
	if (line[i] == '\n')
	{
		i++;
		ft_strlcpy(buffer, line + i, BUF_SIZE + 1);
		line[i] = 0;
	}
	else
		buffer[0] = 0;
}

char	*gnl(int fd)
{
	static char	buffer[BUF_SIZE + 1];
	char	quote;
	int		bracket;
	int		nb_bytes;
	t_list	*head;
	t_list	*new;
	char	*str;

	if (fd < 0)
		return (NULL);
	quote = 0;
	bracket = 0;
	head = NULL;
	nb_bytes = 1;
	if (buffer[0])
	{
		str = ft_strdup(buffer);
		if (!str)
			return (ft_lstclear(&head, free), NULL);
		new = ft_lstnew(str);
		if (!new)
			return (ft_lstclear(&head, free), NULL);
		ft_lstadd_back(&head, new);
	}
	while (nb_bytes > 0 && !closed_word(buffer, &quote, &bracket))
	{
		nb_bytes = read(fd, buffer, BUF_SIZE);
		if (nb_bytes < 0 || (!buffer[0] && !nb_bytes && !head))
			return (NULL);
		buffer[nb_bytes] = 0;
		str = ft_strdup(buffer);
		if (!str)
			return (ft_lstclear(&head, free), NULL);
		new = ft_lstnew(str);
		if (!new)
			return (ft_lstclear(&head, free), NULL);
		ft_lstadd_back(&head, new);
	}
	str = join_list(head);
	if (str)
		update_stash(str, buffer);
	return (ft_lstclear(&head, free), str);
}
