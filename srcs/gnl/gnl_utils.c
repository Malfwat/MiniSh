#include "libftprintf.h"
#include <unistd.h>

bool	closed_word(char const buffer[], char *quote_ptr, int *bracket_ptr)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (*quote_ptr)
		{
			if (buffer[i] == *quote_ptr)
				*quote_ptr = 0;
		}
		else if (!*quote_ptr)
		{
			if (buffer[i] == '\'' || buffer[i] == '\"')
				*quote_ptr = buffer[i];
			else if (buffer[i] == '(')
				(*bracket_ptr)++;
			else if (buffer[i] == ')')
				(*bracket_ptr)--;
			else if (buffer[i] == '\n' && !*bracket_ptr)
				return (true);
		}
		i++;
	}
	return (false);
}

int	read_null_terminated(int fd, char *buffer, int buffer_size)
{
	int	nb_bytes;

	if (fd < 0 || buffer_size < 1)
		return (-1);
	nb_bytes = read(fd, buffer, buffer_size);
	if (nb_bytes < 0)
		return (-1);
	buffer[nb_bytes] = 0;
	return (nb_bytes);
}

bool	add_in_list(char buffer[], t_list **head)
{
	char	*str;
	t_list	*new;

	if (!buffer[0])
		return (true);
	str = ft_strdup(buffer);
	if (!str)
		return (ft_lstclear(head, free), false);
	new = ft_lstnew(str);
	if (!new)
		return (ft_lstclear(head, free), free(str), false);
	ft_lstadd_back(head, new);
	return (true);
}
