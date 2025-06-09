#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h"
#include <stdbool.h>

bool	ms_set_sighandler(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		return (false);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (false);
	return (true);
}

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	ft_putstr_fd(rl_prompt, STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	set_sigint_handler(int fds_to_close[2])
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		close(fds_to_close[0]);
		close(fds_to_close[1]);
		ft_putstr_fd("Error handling signal\n", 2);
		exit(1);
	}
}
