#ifndef MINISHELL_H
# define MINISHELL_H

# define PROMPT_CHARSET "uhHwW$\\"
# define SEP "| &<>;"
# define OPENER "'\"("
# define HOSTNAME_FILE	"/etc/hostname"
# define PROMPT "minishell> "

# include <stdbool.h>
# include <limits.h>
# include <unistd.h>

typedef struct s_prompt
{
	char		*hostname;
	char		cwd[PATH_MAX];
	char		*prompt;
	char const	*cwd_basename;
	char const	*user;
	char const	*prompt_raw;
}	t_prompt;

typedef struct s_pair
{
	char	*key;
	int		key_len;
	char	*value;
}	t_pair;

# define TABLE_SIZE 512 // Always 2^n to keep the speed

typedef struct s_hash_table
{
	t_pair	*bucket[TABLE_SIZE];
}	t_hash_table;

//Utils

char	*pass_whitespace(char *str);
int		get_exit_value(int status);
bool	is_child(pid_t pid);
char	*_basename(char *str);
void	print_until(char *str, char c, int fd);

//History

int		ms_get_history_fd(void);
void	ms_add_history(char *str, int fd, char **ptr_oldcmd);
void	trim_trailling_ws(char *str);

//Signal

void	sigint_handler(int sig);
void	set_sigint_handler(int fds_to_close[2]);
bool	ms_set_sighandler(void);

//Parsing 

int		get_cmd_line_fd(int	*fd, t_prompt prompt, int history_fd);

//Prompt

bool	expand_prompt(t_prompt *prompt);
bool	update_prompt_var(t_prompt *ptr);




bool	is_opened(char *str);

#endif
