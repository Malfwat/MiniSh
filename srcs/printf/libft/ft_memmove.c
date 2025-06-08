/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:08:38 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 12:08:40 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	c;
	size_t	i;

	i = 0;
	while (((long)dest) > ((long)src) && n > 0)
	{
		c = ((char *)src)[n - 1];
		((char *)dest)[n - 1] = c;
		n--;
	}
	while (((long)dest) <= ((long)src) && i < n)
	{
		c = ((char *)src)[i];
		((char *)dest)[i] = c;
		i++;
	}
	return (dest);
}
