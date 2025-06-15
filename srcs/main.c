
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

int	word_len(char *str, bool (*is_sep)(char ))
{
	int		i;
	i = 0;
	while (str[i])
	{
		if (!i && is_sep(str[i]))
		{
			if (*str == '(' || *str == ')' || *str == ';')
				return (1);
			if (*str == '<' || *str == '>' || *str == '|')
				return ((int []){1, 2}[str[0] == str[1]]);
			i++;
		}
		while (str[i] && !is_sep(str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || is_sep(str[i]))
			return (i);
		if (is_sep(str[i]) && *str == '$')
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

bool	dollar_n_sep(char c)
{
	if (ft_strchr(SEP, c) || c == '$')
		return (true);
	return (false);
}

bool	simple_sep(char c)
{
	if (ft_strchr(SEP, c))
		return (true);
	return (false);
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
		len = word_len(str, simple_sep);
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

//char	**expand(char **env, char *var_name, int len)
char	*expand(char **env, char *var_name, int len)
{
//	static char	set[] = {'\t', '\v', '\n', '\r', '\f', ' ', 0};
	int	i;

	i = 0;
	while (env[i] && ft_strncmp_weq(var_name, env[i], len))
		i++;
	if (env[i])
		return (env[i] + len + 1);
	return (NULL);
//		return (ft_split_set(env[i] + len + 1, set));
//	return (NULL);
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

size_t	write_snip(char *str, char *quote, int pfd, int len)
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
			return (write(pfd, str, i));
		i++;
	}
	return (write(pfd, str, i));
}

void	write_without_quote(int pfd, char *str, int len)
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
			ft_putchar_fd(*str, pfd);
		else
		{
			str++;
			ptr = ft_strchr(str, '\'');
			len_to_write = ptr - str;
			i += len_to_write + 2;
			write(pfd, str , len_to_write);
			str = ptr;
		}
		str++;
		i++;
	}
}

void	expand_token(char *ptr, char **env, int len)
{
//	int		pipe_fds[2];
	char	*tmp;
	int		final_len;
	int		wlen;
	int		i;
	char	quote = 0;

	(void)env;(void)final_len;

	size_t	test;
//	if (pipe(pipe_fds) < 0)
//		return (NULL);
//	final_len = 0;
	i = 0;
	while (*ptr && i < len)
	{
		if (*ptr == '$')
			wlen = dollar_len(ptr);
		else if (*ptr == '\'' || *ptr == '"')
			wlen = closing_match(ptr) + 1;
		else
			wlen = word_len(ptr, dollar_n_sep);
		if (*ptr == '"')
		{
			//ft_putstr_fd("\"", 1);
			expand_token(ptr + 1, env, wlen - 2);
			//ft_putstr_fd("\"", 1);
		}
		else
		{
			if (*ptr == '$')
			{
				tmp = expand(env, ptr + 1, wlen - 1);
				if (tmp)
				{
					while (*tmp)
					{
						test = write_snip(tmp, &quote, 2, ft_strlen(tmp));
						if (test != ft_strlen(tmp))
						{
							write(1, "\n", 1);
							tmp += test;
							tmp = pass_whitespace(tmp);
						}
						else
							break ;
					}
				}
			}
			else
				write_without_quote(1, ptr, wlen);
			//write(1, "\n", 1);
		}
		ptr += wlen;
		i += wlen;
	}
}

#include <signal.h>

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
			signal(SIGINT, SIG_DFL);
			expand_token(lst->ptr, env, ft_strlen(lst->ptr));
			write(1, "\n", 1);
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
