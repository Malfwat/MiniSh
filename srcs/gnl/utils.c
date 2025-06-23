/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:59:50 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 10:04:37 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	read_null_terminated(int fd, char *buffer, int buffer_size)
{
	int	nb_bytes;

	if (fd < 0 || buffer_size < 1)
		return (-1);
	nb_bytes = read(fd, buffer, buffer_size);
	if (nb_bytes < 0)
		return (-1);
	buffer[nb_bytes] = 0;
	return (nb_bytes);
}

void	empty_gnl(int fd)
{
	void	*ptr;

	if (fd < 0)
		return ;
	ptr = get_next_line(fd);
	while (ptr)
	{
		free(ptr);
		ptr = get_next_line(fd);
	}
}
