
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "get_next_line.h"
#include "minishell.h"
#include "libftprintf.h"

bool	is_opened(char *str)
{
	static char quote;
	static int	bracket;

	if (!str)
		return (false);
	while (*str)
	{
		if (quote == '\'' && *str == '\'')
			quote = '\0';
		else if (quote == '\"' && *str == '\"')
			quote = '\0';
		else if (!quote && (*str == '\'' || *str == '\"'))
			quote = *str;
		else if (!quote)
		{
			if (*str == '(')
				bracket++;
			else if (*str == ')')
				bracket--;
		}
		str++;
	}
	return (quote || bracket > 0);
}


int	closing_match(char *ptr);

int	find_closing_bracket(char *opening_bracket)
{
	char	*ptr;
	int		i;

	ptr = opening_bracket;
	i = 1;
	while (ptr[i] && ptr[i] != ')')
	{
		if (ptr[i] == '\'' || ptr[i] == '"')
			i += closing_match(ptr + i);
		else if (ptr[i] == '(')
			i += find_closing_bracket(ptr + i);
		i++;
	}
	return (i);
}

int	closing_match(char *ptr)
{
	if (*ptr == '\'' || *ptr == '"')
		return (ft_strchr(ptr + 1, *ptr) - ptr);
	return (find_closing_bracket(ptr));
}

int	word_len(char *str)
{
	int		i;
	i = 0;
	while (str[i])
	{
		if (!i && ft_strchr(SEP, str[i]))
		{
			if (*str == '(' || *str == ')' || *str == ';')
				return (1);
			if (*str == '<' || *str == '>' || *str == '|')
				return ((int []){1, 2}[str[0] == str[1]]);
			i++;
		}
		while (str[i] && !ft_strchr(SEP, str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || ft_strchr(SEP, str[i]))
			return (i);
		if (ft_strchr(OPENER, str[i]) && *str == '$')
			return (i);
		i += closing_match(str + i) + 1;
	}
	return (i);
}

enum e_token get_token(char *str)
{
	if (!ft_strncmp(str, "<<", 2))
		return (here_doc);
	if (*str == '<')
		return (redir_in);
	if (!ft_strncmp(str, ">>", 2))
		return (append);
	if (*str == '>')
		return (redir_out);
	if (!ft_strncmp(str, "||", 2))
		return (or);
	if (*str == '|')
		return (pipe_delim);
	if (!ft_strncmp(str, "&&", 2))
		return (and);
	if (*str == ';')
		return (semicolon);
	if (*str == '(')
		return (open_par);
	if (*str == ')')
		return (closing_par);
	return (word);
}

t_snippet	*lexer(char *str, t_hash_table *table)
{
	int			len;
	t_snippet	*lst;
	char		*dup;

	(void)table;

	if (!str)
		return (NULL);
	lst = NULL;
	while (*str)
	{
		len = word_len(str);
		dup = ft_strndup(str, len);
		if (!dup || !add_to_snip_lst(&lst, get_token(dup), dup))
			return (free_snip_lst(lst), NULL);
		str += len;
		str = pass_whitespace(str);
	}
	return (lst);
}


//#include <stdio.h>

// Convertit le token en une chaîne lisible
const char *token_to_str(enum e_token token)
{
	switch (token)
	{
		case word:         return "word";
		case redir_in:     return "redir_in";
		case redir_out:    return "redir_out";
		case here_doc:     return "here_doc";
		case append:       return "append";
		case pipe_delim:   return "pipe_delim";
		case or:           return "or";
		case and:          return "and";
		case semicolon:    return "semicolon";
		case open_par:     return "open_par";
		case closing_par:  return "closing_par";
		default:           return "unknown";
	}
}

int	ft_strncmp_weq(char *name, char *env_var, size_t n)
{
	while (n && *name)
	{
		if (*name != *env_var)
			return (1);
		--n;
		++name;
		++env_var;
	}
	if (!n && *env_var == '=')
		return (0);
	return (1);
}

char	**expand(char **env, char *var_name, int len)
{
	static char	set[] = {'\t', '\v', '\n', '\r', '\f', ' ', 0};
	int	i;

	i = 0;
	while (env[i] && ft_strncmp_weq(var_name, env[i], len))
		i++;
	if (env[i])
		return (ft_split_set(env[i] + len + 1, set));
	return (NULL);
}

int	dollar_len(char *str)
{
	int	i;

	if (!str || *str != '$')
		return (0);
	i = 1;
	if (str[1] == '$')
		return (2);
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

// Affiche la liste chaînée
void print_snippet_list(t_snippet *head, char **env)
{
	char	**tab;
	char	*ptr;
	int		len;
	int i = 0;
	int	counter;

	// Je dois etre capable de detecter lorsque les var sont entre single quote ou double quote et je ne dois pas print les quotes a l'exterieur
	// creer les nouveau snippet apres le split si necessaire
	// join apres le split si necessaire et bien free
	// Je peux surement faire un fork avec des sous snip ecrire dans un fork split avec un genre de get next word comme je le fais actuellement et ajouter les snip en les inserant;
	while (head)
	{
		ft_printf("Node %d:\n", i++);
		ft_printf("  Token : %-13s\n", token_to_str(head->token));
		ft_printf("  Ptr   : %s\n", head->ptr ? head->ptr : "(null)");
		ptr = ft_strchr(head->ptr, '$');
		counter = 0;
		while (ptr)
		{
			if (!counter++)
				write(1, head->ptr, ptr - head->ptr);
			len = dollar_len(ptr);
			tab = expand(env, ptr + 1, len - 1);
			int y = 0;
			if (is_white_space(*head->ptr))
				ft_putchar_fd(' ', 1);
			while (tab && tab[y])
			{
				ft_printf("%s", tab[y++]);
				if (tab[y])
					write(1, " ", 1);
			}
			ptr = ft_strchr(ptr + len, '$');
		}
		write(1, "\n", 1);
		head = head->next;
	}
}


int	main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;

	char		*str;
	char		*prev_cmdline = NULL;
	int			fd;
	int			history_fd;
	int			ret_val;
	t_prompt	prompt_var;
	t_snippet	*lst;
	t_hash_table table;


	// Creating hash table and aliases
	ft_bzero(&table, sizeof(table));
	parse_rc(&table);

	// Initializing Prompt
	ft_bzero(&prompt_var, sizeof(t_prompt));
	prompt_var.prompt_raw = "\\u@\\h:\\w\\$ ";
	update_prompt_var(&prompt_var);


	// Setting signals
	if (!ms_set_sighandler())
		return (ft_putendl_fd("Error setting signals", 2), 1);

	// Getting .ms_history fd
	history_fd = ms_get_history_fd();

	// Main loop
	while (1)
	{
		ret_val = get_cmd_line_fd(&fd, prompt_var, history_fd);
		if (ret_val == -1)
			break ;
		str = gnl(fd);
		close(fd);
		if (!str)
			break ;
		if (*str)
		{
			ms_add_history(str, history_fd, &prev_cmdline);
			if (!prev_cmdline)
				ft_putstr_fd("Error saving prev_cmdline\n", 2);
			lst = lexer(str, &table);
			if (!lst)
			{
				free(str);
				break ;
			}
			print_snippet_list(lst, env);
		}
		free(str);
		str = NULL;
	}

	// Freeing everything 
	free_table(&table);
	close(history_fd);
	free(prev_cmdline);
	free(prompt_var.prompt);
	free(prompt_var.hostname);
	clear_history();
	rl_clear_history();
	return (0);
}
