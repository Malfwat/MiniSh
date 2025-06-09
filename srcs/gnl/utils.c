/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:59:50 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/09 22:02:18 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
