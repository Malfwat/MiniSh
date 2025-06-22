/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:34:45 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/22 17:36:26 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "minishell.h"

bool	dollar_n_sep(char c)
{
	if (ft_strchr(SEP, c) || c == '$')
		return (true);
	return (false);
}

bool	simple_sep(char c)
{
	if (ft_strchr(SEP, c))
		return (true);
	return (false);
}
