#include "minishell.h"

bool	replace_tilde(t_snippet *lst, char *home)
{
	void	*tmp;

	while (lst)
	{
		if (!ft_strcmp("~", lst->ptr))
		{
			free(lst->ptr);
			lst->ptr = ft_strdup(home);
			if (!lst->ptr)
				return (false);
		}
		else if (!ft_strncmp("~/", lst->ptr, 2))
		{
			tmp = lst->ptr;
			lst->ptr = ft_strsjoin((char *[]){home, "/", lst->ptr + 2, NULL});
			if (!lst->ptr)
				return (false);
			free(tmp);
		}
		lst = lst->next;
	}
	return (true);
}

