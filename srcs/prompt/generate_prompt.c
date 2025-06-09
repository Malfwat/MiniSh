#include "minishell.h"
#include "libftprintf.h"
#include "get_next_line.h"
#include <fcntl.h>

int	print_prompt_elem(t_prompt prompt, char c)
{
	if (c == 'h')
		print_until(prompt.hostname, '.', STDOUT_FILENO);
	else if (c == 'H')
		ft_putstr_fd(prompt.hostname, STDOUT_FILENO);
	else if (c == 'u')
		ft_putstr_fd((char *)prompt.user, STDOUT_FILENO);
	else if (c == 'w')
		ft_putstr_fd(prompt.cwd, STDOUT_FILENO);
	else if (c == 'W')
		ft_putstr_fd((char *)prompt.cwd_basename, STDOUT_FILENO);
	else if (c == '$')
		ft_putchar_fd('$', STDOUT_FILENO);
	return (1);
}

void	expand_prompt(t_prompt prompt)
{
	int	i;

	i = 0;
	while (prompt.prompt[i])
	{
		if (prompt.prompt[i] == '\\')
			i += print_prompt_elem(prompt, prompt.prompt[i + 1]);
		else
			ft_putchar_fd(prompt.prompt[i], STDOUT_FILENO);
		i++;
	}
}

char	*ms_gethostname(void)
{
	int		fd;
	char	*hostname;
	char	*ptr_newline;
	char	*tmp;

	fd = open(HOSTNAME_FILE, O_RDONLY);
	if (fd < 0)
		return (NULL);
	hostname = get_next_line(fd);
	ptr_newline = ft_strchr(hostname, '\n');
	if (ptr_newline)
		*ptr_newline = 0;
	tmp = get_next_line(fd);
	while (tmp)
	{
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
	return (hostname);
}

bool	update_prompt_var(t_prompt *ptr)
{
	ptr->user = getenv("USER");
	if (!ptr->user)
		return (false);
	if (!getcwd(ptr->cwd, PATH_MAX))
		return (false);
	ptr->cwd_basename = _basename(ptr->cwd);
	free(ptr->hostname);
	ptr->hostname = ms_gethostname();
	if (!ptr->hostname)
		return (false);
	return (true);
}

bool	parse_prompt_raw(char *prompt_raw)
{
	int	i;

	i = 0;
	prompt_raw = pass_whitespace(prompt_raw);
	if (!prompt_raw || !*prompt_raw)
		return (false);
	while (prompt_raw[i])
	{
		if (prompt_raw[i] == '\\' && !is_in_str(prompt_raw[i + 1], PROMPT_CHARSET))
			return (false);
		i++;
	}
	return (true);
}
