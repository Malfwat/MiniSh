/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:24:39 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 11:24:42 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>

void	*zero_size(void)
{
	char	*tmp;

	tmp = malloc(sizeof(char));
	if (tmp)
		*tmp = 0;
	return (tmp);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	max_int;
	char	*tmp;
	size_t	i;
	size_t	value_asked;

	max_int = INT_MAX;
	i = 0;
	if (nmemb == 0 || size == 0)
		return (zero_size());
	if (nmemb > max_int / size || size > max_int / nmemb)
		return (NULL);
	value_asked = nmemb * size;
	tmp = malloc(value_asked);
	if (!tmp)
		return (NULL);
	while (i < value_asked)
		tmp[i++] = 0;
	return ((void *)tmp);
}
