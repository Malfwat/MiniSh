/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   snippet.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:49:29 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 11:41:51 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"

t_snippet	*new_snip(enum e_token token, char *ptr)
{
	t_snippet	*new;

	new = malloc(sizeof(t_snippet));
	if (!new)
		return (NULL);
	new->token = token;
	new->ptr = ptr;
	new->next = NULL;
	return (new);
}

t_snippet	*get_last_snip(t_snippet *lst)
{
	if (!lst)
		ft_putstr_fd("Check your code dumbass (get_last_snip)\n", 2);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	pop_snip(t_snippet **head, t_snippet *to_pop)
{
	t_snippet	*ptr;

	if (to_pop == *head)
		*head = (*head)->next;
	else
	{
		ptr = *head;
		while (ptr->next != to_pop)
			ptr = ptr->next;
		if (!ptr)
			ft_putstr_fd("Check your code dumbass (pop_snip)\n", 2);
		ptr->next = ptr->next->next;
	}
	free(to_pop->ptr);
	free(to_pop);
}

bool	add_to_snip_lst(t_snippet **head, enum e_token token, char *ptr)
{
	t_snippet	*new;

	new = new_snip(token, ptr);
	if (!new)
		return (ft_putstr_fd("Error malloc\n", 2), false);
	if (!*head)
		*head = new;
	else
		get_last_snip(*head)->next = new;
	return (true);
}

void	insert_snip(t_snippet *node, t_snippet *to_insert)
{
	if (!node)
		ft_putstr_fd("Check your code dumbass (insert_snip)\n", 2);
	get_last_snip(to_insert)->next = node->next;
	node->next = to_insert;
}

void	free_snip_lst(t_snippet *lst)
{
	void	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->ptr);
		free(lst);
		lst = tmp;
	}
}

int	expand_in_pipe(char *str, char **env, bool one_block)
{
	int	stdout_fd;
	int	pipe_fds[2];

	stdout_fd = dup(STDOUT_FILENO);
	if (stdout_fd < 0 || pipe(pipe_fds) < 0)
		return (-1);
	if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), -1);
	expand_token(str, env, ft_strlen(str), (char)one_block);
	write(STDOUT_FILENO, "\0", 1);
	close(pipe_fds[1]);
	dup2(stdout_fd, STDOUT_FILENO);
	return (pipe_fds[0]);
}

bool	get_snips_expanded(t_snippet **new_lst, int fd)
{
	char	*str;

	str = get_next_null(fd);
	while (str)
	{
		if (*str && str[ft_strlen(str) - 1] == '\n')
			str[ft_strlen(str) - 1] = 0;
		if (!add_to_snip_lst(new_lst, word, str))
			return (free_snip_lst(*new_lst), false);
		str = get_next_null(fd);
	}
	return (true);
}

void	pop_n_insert(t_snippet **head, t_snippet *to_expand, t_snippet *new_lst)
{
	t_snippet	*tmp;

	if (*head == to_expand)
	{
		if ((*head)->next && new_lst)
		{
			insert_snip(get_last_snip(new_lst), (*head)->next);
			*head = new_lst;
		}
		else if ((*head)->next)
			*head = (*head)->next;
		else
			*head = new_lst;
	}
	else
	{
		tmp = *head;
		while (tmp->next != to_expand)
			tmp = tmp->next;
		tmp->next = to_expand->next;
		if (new_lst)
			insert_snip(tmp, new_lst);
	}
}

bool	expand_snip(t_snippet **head, t_snippet *exp, char **env, bool one_blk)
{
	t_snippet	*new_lst;
	int			fd;

	new_lst = NULL;
	if (!exp)
		return (false);
	fd = expand_in_pipe(exp->ptr, env, one_blk);
	if (!get_snips_expanded(&new_lst, fd))
		return (close(fd), false);
	close(fd);
	pop_n_insert(head, exp, new_lst);
	free(exp->ptr);
	free(exp);
	return (true);
}
