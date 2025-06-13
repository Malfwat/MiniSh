
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

int	closing_match(char *ptr)
{
	if (*ptr == '\'' || *ptr == '"')
		return (ft_strchr(ptr + 1, *ptr) - ptr);
	return (find_closing_bracket(ptr));
}

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

int	word_len(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		while (str[i] && !ft_strchr(SEP, str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || ft_strchr(SEP, str[i]))
			return (i);
		i += closing_match(str + i) + 1;
	}
	return (i);
}

void	print_without_quote(char **env, char *str, int len)
{
	int		i;
	int		j;
	char	quote;
	char	*ptr;

	if (!str)
		return ;
	if (!ft_strchr(str, '\'') && !ft_strchr(str, '\"'))
		return (ft_putstr_fd(str, STDOUT_FILENO));
	j = 0;
	while (*str && j < len)
	{
		i = 0;
		while (str[i] && str[i] != '\'' && str[i] != '\"')
			i++;
		j += i;
		write(STDOUT_FILENO, str, i);
		if (j >= len)
			break ;
		str += i;
		if (*str == '\'' || *str == '\"')
		{
			quote = *str;
			str++;
			j++;
			ptr = ft_strchr(str, quote);
			if (!ft_strchr_len(str, '$', ptr - str) || quote == '\'')
				write(STDOUT_FILENO, str, ptr - str);
			else
				expand_env_var(env, str + 1, dollar_len(str + 1));
			j += (ptr - str) + 1;
			str = ptr + 1;
		}
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

void	expand_env_var(char **env, char *varname, int len)
{
	int	i;

	if (!env || !varname || !*varname)
		return ;
	i = 0;
	while (env[i] && ft_strncmp_weq(varname, env[i], len))
		i++;
	if (env[i])
		print_without_quote(env, env[i] + len + 1, ft_strlen(env[i] + len));
	else
		print_without_quote(env, varname, len);
}

int	dollar_len(char *str_dollar)
{
	int	i;

	i = 0;
	if (!str_dollar)
		return (0);
	if (*str_dollar == '$')
		str_dollar++;
	while (ft_isalnum(str_dollar[i]) || str_dollar[i] == '_')
		i++;
	return (i);
}

#define REDIR_IN "<"
#define REDIR_OUT ">"
#define HERE_DOC "<<"
#define APPEND ">>"

void	pass_sep(char **str)
{
	if (!ft_strncmp(*str, HERE_DOC, 2))
	{
		ft_putstr_fd(HERE_DOC, STDOUT_FILENO);
		(*str) += 2;
		(*str) += write(STDOUT_FILENO, *str, word_len(*str));
	}
	else if (!ft_strncmp(*str, APPEND, 2))
	{
		ft_putstr_fd(APPEND, STDOUT_FILENO);
		(*str) += 2;
		*str = pass_whitespace(*str);
	}
	else if (!ft_strncmp(*str, REDIR_IN, 1) || !ft_strncmp(*str, REDIR_OUT, 1))
	{
		ft_putchar_fd(**str, STDOUT_FILENO);
		(*str)++;
		*str = pass_whitespace(*str);
	}
	else
	{
		ft_putchar_fd(**str, STDOUT_FILENO);
		(*str)++;
	}
}

void	expand(char **env, char *str, t_hash_table *table)
{
	bool	count;
	int		len;
	t_pair	*pair;

	if (!str)
		return ;
	count = false;
	while (*str)
	{
		if (*str == '$')
			len = dollar_len(str);
		else
			len = word_len(str);
		if (!len)
		{
			pass_sep(&str);
			continue ;
			exit(0);
		}
		if (!count)
		{
			count = true;
			pair = get_pair(table, str, len);
			if (pair)
			{
				if (pair->value[0] == '$')
					expand_env_var(env, pair->value + 1, dollar_len(pair->value + 1) + 1);
				else
					print_without_quote(env, pair->value, ft_strlen(pair->value));
			}
			else
			{
				if (*str == '$')
				{
					expand_env_var(env, str+ 1, len);
					len++;
				}
				else
					print_without_quote(env, str, len);
			}
		}
		else
			if (*str == '$')
			{
				expand_env_var(env, str+ 1, len);
				len++;
			}
			else
				print_without_quote(env, str, len);
		str += len;
		str = pass_whitespace(str);
		write(1, " ", 1);
	}
	write(1, "\n", 1);
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
			expand(env, str, &table);
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
