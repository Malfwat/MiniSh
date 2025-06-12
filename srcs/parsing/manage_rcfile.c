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

void	parse_rc(t_hash_table *table)
{
	char	*str;
	int		fd;

	if (!table || !get_fd(&fd))
		return ;
	str = get_next_line(fd);
	while (str)
	{
		alias(table, str);
		free(str);
		str = get_next_line(fd);
	}
}
