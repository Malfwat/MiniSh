/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 23:00:33 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/21 23:00:36 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_without_quote(char *str)
{
	char	*new_str;
	char	quote;
	int		i;

	if (!str || !*str)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (false);
	i = 0;
	while (*str)
	{
		while (*str && *str != '\'' && *str != '"')
			new_str[i++] = *(str++);
		if (!*str)
			break ;
		quote = *(str++);
		while (*str != quote)
			new_str[i++] = *(str++);
		str++;
	}
	new_str[i] = 0;
	return (new_str);
}

static bool	replace(t_snippet **head, t_snippet **snip, t_hash_table *table)
{
	t_pair		*pair;
	t_snippet	*new_lst;
	char		*tmp;
	void		*ptr;
	

	pair = get_pair(table, (*snip)->ptr, ft_strlen((*snip)->ptr));
	if (!pair)
		return (true);
	tmp = str_without_quote(pair->value);
	if (!tmp)
		return (false);
	new_lst = lexer(tmp);
	free(tmp);
	if (!new_lst)
		return (false);
	ptr = get_last_snip(new_lst);
	insert_snip(*snip, new_lst);
	pop_snip(head, *snip);
	*snip = ptr;
	return (true);
}

bool	replace_aliases(t_snippet **head, t_hash_table *table)
{
	t_snippet	*lst;
	int			bracket;
	int			prev;

	bracket = 0;
	prev = -1;
	lst = *head;
	while (lst)
	{
		if (lst->token == open_par)
			bracket++;
		else if (lst->token == closing_par)
			bracket--;
		if (lst->token == word && !bracket && (prev == -1 || is_cntl_op(prev)))
			if (!replace(head, &lst, table))
				return (false);
		prev = lst->token;
		lst = lst->next;
	}
	return (true);
}
