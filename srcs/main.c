/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <admoufle@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:05:47 by malfwa            #+#    #+#             */
/*   Updated: 2025/06/10 20:16:42 by malfwa           ###   ########.fr       */
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
	ft_printf("quote:[%c]\tbracket:[%i]\n", quote, bracket);
	return (quote || bracket > 0);
}


int	closing_match(char *ptr);

int	find_closing_bracket(char *opening_bracket)
{
	char	*ptr;
	int		i;
	int		nb_opened;

	ptr = opening_bracket;
	nb_opened = 1;
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
	int	i;

	i = 0;
	if (*ptr == '\'' || *ptr == '"')
		return (ft_strchr(ptr + 1, *ptr) - ptr);
	return (find_closing_bracket(ptr));
}

int	word_len(char *str)
{
	int		i;
//	char	opener; 
	i = 0;
	while (str[i])
	{
		while (str[i] && !ft_strchr(SEP, str[i]) && !ft_strchr(OPENER, str[i]))
			i++;
		if (!str[i] || ft_strchr(SEP, str[i]))
			return (i);
		i += closing_match(str + i) + 1;
//		ft_printf("%i\n", closing_match(str + i));
//		exit(1);
//		return (i);
	}
	return (i);
}

void	lexer(char *str)
{
	int	len;

	if (!str)
		return ;
	while (*str)
	{
		len = word_len(str);
		ft_printf("-%i-word:", len);
		write(STDOUT_FILENO, str, len);
		ft_putendl_fd("", STDOUT_FILENO);
		str += len;
		str = pass_whitespace(str);
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
	
	ft_bzero(&prompt_var, sizeof(t_prompt));
	prompt_var.prompt_raw = "\\u@\\h:\\w\\$ ";
	//prompt_var.prompt_raw = "\e[96m\\u\e[0m@\\h:\\w\\$ ";
	update_prompt_var(&prompt_var);
	if (!ms_set_sighandler())
		return (ft_putendl_fd("Error setting signals", 2), 1);
	history_fd = ms_get_history_fd();
	while (1)
	{
		ret_val = get_cmd_line_fd(&fd, prompt_var, history_fd);
		if (ret_val == -1)
			return (1);
		str = gnl(fd);
		ft_printf("\n\nThis is str: [%s]\n\n", str);
		close(fd);
		if (!str)
			break ;
		if (*str)
		{
			ms_add_history(str, history_fd, &prev_cmdline);
			if (!prev_cmdline)
				ft_putstr_fd("Error saving prev_cmdline\n", 2);
			lexer(str);
		}
		free(str);
		str = NULL;
	}
	close(history_fd);
	free(prev_cmdline);
	free(prompt_var.prompt);
	free(prompt_var.hostname);
	rl_clear_history();
	return (0);
}
