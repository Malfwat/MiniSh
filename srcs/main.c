
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
		while (str[i] && !ft_strchr(SEP, str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || ft_strchr(SEP, str[i]))
			return (i);
		i += closing_match(str + i) + 1;
	}
	return (i);
}

void	lexer(char *str, t_hash_table *table)
{
	bool	count;
	int		len;
	t_pair	*pair;

	if (!str)
		return ;
	count = false;
	while (*str)
	{
		len = word_len(str);
		if (!count)
		{
			count = true;
			pair = get_pair(table, str, len);
			if (pair)
				ft_putstr(pair->value);
			else
				write(STDOUT_FILENO, str, len);
		}
		else
			write(STDOUT_FILENO, str, len);
		ft_putchar_fd('\n', STDOUT_FILENO);
		str += len;
		str = pass_whitespace(str);
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
			lexer(str, &table);
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
