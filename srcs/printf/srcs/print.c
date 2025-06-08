/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:08:52 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 10:24:23 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_printnbr(unsigned long int nb, unsigned int base_len, char *base)
{
	static int	loop;
	int			size;

	if (++loop && nb >= base_len)
		ft_printnbr(nb / base_len, base_len, base);
	if (loop)
	{
		size = get_rule()->precision - loop;
		fill(size, '0');
		loop = 0;
	}
	ft_putchar(base[nb % base_len]);
	return (0);
}

int	ft_puthexa(long long int nb, enum e_case casee)
{
	char	*base;

	if (get_rule()->dot && !nb && get_rule()->precision < 1)
		return (0);
	if (get_rule()->prefix == prefix && nb)
	{
		ft_putchar('0');
		ft_putchar((char []){'x', 'X'}[casee]);
	}
	base = (char *[]){"0123456789abcdef", "0123456789ABCDEF"}[casee];
	return (ft_printnbr(nb, 16, base));
}

int	ft_putnbr(int nb)
{
	int	tmp;

	if (get_rule()->dot && !nb && get_rule()->precision < 1)
		return (0);
	if (nb < 0)
	{
		if (get_rule()->filling == ' ' || get_rule()->prefix == neg)
			ft_putchar('-');
		return (ft_printnbr(-1 * (long)nb, 10, "0123456789"));
	}
	tmp = get_rule()->prefix;
	if (tmp == wspace)
		ft_putchar(' ');
	else if (tmp == pos)
		ft_putchar('+');
	return (ft_printnbr(nb, 10, "0123456789"));
}

void	print_addr(unsigned long addr)
{
	if (!addr)
		return ((void)ft_putstr("(nil)"));
	ft_putstr("0x");
	ft_puthexa(addr, lower - 1);
}

int	putstr_size(char *str, int size)
{
	int	i;

	if (!str && get_rule()->precision > 5)
		return (ft_putstr(str));
	i = 0;
	while (i < size && str && str[i])
		ft_putchar(str[i++]);
	return (i);
}
