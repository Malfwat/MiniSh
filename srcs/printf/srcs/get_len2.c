/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:23:14 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/08 15:08:00 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	get_number(va_list ap)
{
	int	nb;

	nb = va_arg(ap, int);
	if (nb < 0)
		get_rule()->prefix = neg;
	return (nb);
}

int	arg_size(long long int nb, int base_len, char const type)
{
	int	len;

	len = (!nb);
	if (type == 'p')
		len = p_size(nb, base_len);
	else
		while (nb && ++len)
			nb /= base_len;
	return (len);
}

int	s_c(int tmp)
{
	return ((int []){lower, upper}[tmp - x] - 1);
}

void	call_func(char *str, va_list lst)
{
	unsigned int	nb;
	int				tmp;

	tmp = is_in_str(*str, FORMAT) - 1;
	if (tmp == c)
		ft_putchar(va_arg(lst, int));
	else if (tmp == s)
		putstr_size(va_arg(lst, char *), get_rule()->precision);
	else if (tmp == p)
		print_addr(va_arg(lst, unsigned long));
	else if (tmp == d)
		ft_putnbr(va_arg(lst, int));
	else if (tmp == i)
		ft_putnbr(va_arg(lst, int));
	else if (tmp == u)
	{
		nb = va_arg(lst, unsigned int);
		if (!(!nb && get_rule()->dot && get_rule()->precision < 1))
			ft_printnbr(nb, 10, "0123456789");
	}
	else if (tmp == x || tmp == X)
		ft_puthexa(va_arg(lst, unsigned int), s_c(tmp));
	else if (tmp == percent)
		ft_putchar('%');
}
