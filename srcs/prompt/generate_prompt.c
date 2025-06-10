#include "minishell.h"
#include "libftprintf.h"
#include "get_next_line.h"
#include <sys/wait.h>
#include <fcntl.h>

void	write_cwd(t_prompt *prompt, int fd)
{
	int		len;
	char	*home;

	home = getenv("HOME");
	len = ft_strlen(home);
	if (!ft_strncmp(prompt->cwd, home, len))
	{
		ft_putstr_fd("~", fd);
		ft_putstr_fd(prompt->cwd + len, fd);
	}
	else
		ft_putstr_fd(prompt->cwd, fd);
}

int	write_prompt_elem(t_prompt *prompt, char c, int fd)
{
	if (c == 'h')
		print_until(prompt->hostname, '.', fd);
	else if (c == 'H')
		ft_putstr_fd(prompt->hostname, fd);
	else if (c == 'u')
		ft_putstr_fd((char *)prompt->user, fd);
	else if (c == 'w')
		write_cwd(prompt, fd);
	else if (c == 'W')
		ft_putstr_fd((char *)prompt->cwd_basename, fd);
	else if (c == '$')
		ft_putchar_fd('$', fd);
	return (1);
}

void	get_prompt(t_prompt *prompt, int pipe_fds[2])
{
	int	i;

	i = 0;
	while (prompt->prompt_raw[i])
	{
		if (prompt->prompt_raw[i] == '\\')
			i += write_prompt_elem(prompt, prompt->prompt_raw[i + 1], pipe_fds[1]);
		else
			ft_putchar_fd(prompt->prompt_raw[i], pipe_fds[1]);
		i++;
	}
}

bool	expand_prompt(t_prompt *prompt)
{
	int		pipe_fds[2];

	if (pipe(pipe_fds) == -1)
		return (false);
	get_prompt(prompt, pipe_fds);
	close(pipe_fds[1]);
	prompt->prompt = get_next_line(pipe_fds[0]);
	empty_gnl(pipe_fds[0]);
	close(pipe_fds[0]);
	return (true);
}

char	*ms_gethostname(void)
{
	int		fd;
	char	*hostname;
	char	*ptr_newline;

	fd = open(HOSTNAME_FILE, O_RDONLY);
	if (fd < 0)
		return (NULL);
	hostname = get_next_line(fd);
	ptr_newline = ft_strchr(hostname, '\n');
	if (ptr_newline)
		*ptr_newline = 0;
	empty_gnl(fd);
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
	free(ptr->prompt);
	if (!expand_prompt(ptr))
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
