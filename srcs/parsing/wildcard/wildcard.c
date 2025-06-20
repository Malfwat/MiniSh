/*#include "minishell.h"

t_snippet	wildcard(char *str, char *cwd)
{
}*/
#include <sys/types.h>
#include <dirent.h>

int	main(void)
{
	DIR	*folder;
	struct dirent	*file;

	folder = opendir("/home/malfwa/Documents/minishell");
	if (!folder)
		return (0);

	file = readdir(folder);
	while (file)
	{
		printf("%s\n", file->d_name);
		file = readdir(folder);
	}

	closedir(folder);
	return (0);
}
