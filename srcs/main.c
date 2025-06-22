/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:05:13 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/22 21:27:23 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "get_next_line.h"
#include "minishell.h"
#include "libftprintf.h"

bool	is_opened(char *str)
{
	static char	quote;
	static int	bracket;
	char		c;
	char		d;

	c = 0;
	d = 0;
	if (!str)
		return (quote || bracket > 0);
	while (*str)
	{
		if ((quote == '\'' && *str == '\'') || (quote == '\"' && *str == '\"'))
			quote = '\0';
		else if (!quote && (*str == '\'' || *str == '\"'))
			quote = *str;
		else if (!quote && (*str == '(' || *str == ')'))
			bracket += (int []){-1, 1}[*str == '('];
		d = c;
		c = *str;
		str++;
	}
	return (quote || bracket > 0 || c == '|' || (c == '&' && d == '&'));
}

enum e_token	get_token(char *str)
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

void	optimize_lst(t_snippet **head)
{
	t_snippet	*ptr;
	void		*tmp;

	ptr = *head;
	while (ptr)
	{
		if (is_redir(ptr->token) && ptr->next && ptr->next->token == word)
		{
			ptr->next->token = ptr->token;
			tmp = ptr->next->next;
			pop_snip(head, ptr);
			ptr = tmp;
		}
		else
			ptr = ptr->next;
	}
}

const char	*token_to_string(enum e_token token);

t_snippet	*lexer(char *str)
{
	int			len;
	t_snippet	*lst;
	char		*dup;

	if (!str)
		return (NULL);
	lst = NULL;
	while (*str)
	{
		len = word_len(str, simple_sep, ft_strlen(str));
		dup = ft_strndup(str, len);
		if (!dup || !add_to_snip_lst(&lst, get_token(dup), dup))
			return (free_snip_lst(lst), NULL);
		str += len;
		str = pass_whitespace(str);
	}
	return (lst);
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

char	*expand(char **env, char *var_name, int len)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp_weq(var_name, env[i], len))
		i++;
	if (env[i])
		return (env[i] + len + 1);
	return (NULL);
}

size_t	write_snip(char *str, char *quote, int len)
{
	int	i;

	if (!str || !quote || !*str)
		return (0);
	i = 0;
	while (str[i] && i < len)
	{
		if (!*quote && (str[i] == '\'' || str[i] == '"'))
			*quote = str[i];
		else if (*quote == str[i])
			*quote = 0;
		if (str[i] == ' ' && !*quote)
			return (write(STDOUT_FILENO, str, i));
		i++;
	}
	return (write(STDOUT_FILENO, str, i));
}

void	write_without_quote(char *str, int len)
{
	int		i;
	int		len_to_write;
	char	*ptr;

	if (!str)
		return ;
	i = 0;
	while (*str && i < len)
	{
		if (*str != '\'')
			ft_putchar_fd(*str, STDOUT_FILENO);
		else
		{
			str++;
			ptr = ft_strchr(str, '\'');
			len_to_write = ptr - str;
			i += len_to_write + 2;
			write(STDOUT_FILENO, str, len_to_write);
			str = ptr;
		}
		str++;
		i++;
	}
}

void	dollar_exp(char *ptr, char scope, char *quote)
{
	size_t	test;

	if (ptr && !scope)
	{
		while (*ptr)
		{
			test = write_snip(ptr, quote, ft_strlen(ptr));
			if (test != ft_strlen(ptr))
				write(1, "\n", 1);
			ptr += test;
			ptr = pass_whitespace(ptr);
		}
	}
	else if (ptr)
		write_without_quote(ptr, ft_strlen(ptr));
}

void	put_to_zero(int *i, char *quote)
{
	if (i)
		*i = 0;
	if (quote)
		*quote = 0;
}

void	expand_token(char *ptr, char **env, int len, char scope)
{
	int		wlen;
	int		i;
	char	quote;

	put_to_zero(&i, &quote);
	while (*ptr && i < len)
	{
		wlen = get_wlen(ptr, len);
		if (*ptr == '"')
			expand_token(ptr + 1, env, wlen - 2, *ptr);
		else
		{
			if (*ptr == '$' && wlen != 1 && ft_strncmp("$$", ptr, 2))
			{
				if (wlen == 2 && !ft_strncmp("$?", ptr, wlen))
					ft_putnbr_fd(972, STDOUT_FILENO);
				else
					dollar_exp(expand(env, ptr + 1, wlen - 1), scope, &quote);
			}
			else
				write_without_quote(ptr, wlen);
		}
		ptr += wlen;
		i += wlen;
	}
}

#include <stdio.h>

const char *token_to_string(enum e_token token)
{
	switch (token)
	{
		case word: return ("word");
		case redir_in: return ("redir_in");
		case redir_out: return ("redir_out");
		case here_doc: return ("here_doc");
		case append: return ("append");
		case pipe_delim: return ("pipe_delim");
		case or: return ("or");
		case and: return ("and");
		case semicolon: return ("semicolon");
		case open_par: return ("open_par");
		case closing_par: return ("closing_par");
		case env_var: return ("env_var");
		default: return ("unknown");
	}
}

void print_snippet_list(t_snippet *head)
{
	while (head != NULL)
	{
		printf("Token: %-12s | Value: %s\n", token_to_string(head->token), head->ptr ? head->ptr : "(null)");
		head = head->next;
	}
}

int	main(int ac, char **av, char **env)
{
	char			*str;
	char			*prev_cmdline = NULL;
	int				fd;
	int				history_fd;
	int				ret_val;
	t_prompt		prompt_var;
	t_snippet		*lst = NULL;
	t_hash_table	table;

	(void)ac;(void)av;(void)env;

	// Checking if we are in a tty
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) || !isatty(STDERR_FILENO))
		return (1);

	// Creating hash table and aliases
	ft_bzero(&table, sizeof(table));
	parse_rc(&table);

	// Initializing Prompt
	ft_bzero(&prompt_var, sizeof(t_prompt));
	prompt_var.prompt_raw = "\\u@\\h:\\w\\$ ";
	update_prompt_var(&prompt_var);

	// Getting .ms_history fd
	history_fd = ms_get_history_fd(&prev_cmdline);

	// Main loop
	while (1)
	{
		ret_val = get_cmd_line_fd(&fd, prompt_var, history_fd);
		if (ret_val == -1)
			break ;
		//str = gnl(fd);
		str = get_next_null(fd);
		close(fd);
		if (!str)
			break ;
		if (*str)
		{
			ms_add_history(str, history_fd, &prev_cmdline);
			if (!prev_cmdline)
				ft_putstr_fd("Error saving prev_cmdline\n", 2);
			lst = lexer(str);
			if (!lst)
			{
				free(str);
				break ;
			}
			//expand_snip(&lst, lst, env, true); // would be done in exec
			if (check_syntaxe(lst, _basename(av[0])))
			{
				replace_aliases(&lst, &table);
				replace_tilde(lst, getenv("HOME"));
				replace_wildcards(&lst);
				optimize_lst(&lst);
			expand_snip(&lst, lst, env, true); // would be done in exec
				print_snippet_list(lst); // exec
			}
		}
		free(str);
		free_snip_lst(lst);
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
