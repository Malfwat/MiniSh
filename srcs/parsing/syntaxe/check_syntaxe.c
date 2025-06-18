/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntaxe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:35:26 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/18 17:36:48 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(enum e_token token)
{
	return (token >= redir_in && token <= append);
}

bool	is_cntl_op(enum e_token token)
{
	return (token >= pipe_delim && token <= semicolon);
}

bool	is_syntaxe_ok(enum e_token prev, enum e_token token)
{
	if (is_cntl_op(prev) && is_cntl_op(token))
		return (false);
	if (is_redir(prev))
		return (token == word);
	return (true);
}

bool	check_syntaxe(t_snippet *lst)
{
	enum e_token	prev;
	char			*ptr;

	prev = lst->token;
	if (prev == semicolon || prev == pipe_delim || prev == closing_par || prev == or || prev == and)
		return (ft_printf("minishell: syntax error near unexpected token `%c'\n", lst->ptr[0]), false);
	lst = lst->next;
	ptr = NULL;
	while (lst && is_syntaxe_ok(prev, lst->token))
	{
		prev = lst->token;
		ptr = lst->ptr;
		lst = lst->next;
	}
	if (lst)
		return (ft_printf("minishell: syntax error near unexpected token `%c'\n", lst->ptr[0]), false);
	if (is_cntl_op(prev) || is_redir(prev) || (ptr && ptr[0] == '&'))
		return (ft_printf("minishell: syntax error near unexpected token `%c'\n", ptr[0]), false);
	return (true);
}

