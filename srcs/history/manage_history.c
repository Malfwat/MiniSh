/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 23:01:44 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 11:40:05 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include "get_next_line.h"
#include "libftprintf.h"
#include "minishell.h"
#include <fcntl.h>

void	trim_trailling_ws(char *str)
{
	int	i;

	i = ft_strlen(str);
	if (!i)
		return ;
	while (i > 0)
	{
		if (!is_white_space(str[i - 1]) || str[i] == 10)
			break ;
		i--;
	}
	str[i] = 0;
}

static bool	fill_history(int fd, char **ptr_oldcmd)
{
	char	*str;
	char	*ptr;
	int		len;

	if (fd < 0)
		return (false);
	str = get_next_null(fd);
	int i= 0;
	while (str && i++ < 5)
	{
		ft_printf("Debug [%s][%d]\n", str, *str);
		ptr = pass_whitespace(str);
		len = ft_strlen(ptr);
		if (len >= 1 && ptr[len - 1] == '\n')
			ptr[len - 1] = 0;
		trim_trailling_ws(ptr);
		if (*ptr && ft_strcmp(ptr, *ptr_oldcmd))
		{
			add_history(ptr);
			free(*ptr_oldcmd);
			*ptr_oldcmd = ft_strdup(ptr);
		}
		free(str);
		str = get_next_null(fd);
	}
	return (true);
}

int	ms_get_history_fd(char **ptr_oldcmd)
{
	static int	fd;
	char		*tmp;

	if (!fd)
	{
		tmp = getenv("HOME");
		if (!tmp)
			return (-1);
		tmp = ft_strsjoin((char *[]){tmp, "/", MS_HISTORY, NULL});
		if (!tmp)
			return (-1);
		fd = open(tmp, O_RDWR | O_CREAT | O_APPEND, 00666);
		free(tmp);
		if (fd < 0)
			return (-1);
		fill_history(fd, ptr_oldcmd);
	}
	return (fd);
}

void	ms_add_history(char *str, int fd, char **ptr_oldcmd)
{
	char	*ptr;

	ptr = pass_whitespace(str);
	trim_trailling_ws(ptr);
	if (!*ptr)
		return ;
	if (ft_strcmp(ptr, *ptr_oldcmd))
	{
		add_history(ptr);
		free(*ptr_oldcmd);
		*ptr_oldcmd = ft_strdup(ptr);
		if (fd < 0)
			return ;
		ft_putstr_fd(ptr, fd);
		ft_putchar_fd(0, fd);
	}
}
