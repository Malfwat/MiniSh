#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "get_next_line.h"
#include "minishell.h"
#include "libftprintf.h"

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
	prompt_var.prompt_raw = "\e[96m\\u\e[0m@\\h:\\w\\$ ";
	update_prompt_var(&prompt_var);
	if (!ms_set_sighandler())
		return (ft_putendl_fd("Error setting signals", 2), 1);
	history_fd = ms_get_history_fd();
	while (1)
	{
		ret_val = get_cmd_line_fd(&fd, prompt_var, history_fd);
		if (ret_val == -1)
			return (1);
		str = get_next_line(fd);
		ft_printf("\n\nThis is str: [%s]\n\n", str);
		close(fd);
		if (!str)
			break ;
		if (*str)
		{
			ms_add_history(str, history_fd, &prev_cmdline);
			if (!prev_cmdline)
				ft_putstr_fd("Error saving prev_cmdline\n", 2);
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
