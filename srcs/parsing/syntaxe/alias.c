#include "minishell.h"

char	*str_without_quote(char *str)
{
	char	*new_str;
	char	quote;
	int		i;

	if (!str || !*str)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (false);
	i = 0;
	while (*str)
	{
		while (*str && *str != '\'' && *str != '"')
			new_str[i++] = *(str++);
		if (!*str)
			break ;
		quote = *(str++);
		while (*str != quote)
			new_str[i++] = *(str++);
		str++;
	}
	new_str[i] = 0;
	return (new_str);
}

static bool	replace(t_snippet **head, t_snippet *snip, t_hash_table *table)
{
	t_pair		*pair;
	t_snippet	*new_lst;
	char		*tmp;

	(void)head;
	(void)new_lst;

	pair = get_pair(table, snip->ptr, ft_strlen(snip->ptr));
	if (!pair)
		return (true);
	free(snip->ptr);
	snip->ptr = ft_strdup(pair->value);
	if (!snip->ptr)
		return (false);
	tmp = str_without_quote(pair->value);
	if (!tmp)
		return (false);
	new_lst = lexer(tmp);
	free(tmp);
	if (!new_lst)
		return (false);
	insert_snip(snip, new_lst);
	pop_snip(head, snip);
	optimize_lst(head);
	return (true);
}

// a appeler apres avoir fait le changement pour les redir et les noms de fichiers

bool	replace_aliases(t_snippet **head, t_hash_table *table)
{
	t_snippet	*lst;
	int			bracket;
	int			prev;

	bracket = 0;
	prev = -1;
	lst = *head;
	while (lst)
	{
		if (lst->token == open_par)
			bracket++;
		else if (lst->token == closing_par)
			bracket--;
		if (lst->token == word && !bracket && (prev == -1 || is_cntl_op(prev)))
			if (!replace(head, lst, table))
				return (false);
		prev = lst->token;
		lst = lst->next;
	}
	return (true);
}
