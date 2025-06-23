/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_null.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 22:18:56 by amouflet          #+#    #+#             */
/*   Updated: 2025/06/23 11:36:45 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static void	cpy(char **stash, char **line, t_buf *tmp)
{
	int	i;
	int	j;

	i = 0;
	while (tmp != NULL)
	{
		j = 0;
		while (tmp->str && tmp->str[j] && j < BUFFER_SIZE)
			(*line)[i++] = tmp->str[j++];
		if (j != BUFFER_SIZE)
			break ;
		tmp = tmp->next;
	}
	(*line)[i] = 0;
	if (tmp && j < BUFFER_SIZE && tmp->str[j] == 0)
	{
		i = 0;
		while (++j < BUFFER_SIZE)
			(*stash)[i++] = tmp->str[j];
		while (i < BUFFER_SIZE)
			(*stash)[i++] = 0;
	}
	else
	{
		free(*stash);
		*stash = NULL;
	}
}

static int	join_t_buf(t_buf **begin, char **stash, char **line)
{
	t_buf	*tmp;
	int		i;

	i = 0;
	tmp = *begin;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		i++;
	}
	tmp = *begin;
	if (i == 0)
		return (0);
	*line = malloc(sizeof(char) * (BUFFER_SIZE * i + 1));
	if (!*line)
	{
		line = NULL;
		free(*stash);
		stash = NULL;
		return (free_lst(begin), -1);
	}
	cpy(stash, line, tmp);
	return (free_lst(begin));
}

static int	check_stash(char **stash, char	**line, char *new_stash, int i)
{
	while (*stash && (*stash)[i] && i < BUFFER_SIZE)
		i++;
	if (!i || !(*stash) || i == BUFFER_SIZE)
		return (0);
	*line = malloc(sizeof(char) * (i + 2));
	if (!*line)
		return (free(*stash), -1);
	ft_strlcpy(*line, *stash, i + 2);
	if ((*stash)[i + 1] == 0)
	{
		free(*stash);
		*stash = NULL;
		return (1);
	}
	new_stash = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!new_stash)
		return (free(*line), free(*stash), -1);
	ft_strlcpy(new_stash, &((*stash)[i + 1]), BUFFER_SIZE + 1);
	free(*stash);
	*stash = new_stash;
	return (1);
}

static char	*get_line(int fd, char *stash, char **line, t_buf *lst)
{
	int	nb_read;
	int	go;

	nb_read = 1;
	if (*stash)
	{
		if (check_stash(&stash, line, NULL, 0) == -1)
			return (NULL);
		if (*line)
			return (stash);
		if (new_elem_back(&lst, stash) == -1)
			return (NULL);
	}
	go = 0;
	while (nb_read > 0 && (in_str_len(stash, '\0', BUFFER_SIZE) == -1 || !go++))
	{
		nb_read = read_null_terminated(fd, stash, BUFFER_SIZE);
		if (nb_read < 0 || new_elem_back(&lst, stash) == -1)
			return (NULL);
		if (!nb_read)
			stash = ft_free_null(stash);
	}
	return (join_t_buf(&lst, &stash, line), stash);
}

char	*get_next_null(int fd)
{
	static char	*stash_tab[4096];
	char		*line;

	line = NULL;
	if (BUFFER_SIZE <= 0 || fd >= 4096 || fd < 0 || BUFFER_SIZE > INT_MAX - 1)
		return (NULL);
	if (!stash_tab[fd])
	{
		stash_tab[fd] = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!stash_tab[fd])
			return (NULL);
		stash_tab[fd][0] = 0;
	}
	stash_tab[fd] = get_line(fd, stash_tab[fd], &line, NULL);
	return (line);
}
