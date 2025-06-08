/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:16:52 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 16:32:42 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "libftprintf.h"

int	get_modifiyer(char modifiyer[256], char *format_str)
{
	int	i;

	i = 0;
	while (!is_in_str(format_str[i], FORMAT))
	{
		modifiyer[i] = format_str[i];
		i++;
	}
	modifiyer[i] = 0;
	return (i);
}

int	format(char *str, va_list lst)
{
	char	modifiyer[256];
	va_list	save;
	int		nb_char;

	str += get_modifiyer(modifiyer, str);
	analyse_modifiyer(modifiyer);
	get_rule()->format = *str;
	va_copy(save, lst);
	nb_char = mesure_arg(str, save);
	if (get_rule()->width > nb_char || (*str == 's' && nb_char < 1))
	{
		if (get_rule()->align == left)
			left_align(str, lst, nb_char);
		else
			right_align(str, lst, nb_char);
	}
	else
		call_func(str, lst);
	return (ft_strlen(modifiyer));
}
