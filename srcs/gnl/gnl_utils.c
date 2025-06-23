/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 23:01:55 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 10:04:33 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include <unistd.h>

bool	is_line_terminated(char *buffer, int i)
{
	while (i && ((buffer[i] >= 9 && buffer[i] <= 13) || buffer[i] == ' '))
		i--;
	if (!i || (buffer[i - 1] != '|' && buffer[i - 1] != '&'))
		return (true);
	return (false);
}

bool	closed_word(char const buffer[], char *quote_ptr, int *bracket_ptr)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (*quote_ptr)
		{
			if (buffer[i] == *quote_ptr)
				*quote_ptr = 0;
		}
		else if (!*quote_ptr)
		{
			if (buffer[i] == '\'' || buffer[i] == '\"')
				*quote_ptr = buffer[i];
			else if (buffer[i] == '(')
				(*bracket_ptr)++;
			else if (buffer[i] == ')')
				(*bracket_ptr)--;
			else if (buffer[i] == '\n' && !*bracket_ptr)
				return (is_line_terminated((char *)buffer, i));
		}
		i++;
	}
	return (false);
}

bool	add_in_list(char buffer[], t_list **head)
{
	char	*str;
	t_list	*new;

	if (!buffer[0])
		return (true);
	str = ft_strdup(buffer);
	if (!str)
		return (ft_lstclear(head, free), false);
	new = ft_lstnew(str);
	if (!new)
		return (ft_lstclear(head, free), free(str), false);
	ft_lstadd_back(head, new);
	return (true);
}
