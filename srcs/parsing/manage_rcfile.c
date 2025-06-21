/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_rcfile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 23:00:00 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/21 23:00:02 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"

bool	get_fd(int *fd)
{
	char	*tmp;

	tmp = getenv("HOME");
	if (!tmp)
		return (false);
	tmp = ft_strsjoin((char *[]){tmp, "/", MS_RC, NULL});
	if (!tmp)
		return (false);
	*fd = open(tmp, O_RDONLY);
	free(tmp);
	if (*fd < 0)
		return (false);
	return (true);
}

bool	alias_open(char *str)
{
	char quote;
	int	bracket;

	quote = 0;
	bracket = 0;

	if (!str)
		return (false);
	while (*str)
	{
		if (quote == '\'' && *str == '\'')
			quote = '\0';
		else if (quote == '\"' && *str == '\"')
			quote = '\0';
		else if (!quote && (*str == '\'' || *str == '\"'))
			quote = *str;
		else if (!quote)
		{
			if (*str == '(')
				bracket++;
			else if (*str == ')')
				bracket--;
		}
		str++;
	}
	return (quote || bracket > 0);
}

bool	check_alias_chars(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (str[i] != '=' || alias_open(str + i + 1))
		return (false);
	return (true);
}

bool	is_alias_cmd(char *str, char **ptr)
{
	if (ft_strncmp(str, ALIAS, ALIAS_LEN))
		return (false);
	str += ALIAS_LEN;
	str = pass_whitespace(str);
	if (ft_isdigit(*str) || !check_alias_chars(str))
		return (false);
	*ptr = ft_strdup(str);
	if (!*ptr)
		return (false);
	return (true);
}

void	alias(t_hash_table *table, char *str)
{
	char	*ptr;
	t_pair	*new;
	
	if (is_alias_cmd(str, &ptr))
	{
		new = create_pair(ptr);
		if (!new)
			return ;
		set_pair(table, new);
	}
}
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
