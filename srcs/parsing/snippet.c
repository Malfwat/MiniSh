/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   snippet.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 20:49:29 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/18 16:51:50 by malfwa           ###   ########.fr       */
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

bool	expand_snip(t_snippet **head, t_snippet *to_expand, char **env, bool one_block)
{
	int	stdout_fd;
	int	pipe_fds[2];
	t_snippet	*new_lst;


	new_lst = NULL;
	(void)head;

	stdout_fd = dup(STDOUT_FILENO);
	if (stdout_fd < 0 || pipe(pipe_fds) < 0)
		return (false);
	if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), false);
	expand_token(to_expand->ptr, env, ft_strlen(to_expand->ptr), (char)one_block);

			write(1, "\0", 1);
	close(pipe_fds[1]);
	dup2(stdout_fd, STDOUT_FILENO);


	char *str = get_next_null(pipe_fds[0]);
	while (str)
	{
		if (*str && str[ft_strlen(str) - 1] == '\n')
			str[ft_strlen(str) - 1] = 0;
		if (!add_to_snip_lst(&new_lst, word, str))
			return (free_snip_lst(new_lst), false);
		str = get_next_null(pipe_fds[0]);
	}
	close(pipe_fds[0]);


	if (!new_lst)
		ft_printf("Weird things going on\n");
	if (*head == to_expand)
	{
		if ((*head)->next && new_lst)
			insert_snip(get_last_snip(new_lst), (*head)->next);
		*head = new_lst;
	}
	else
	{
		t_snippet *tmp;
		tmp = *head;
		while (tmp->next != to_expand)
			tmp = tmp->next;
		tmp->next = to_expand->next;
		insert_snip(tmp, new_lst);
	}
	free(to_expand->ptr);
	free(to_expand);
	return (true);
}
