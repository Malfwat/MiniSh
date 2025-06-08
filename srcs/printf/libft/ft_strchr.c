/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:49:24 by malfwa            #+#    #+#             */
/*   Updated: 2022/11/15 16:17:18 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*str;
	char	a;

	a = (char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == a)
			return (&(str[i]));
		i++;
	}
	if (a == '\0')
		return (&(str[i]));
	return (NULL);
}
