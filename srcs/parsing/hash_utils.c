#include "minishell.h"
#include <stdlib.h>

void	remove_pair(t_hash_table *table, t_pair *pair)
{
	int		index;
	t_pair	*tmp;

	index = _hash(pair->key, pair->len);
	if (table->bucket[index] == pair)
		table->bucket[index] = table->bucket[index]->next;
	else
	{
		tmp = table->bucket[index];
		while (tmp && tmp->next && tmp->next != pair)
			tmp = tmp->next;
		if (tmp && tmp->next == pair)
			tmp->next = tmp->next->next;
	}
	free(pair->key);
	free(pair);
}

void	free_table(t_hash_table *table)
{
	int	i;

	i = 0;
	while (i < TABLE_SIZE)
	{
		while (table->bucket[i])
			remove_pair(table, table->bucket[i++]);
		i++;
	}
}
