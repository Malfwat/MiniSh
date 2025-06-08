/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:08:42 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/28 11:16:48 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	get_arg(va_list ap, char const type, long long int *nb, int *base_len)
{
	if (type == 'p')
		*nb = va_arg(ap, unsigned long int);
	else
		*nb = (long int)va_arg(ap, unsigned int);
	if (type != 'u')
		*base_len = 16;
	if (type == 'p' && !*nb)
		return (va_end(ap), 3);
	if (type == 'x' || type == 'X' || type == 'u')
	{
		if (!*nb && get_rule()->precision < 1 && get_rule()->dot)
			return (va_end(ap), 0);
	}
	return (-1);
}

int	get_s_len(va_list ap)
{
	char	*str;

	str = va_arg(ap, char *);
	if ((int)ft_strlen(str) < get_rule()->precision && str)
		get_rule()->precision = ft_strlen(str);
	return ((int []){ft_strlen(str), 6}[!str]);
}

int	p_size(unsigned long int tmp, int base_len)
{
	int	len;

	len = 0;
	while (tmp && ++len)
		tmp /= base_len;
	return (len);
}

int	get_len(char const *type, ...)
{
	va_list			ap;
	int				len;
	int				base_len;
	long long int	nb;

	va_start(ap, type);
	nb = 0;
	if (*type == 's')
		return (get_s_len(ap));
	base_len = 10;
	if (*type == 'n')
		nb = get_number(ap);
	else
	{
		len = get_arg(ap, *type, &nb, &base_len);
		if (len >= 0)
			return (len);
	}
	if (get_rule()->dot && get_rule()->precision < 1 && !nb)
		return (va_end(ap), 0);
	len = arg_size(nb, base_len, *type);
	va_end(ap);
	return (len + ((get_rule()->prefix != prefix) && get_rule()->prefix));
}
