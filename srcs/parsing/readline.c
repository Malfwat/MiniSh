#include <sys/wait.h>
#include <readline/readline.h>
#include "libftprintf.h"
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

void	rdl_child(int pipe_fds[2], pid_t pid, t_prompt prompt)
{
	char	*ptr;

	if (is_child(pid))
	{
		set_sigint_handler(pipe_fds);
		if (prompt.prompt)
			ptr = readline(prompt.prompt);
		else
			ptr = readline(PROMPT);
		if (ptr && *ptr)
			ft_putstr_fd(ptr, pipe_fds[1]);
		else if (ptr)
			ft_putchar_fd(' ', pipe_fds[1]);
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		free(ptr);
		free(prompt.prompt);
		exit(0);
	}
}

int	get_cmd_line_fd(int	*fd, t_prompt prompt)
{
	int		pipe_fds[2];
	int		status;
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (close(pipe_fds[0]), close(pipe_fds[1]), -1);
	rdl_child(pipe_fds, pid, prompt);
	close(pipe_fds[1]);
	wait(&status);
	*fd = pipe_fds[0];
	return (get_exit_value(status));
}
