#include "minishell.h"

static bool	replace(t_snippet **head, t_snippet *snip, t_hash_table *table)
{
	t_pair		*pair;
	t_snippet	*new_lst;

	(void)head;
	(void)new_lst;

	pair = get_pair(table, snip->ptr, ft_strlen(snip->ptr));
	if (!pair)
		return (true);
	free(snip->ptr);
	snip->ptr = ft_strdup(pair->value);
	if (!snip->ptr)
		return (false);

	new_lst = lexer(pair->value);
	// re-lexer le contenu de lst->ptr
	// re-appeler pour les noms de fichier et redirs
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
