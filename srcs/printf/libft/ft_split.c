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

static int	ft_count_word(char const *s, char c)
{
	unsigned int	i;
	unsigned int	x;

	x = 0;
	i = -1;
	while (s && s[++i])
		if ((s[i] != c && s[i + 1] == c) || (s[i] != c && s[i + 1] == 0))
			x++;
	return (x);
}

static void	*ft_free(char **tab, unsigned int x)
{
	unsigned int	i;

	i = 0;
	while (x > i)
		free(tab[i++]);
	free(tab);
	return (NULL);
}

static int	ft_fill_tab(char **tab, char const *s, unsigned int *i, char c)
{
	int	j;
	int	len;

	while (s && s[(*i)] == c && s[(*i)] != 0)
		(*i)++;
	j = 0;
	len = 0;
	while (s && s[(*i + len)] && s[(*i + len)] != c)
		len++;
	*tab = malloc(sizeof(char) * (len + 1));
	if (*tab == NULL)
		return (1);
	while (s && s[(*i)] && s[(*i)] != c)
		(*tab)[j++] = s[(*i)++];
	(*tab)[j] = 0;
	while (s && s[(*i)] == c && s[(*i)] != 0)
		(*i)++;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	unsigned int	count;
	unsigned int	i;
	int				len;
	unsigned int	x;
	char			**tab;

	count = ft_count_word(s, c);
	tab = malloc(sizeof(char *) * (count + 1));
	if (tab == NULL)
		return (NULL);
	tab[count] = NULL;
	i = 0;
	x = 0;
	while (s && s[i] && x != count)
	{
		len = 0;
		while (s[i + len] != c && s[i + len] != 0)
			len++;
		if (ft_fill_tab(&(tab[x]), s, &i, c) == 1)
			return (ft_free(tab, x));
		x++;
	}
	return (tab);
}
