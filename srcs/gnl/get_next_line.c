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

#include "get_next_line.h"

void	cpy(char **stash, char **line, t_buf *tmp)
{
	int	i;
	int	j;

	i = 0;
	while (tmp != NULL)
	{
		j = 0;
		while (tmp->str && (tmp->str)[j] && (tmp->str)[j] != '\n')
			(*line)[i++] = tmp->str[j++];
		if (tmp->str[j] == '\n')
			(*line)[i++] = tmp->str[j++];
		if (tmp->next == NULL)
			break ;
		tmp = tmp->next;
	}
	(*line)[i] = 0;
	if (tmp && tmp->str[j] != 0)
		ft_strlcpy(*stash, &(tmp->str)[j], BUFFER_SIZE + 1);
	else
	{
		free(*stash);
		*stash = NULL;
	}
}

int	join_t_buf(t_buf **begin, char **stash, char **line)
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

int	check_stash(char **stash, char	**line, char *new_stash, int i)
{
	while (*stash && (*stash)[i] && (*stash)[i] != '\n')
		i++;
	if (i == 0 || !(*stash) || (*stash)[i] == 0)
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

char	*get_line(int fd, char *stash, char **line, t_buf *lst)
{
	int	nb_read;

	nb_read = 1;
	if (stash[0] != 0)
	{
		if (check_stash(&stash, line, NULL, 0) == -1)
			return (NULL);
		if (*line)
			return (stash);
		if (new_elem_back(&lst, stash) == -1)
			return (NULL);
	}
	while (nb_read > 0 && in_str(stash, '\n') == -1)
	{
		nb_read = read(fd, stash, BUFFER_SIZE);
		stash[(nb_read >= 0) * nb_read] = 0;
		if (nb_read > 0 && new_elem_back(&lst, stash) == -1)
			return (NULL);
		if (nb_read <= 0)
		{
			free(stash);
			stash = NULL;
		}
	}
	return (join_t_buf(&lst, &stash, line), stash);
}

char	*get_next_line(int fd)
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
