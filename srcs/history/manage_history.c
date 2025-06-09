#include <readline/history.h>
#include "get_next_line.h"
#include "libftprintf.h"
#include "minishell.h"
#include <fcntl.h>

void	trim_trailling_ws(char *str)
{
	int	i;

	i = ft_strlen(str);
	if (!i)
		return ;
	while (i > 0)
	{
		if (str[i - 1] != ' ' && str[i - 1] != '\t')
			break ;
		i--;
	}
	str[i] = 0;
}

static bool	fill_history(int fd)
{
	char	*str;
	char	*ptr;
	int		len;

	if (fd < 0)
		return (false);
	str = get_next_line(fd);
	while (str)
	{
		ptr = pass_whitespace(str);
		if (*ptr)
		{
			len = ft_strlen(ptr);
			if (len >= 1 && ptr[len - 1] == '\n')
				ptr[len - 1] = 0;
			trim_trailling_ws(ptr);
			add_history(ptr);
		}
		free(str);
		str = get_next_line(fd);
	}
	return (true);
}

int	ms_get_history_fd(void)
{
	static int	fd;
	char		*tmp;

	if (!fd)
	{
		tmp = getenv("HOME");
		if (!tmp)
			return (-1);
		tmp = ft_strsjoin((char *[]){tmp, "/", ".ms_history", NULL});
		if (!tmp)
			return (-1);
		fd = open(tmp, O_RDWR | O_CREAT | O_APPEND, 00666);
		free(tmp);
		if (fd < 0)
			return (-1);
		fill_history(fd);
	}
	return (fd);
}

void	ms_add_history(char *str, int fd, char **ptr_oldcmd)
{
	char	*ptr;

	ptr = pass_whitespace(str);
	if (!*ptr)
		return ;
	trim_trailling_ws(ptr);
	if (ft_strcmp(ptr, *ptr_oldcmd))
	{
		ft_printf("history [%s]\n", ptr);
		add_history(ptr);
		free(*ptr_oldcmd);
		*ptr_oldcmd = ft_strdup(ptr);
		if (fd < 0)
			return ;
		ft_putendl_fd(ptr, fd);
	}
}
