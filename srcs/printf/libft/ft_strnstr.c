/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:44:34 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 12:50:52 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	x;

	i = 0;
	x = 0;
	if (!little || !*little)
		return ((char *)big);
	if (!big || !len)
		return (NULL);
	while (big[i + x] && x + i < len && little[x])
	{
		if (big[i + x] == little[x])
			x++;
		else
		{
			x = 0;
			i++;
		}
	}
	if (!little[x])
		return ((char *)big + i);
	return (NULL);
}
