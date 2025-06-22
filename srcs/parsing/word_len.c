/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_len.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:39:22 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/22 21:28:29 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_closing_bracket(char *opening_bracket)
{
	char	*ptr;
	int		i;

	ptr = opening_bracket;
	i = 1;
	while (ptr[i] && ptr[i] != ')')
	{
		if (ptr[i] == '\'' || ptr[i] == '"')
			i += closing_match(ptr + i);
		else if (ptr[i] == '(')
			i += find_closing_bracket(ptr + i);
		i++;
	}
	return (i);
}

int	closing_match(char *ptr)
{
	char	*tmp;

	if (*ptr == '\'' || *ptr == '"')
	{
		tmp = ft_strchr(ptr + 1, *ptr);
		if (!tmp)
			return (0);
		return (tmp - ptr);
	}
	return (find_closing_bracket(ptr));
}

int	word_len(char *str, bool (*is_sep)(char ), int len)
{
	int		i;

	i = 0;
	while (i < len && str[i])
	{
		i += pass_whitespace(str + i) - (str + i);
		if (!i && is_sep(str[i]))
		{
			if (*str == '(' || *str == ')' || *str == ';')
				return (1);
			if (*str == '<' || *str == '>' || *str == '|' || *str == '&')
				return ((int []){1, 2}[str[0] == str[1]]);
			i++;
		}
		while (str[i] && !is_sep(str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || is_sep(str[i]) || (is_sep(str[i]) && *str == '$'))
			return (i);
		if (!ft_strchr(str + i + 1, str[i]))
			return (len);
		i += closing_match(str + i) + 1;
	}
	return (i);
}

int	dollar_len(char *str)
{
	int	i;

	if (!str || *str != '$')
		return (0);
	i = 1;
	if (str[1] == '?' || str[1] == '$')
		return (2);
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

int	get_wlen(char *ptr, int len)
{
	int	wlen;

	if (*ptr == '$')
		wlen = dollar_len(ptr);
	else if (*ptr == '\'' || *ptr == '"')
		wlen = closing_match(ptr) + 1;
	else
		wlen = word_len(ptr, dollar_n_sep, len);
	return (wlen);
}
