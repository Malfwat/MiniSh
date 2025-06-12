#include "minishell.h"
#include "libftprintf.h"
#include "get_next_line.h"
#include <sys/wait.h>
#include <fcntl.h>

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
		if (prompt_raw[i] == '\\'
			&& !is_in_str(prompt_raw[i + 1], PROMPT_CHARSET))
			return (false);
		i++;
	}
	return (true);
}
