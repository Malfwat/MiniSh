/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:31:37 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/23 09:34:00 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"

/*
int	show_aliases(t_hash_table *table)
{
	int		i;
	t_pair	*ptr;

	i = 0;
	while (i < TABLE_SIZE)
	{
		ptr = table->bucket[i];
		while (ptr)
		{
			ft_putendl_fd(ptr->key, STDOUT_FILENO);
			ptr = ptr->next;
		}
		i++;
	}
	return (0);
}

int	bi_alias(int ac, char **av,int fds[2], t_ms *ms)
{
	int		i;
	char	*dup;
	t_pair	*new;

	(void)ac; (void)fds;

	if (ac == 1)
		return (show_aliases(ms->table));
	i = 0;
	while (++i < ac)
	{
		if (ft_isdigit(*av[i]) || !check_alias_chars(av[i]))
			continue ;	
		dup = ft_strdup(av[i]);
		if (!dup)
			return (1);
		new = create_pair(dup);
		if (!new)
			return (free(dup), 1);
		set_pair(ms->table, new);
	}
	return (0);
}
*/

void	parse_rc(t_hash_table *table)
{
	char	*str;
	int		len;
	int		fd;

	if (!table || !get_fd(&fd))
		return ;
	str = get_next_line(fd);
	while (str)
	{
		len = ft_strlen(str);
		if (len > 0 && str[len - 1] == '\n')
			str[len - 1] = 0;
		alias(table, str);
		free(str);
		str = get_next_line(fd);
	}
}
