#include "minishell.h"
#include "libftprintf.h"
#include "get_next_line.h"
#include <sys/wait.h>
#include <fcntl.h>

int	write_prompt_elem(t_prompt *prompt, char c)
{
	if (c == 'h')
		print_until(prompt->hostname, '.', STDOUT_FILENO);
	else if (c == 'H')
		ft_putstr_fd(prompt->hostname, STDOUT_FILENO);
	else if (c == 'u')
		ft_putstr_fd((char *)prompt->user, STDOUT_FILENO);
	else if (c == 'w')
		ft_putstr_fd(prompt->cwd, STDOUT_FILENO);
	else if (c == 'W')
		ft_putstr_fd((char *)prompt->cwd_basename, STDOUT_FILENO);
	else if (c == '$')
		ft_putchar_fd('$', STDOUT_FILENO);
	return (1);
}

void	get_prompt(t_prompt *prompt, int pipe_fds[2])
{
	int	i;

	i = 0;
	if (dup2(pipe_fds[1], STDOUT_FILENO) == -1
		|| close(pipe_fds[0]) == -1 || close(pipe_fds[1]) == -1)
	{
		free(prompt->hostname);
		exit(1);//manage memory leaks
	}
	while (prompt->prompt_raw[i])
	{
		if (prompt->prompt_raw[i] == '\\')
			i += write_prompt_elem(prompt, prompt->prompt_raw[i + 1]);
		else
			ft_putchar_fd(prompt->prompt_raw[i], STDOUT_FILENO);
		i++;
	}
	free(prompt->hostname);
	//close(history_fd);
	exit(0);
}

bool	expand_prompt(t_prompt *prompt)
{
	int		status;
	int		pipe_fds[2];
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (false);
	pid = fork();
	if (pid < 0)
		return (close(pipe_fds[0]),close(pipe_fds[1]), false);
	else if (is_child(pid))
		get_prompt(prompt, pipe_fds);
	close(pipe_fds[1]);
	wait(&status);
	if (get_exit_value(status))
		return (close(pipe_fds[0]), false);
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
	char	*tmp;

	fd = open(HOSTNAME_FILE, O_RDONLY);
	if (fd < 0)
		return (NULL);
	hostname = get_next_line(fd);
	ptr_newline = ft_strchr(hostname, '\n');
	if (ptr_newline)
		*ptr_newline = 0;
	tmp = get_next_line(fd);
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
