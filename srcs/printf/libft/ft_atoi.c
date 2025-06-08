/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:24:14 by admoufle          #+#    #+#             */
/*   Updated: 2025/04/24 11:24:17 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>

bool	is_white_space(char c)
{
	return ((c <= 13 && c >= 9) || c == 32);
}

bool	is_digit(char c)
{
	return (c <= '9' && c >= '0');
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	value;
	int	i;

	i = 0;
	value = 0;
	sign = 1;
	if (!nptr)
		return (0);
	while (is_white_space(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (is_digit(nptr[i]))
	{
		value = value * 10 + nptr[i] - 48;
		i++;
	}
	return (sign * value);
}
