#include <sys/wait.h>
#include <readline/readline.h>
#include "libftprintf.h"
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

void	ms_rdl(char *prompt, int fd)
{
	char	*ptr;
	char	*tmp;

	ft_putchar_fd('\n', fd);
	rl_on_new_line();
	ptr = readline(prompt);
	if (ptr && *ptr)
	{
		tmp = pass_whitespace(ptr);
		trim_trailling_ws(tmp);
		ft_putstr_fd(tmp, fd);
	}
	if (is_opened(ptr))
	{
		free(ptr);
		return (ms_rdl(prompt, fd));
	}
	free(ptr);
}

void	rdl_child(int pipe_fds[2], pid_t pid, t_prompt prompt, int history_fd)
{
	char	*ptr;
	void	*tmp;

	if (is_child(pid))
	{
		set_sigint_handler(pipe_fds);
		if (prompt.prompt)
			ptr = readline(prompt.prompt);
		else
			ptr = readline(PROMPT);
		if (ptr && *ptr)
		{
			tmp = pass_whitespace(ptr);
			trim_trailling_ws(tmp);
			ft_putstr_fd(tmp, pipe_fds[1]);
		}
		else if (ptr)
			ft_putchar_fd(' ', pipe_fds[1]);
		if (is_opened(ptr))
		{
			ms_rdl("> ", pipe_fds[1]);
		}
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		close(history_fd);
		free(ptr);
		free(prompt.prompt);
		exit(0);
	}
}

int	get_cmd_line_fd(int	*fd, t_prompt prompt, int history_fd)
{
	int		pipe_fds[2];
	int		status;
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), -1);
	rdl_child(pipe_fds, pid, prompt, history_fd);
	close(pipe_fds[1]);
	wait(&status);
	*fd = pipe_fds[0];
	return (get_exit_value(status));
}
