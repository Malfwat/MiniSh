/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:03:59 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 13:04:01 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	find_len(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	ft_is_in_set(char const *set, char c)
{
	unsigned int	i;

	i = -1;
	while (set && set[++i])
		if (c == set[i])
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	x;
	unsigned int	j;
	char			*str;

	x = find_len(s1) - 1;
	i = 0;
	while (ft_is_in_set(set, s1[i]) == 1)
		i++;
	if (s1[i] == 0)
		return (ft_calloc(1, 1));
	while (ft_is_in_set(set, s1[x]) == 1)
		x--;
	x -= i - 1;
	str = malloc(sizeof(char) * (x + 1));
	if (str == NULL)
		return (NULL);
	str[x] = 0;
	j = 0;
	while (s1 && s1[i] && j < x)
	{
		str[j] = s1[i + j];
		j++;
	}
	return (str);
}
