/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   align.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:50:37 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 15:03:03 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "libft.h"
#include <stdarg.h>

void	left_align(char *str, va_list lst, int nb_char)
{
	int				tmp;
	enum e_signed	e;

	call_func(str, lst);
	e = get_rule()->prefix;
	tmp = get_rule()->width;
	tmp -= biggest(nb_char, get_rule()->precision + (e != prefix && e));
	fill(tmp, ' ');
}

void	right_align(char *str, va_list lst, int nb_char)
{
	int	tmp;

	if (get_rule()->prefix == neg && get_rule()->filling == '0')
		ft_putchar('-');
	tmp = get_rule()->width - nb_char;
	if (get_rule()->prefix == prefix)
		tmp -= 2;
	if ((*str == 'i' || *str == 'd') && get_rule()->prefix == neg)
		if (tmp == get_rule()->precision)
			tmp--;
	fill(tmp, get_rule()->filling);
	if (get_rule()->prefix == neg)
		get_rule()->prefix = none;
	call_func(str, lst);
}
