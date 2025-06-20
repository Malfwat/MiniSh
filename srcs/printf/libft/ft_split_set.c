/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 20:33:18 by amouflet          #+#    #+#             */
/*   Updated: 2022/11/15 15:06:46 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static int	ft_count_word(char const *s, char *set)
{
	int	i = 0;
	int	count = 0;

	if (!s)
		return (0);
	while (s[i])
	{
		while (s[i] && ft_strchr(set, s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && !ft_strchr(set, s[i]))
			i++;
	}
	return (count);
}

static int	ft_fill_tab(char **tab, char const *s, unsigned int *i, char *set)
{
	int	j;
	int	len;

	while (s && ft_strchr(set, s[(*i)]) && s[(*i)] != 0)
		(*i)++;
	j = 0;
	len = 0;
	while (s && s[(*i + len)] && !ft_strchr(set, s[(*i + len)]))
		len++;
	*tab = malloc(sizeof(char) * (len + 1));
	if (*tab == NULL)
		return (1);
	while (s && s[(*i)] && !ft_strchr(set, s[(*i)]))
		(*tab)[j++] = s[(*i)++];
	(*tab)[j] = 0;
	return (0);
}

char	**ft_split_set(char const *s, char *set)
{
	unsigned int	count;
	unsigned int	i;
	int				len;
	unsigned int	x;
	char			**tab;

	if (!s || !set)
		return (NULL);
	count = ft_count_word(s, set);
	tab = malloc(sizeof(char *) * (count + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	x = 0;
	while (s[i] && x != count)
	{
		len = 0;
		while (ft_strchr(set, s[i + len]) && s[i + len] != 0)
			len++;
		if (ft_fill_tab(&(tab[x]), s, &i, set) == 1)
			return (ft_free(tab));
		x++;
	}
	tab[x] = NULL;
	return (tab);
}
