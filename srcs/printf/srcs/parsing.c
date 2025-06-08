/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:15:34 by malfwa            #+#    #+#             */
/*   Updated: 2025/04/29 15:48:15 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

t_modif	initialize_t_modif(void)
{
	t_modif	new;

	new.dot = false;
	new.filling = 32;
	new.precision = -1;
	new.width = -1;
	new.prefix = none;
	new.align = 0;
	return (new);
}

void	analyse(t_modif *rule, char const *str, int *i)
{
	int	tmp;

	tmp = is_in_str(str[*i], FLAG) - 1;
	if (tmp == minus)
		rule->align = left;
	else if (tmp == plus)
		rule->prefix = pos;
	else if (tmp == space)
		rule->prefix = wspace;
	else if (tmp == hash)
		rule->prefix = prefix;
	else if (tmp == zero)
	{
		if (!rule->dot)
			rule->filling = '0';
		else
			get_sizes(rule, str, i);
	}
	else if (tmp == dot)
	{
		rule->dot = true;
		rule->filling = ' ';
	}
	else
		get_sizes(rule, str, i);
}

t_modif	analyse_modifiyer(char const *str)
{
	t_modif	rule;
	int		i;

	i = 0;
	rule = initialize_t_modif();
	while (str[i])
	{
		analyse(&rule, str, &i);
		i++;
	}
	*get_rule() = rule;
	return (rule);
}

int	mesure_arg(char const *str, va_list lst)
{
	int	tmp;

	tmp = is_in_str(*str, FORMAT) - 1;
	if (tmp == c)
		return (1);
	if (tmp == s)
		return (s_len(lst, 's'));
	if (tmp == p)
		return (get_len("p", va_arg(lst, unsigned long)) + 2);
	if (tmp == d)
		return (i_len(lst, 'n'));
	if (tmp == i)
		return (i_len(lst, 'n'));
	if (tmp == u)
		return (ui_len(lst, 'u'));
	if (tmp == x)
		return (ui_len(lst, 'x'));
	if (tmp == X)
		return (ui_len(lst, 'x'));
	if (tmp == percent)
		return (1);
	return (-1);
}
