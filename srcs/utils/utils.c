#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void	print_until(char *str, char c, int fd)
{
	int	i;

	i = 0;
	if (!str || !*str || fd < 0)
		return ;
	while (str[i] && str[i] != c)
		i++;
	write(fd, str, i);
}

char	*_basename(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (str);
	while (str[i])
		i++;
	i--;
	while (str[i] != '/' && i > 0)
		i--;
	return (str + i + (str[i] == '/'));
}

char	*pass_whitespace(char *str)
{
	if (!str)
		return (NULL);
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

bool	is_child(pid_t pid)
{
	return (!pid);
}

int	get_exit_value(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	return (0);
}
