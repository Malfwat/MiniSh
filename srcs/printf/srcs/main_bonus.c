/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 04:45:13 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 16:31:51 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include "libftprintf.h"
#include "libft.h"
#include <stdio.h>

int	ft_printf(char *str, ...)
{
	va_list	lst;
	int		i;
	int		len;

	i = 0;
	if (!str)
		return (-1);
	va_start(lst, str);
	while (str[i] && *get_nb_printed() >= 0)
	{
		if (str[i] == '%')
		{
			i++;
			i += format(&str[i], lst);
		}
		else
			ft_putchar(str[i]);
		i++;
	}
	va_end(lst);
	len = *get_nb_printed();
	*get_nb_printed() = 0;
	return (len);
}
