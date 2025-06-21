/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 23:01:02 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/21 23:01:04 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_cwd(t_prompt *prompt, int fd)
{
	int		len;
	char	*home;

	home = getenv("HOME");
	len = ft_strlen(home);
	if (!ft_strncmp(prompt->cwd, home, len))
	{
		ft_putstr_fd("~", fd);
		ft_putstr_fd(prompt->cwd + len, fd);
	}
	else
		ft_putstr_fd(prompt->cwd, fd);
}

int	write_prompt_elem(t_prompt *prompt, char c, int fd)
{
	if (c == 'h')
		print_until(prompt->hostname, '.', fd);
	else if (c == 'H')
		ft_putstr_fd(prompt->hostname, fd);
	else if (c == 'u')
		ft_putstr_fd((char *)prompt->user, fd);
	else if (c == 'w')
		write_cwd(prompt, fd);
	else if (c == 'W')
		ft_putstr_fd((char *)prompt->cwd_basename, fd);
	else if (c == '$')
		ft_putchar_fd('$', fd);
	return (1);
}

void	get_prompt(t_prompt *prompt, int pipe_fds[2])
{
	int	i;

	i = 0;
	while (prompt->prompt_raw[i])
	{
		if (prompt->prompt_raw[i] == '\\')
			i += write_prompt_elem(prompt,
					prompt->prompt_raw[i + 1], pipe_fds[1]);
		else
			ft_putchar_fd(prompt->prompt_raw[i], pipe_fds[1]);
		i++;
	}
}
