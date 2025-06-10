#include "minishell.h"
#include "libftprintf.h"
#include <stdint.h>

#define PRIME 53

int	_hash(char *key, int len)
{
	int			i;
	uint64_t	hash_val;

	i = 0;
	hash_val = 0;
	if (!key)
		return (0);
	while (key[i] && i < len)
	{
		hash_val = hash_val * PRIME + key[i];
		i++;
	}
	return (hash_val & (TABLE_SIZE - 1));
}

t_pair	*get_last(t_pair *lst)
{
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_pair	*get_pair(t_hash_table *table, char *key, int key_len)
{
	int	index;
	t_pair	*tmp;

	if (!key)
		return (NULL);
	index = _hash(key, key_len);
	tmp = table->bucket[index];
	while (tmp && ft_strncmp(tmp->key, key, key_len))
		tmp = tmp->next;
	return (tmp);
}

void	insert_pair(t_hash_table *table, t_pair *pair)
{
	int		index;
	t_pair	*tmp;

	if (!pair)
		return ;
	index = _hash(pair->key, pair->key_len);
	if (table->bucket[index])
		get_last(table->bucket[index])->next = pair;
	else
		table->bucket[index] = pair;
}

void	remove_pair(t_hash_table *table, t_pair *pair)
{}

void	set_pair(t_hash_table *table, t_pair *pair)
{
	t_pair	*previous;

	previous = get_pair(table, pair->key, pair->key_len);
	if (previous)
	{
		free(p)
	}
}

int	main(void)
{
	t_hash_table	table;

	ft_bzero(&table, sizeof(table));

}
