/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:02:04 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/28 11:17:25 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include <stdarg.h>

int	ui_len(va_list lst, char const type)
{
	int	len;

	len = get_len(&type, va_arg(lst, unsigned int));
	if (get_rule()->precision > len)
		len = get_rule()->precision;
	return (len);
}

int	i_len(va_list lst, char const type)
{
	int	len;

	len = get_len(&type, va_arg(lst, int));
	if (get_rule()->precision > len)
		len = get_rule()->precision;
	return (len);
}

int	s_len(va_list lst, char const type)
{
	int	len;

	len = get_len(&type, va_arg(lst, char *));
	if (get_rule()->precision < len && get_rule()->precision != -1)
		len = get_rule()->precision;
	else if (get_rule()->precision == -1 && !get_rule()->dot)
		get_rule()->precision = len;
	else if (get_rule()->dot && get_rule()->precision < 1)
		len = 0;
	return (len);
}
