/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 22:19:30 by amouflet          #+#    #+#             */
/*   Updated: 2022/12/03 15:30:10 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 511
# endif /*BUFFER_SIZE*/

# if (BUFFER_SIZE < 0)
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 0
# endif

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stddef.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_stash
{
	char			*str;
	struct s_stash	*next;
}	t_buf;

char	*get_next_line(int fd);
char	*get_next_null(int fd);
bool	is_line_terminated(char *line, int i);
//char	*get_line(int fd, char *stash, char **line, t_buf *lst);
//int		check_stash(char **stash, char	**line, char *new_stash, int i);
//int		join_t_buf(t_buf **begin, char **stash, char **line);
int		new_elem_back(t_buf **begin, char *buf);
int		in_str(char *str, char c);
int		free_lst(t_buf **lst);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	empty_gnl(int fd);
char	*gnl(int fd);

#endif /*GET_NEXT_LINE_H*/
