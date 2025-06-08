/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:51:39 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/28 09:49:51 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;
	char	*tmp;
	char	c2;

	c2 = (char)c;
	str = (char *)s;
	i = 0;
	tmp = NULL;
	while (str[i])
	{
		if (str[i] == c2)
			tmp = str + i;
		i++;
	}
	if (!c2)
		tmp = str + i;
	return (tmp);
}
