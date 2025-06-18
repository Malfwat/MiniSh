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
	if (prev == open_par && (token == closing_par || is_cntl_op(token)))
		return (false);
	if (is_cntl_op(prev) && (is_cntl_op(token) || token == closing_par))
		return (false);
	if (is_redir(prev))
		return (token == word);
	if (prev == closing_par && token == word)
		return (false);
	return (true);
}

bool	check_syntaxe(t_snippet *lst)
{
	enum e_token	prev;
	int				bracket;
	char			*ptr;

	prev = lst->token;
	if (is_cntl_op(prev) || prev == closing_par)
		return (ft_printf("minishell: syntax error near unexpected token `%s'\n", lst->ptr), false);
	lst = lst->next;
	ptr = NULL;
	bracket = (int []){0, 1}[prev == open_par];
	while (lst && is_syntaxe_ok(prev, lst->token))
	{
		if (lst->token == open_par)
			bracket++;
		else if (lst->token == closing_par)
			bracket--;
		prev = lst->token;
		ptr = lst->ptr;
		lst = lst->next;
	}
	if (bracket < 0)
		return (ft_printf("minishell: syntax error near unexpected token `%c'\n", ')'), false);
	if (lst)
		return (ft_printf("minishell: syntax error near unexpected token `%s'\n", lst->ptr), false);
	if (is_cntl_op(prev) || is_redir(prev) || (ptr && ptr[0] == '&'))
		return (ft_printf("minishell: syntax error near unexpected token `%s'\n", ptr), false);
	return (true);
}

