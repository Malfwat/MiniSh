/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:15:22 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/06 15:15:23 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strslen(char **tab)
{
	int	i;
	int	j;
	int	len;

	j = 0;
	len = 0;
	if (!tab)
		return (0);
	while (tab[j])
	{
		i = 0;
		while (tab[j][i])
		{
			len++;
			i++;
		}
		j++;
	}
	return (len);
}

char	*ft_strsjoin(char **strs)
{
	char	*str;
	int		len;
	int		i;
	int		j;

	len = ft_strslen(strs);
	if (!len)
		return (NULL);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	j = -1;
	len = 0;
	while (strs[++j])
	{
		i = -1;
		while (strs[j][++i])
			str[len++] = strs[j][i];
	}
	str[len] = 0;
	return (str);
}
