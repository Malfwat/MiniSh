/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_size_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admoufle <admoufle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:06:41 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 16:32:04 by admoufle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void	get_sizes(t_modif *rule, char const *str, int *i)
{
	if (str[*i] >= '0' && str[*i] <= '9')
	{
		if (rule->dot)
			rule->precision = get_val(str, i);
		else
		{
			rule->width = get_val(str, i);
			if (!rule->align)
				rule->align = right;
		}
		(*i)--;
	}
}
