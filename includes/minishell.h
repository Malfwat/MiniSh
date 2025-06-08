#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>

enum e_state
{
	leave
};

//Utils

char	*pass_whitespace(char *str);
int		get_exit_value(int status);
bool	is_child(pid_t pid);

//History

int		ms_get_history_fd(void);
void	ms_add_history(char *str, int fd, char **ptr_oldcmd);

//Signal

void	sigint_handler(int sig);
void	set_sigint_handler(int fds_to_close[2]);
bool	ms_set_sighandler(void);

//Parsing 

int	get_cmd_line_fd(int	*fd);

//Prompt

char	*expand_prompt(char *prompt_raw);
#endif
