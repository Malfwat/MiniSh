#include <stdbool.h>
#include <stdlib.h>

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
