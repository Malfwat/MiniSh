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

t_pair	*get_pair(t_hash_table *table, char *key, int len)
{
	int		index;
	t_pair	*tmp;

	if (!key)
		return (NULL);
	index = _hash(key, len);
	tmp = table->bucket[index];
	while (tmp && ft_strncmp(tmp->key, key, len))
		tmp = tmp->next;
	return (tmp);
}

void	insert_pair(t_hash_table *table, t_pair *pair)
{
	int		index;

	if (!pair)
		return ;
	index = _hash(pair->key, pair->len);
	if (table->bucket[index])
		pair->next = table->bucket[index];
	else
		table->bucket[index] = pair;
}

void	set_pair(t_hash_table *table, t_pair *pair)
{
	t_pair	*previous;

	previous = get_pair(table, pair->key, pair->len);
	if (previous)
		remove_pair(table, previous);
	insert_pair(table, pair);
}

t_pair	*create_pair(char *str)
{
	t_pair	*pair;
	char	*ptr;

	if (!str || !ft_strchr(str, '='))
		return (NULL);
	pair = malloc(sizeof(t_pair));
	if (!pair)
		return (NULL);
	pair->next = NULL;
	pair->key = str;
	ptr = ft_strchr(str, '=');
	pair->len = ptr - str;
	pair->value = ptr + 1;
	*ptr = 0;
	return (pair);
}
