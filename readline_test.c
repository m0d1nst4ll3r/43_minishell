#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE	64

int	main(int ac, char **av)
{
	char	buffer[BUFFER_SIZE];
	char	*line;

	(void)ac;
	while (1)
	{
		line = readline(av[1]);
		if (!strcmp(line, "clear"))
		{
			printf("History cleared\n");
			rl_clear_history();
			free(line);
		}
		else if (!strcmp(line, "pwd"))
		{
			add_history(line);
			if (getcwd(buffer, BUFFER_SIZE))
				printf("%s\n", buffer);
			free(line);
		}
		else if (!strncmp(line, "cd ", 3) && line[3])
		{
			if (chdir(line + 3) == -1)
				printf("%s: %s\n", line + 3, strerror(errno));
			free(line);
		}
		else if (!line || !strcmp(line, "exit"))
		{
			free(line);
			return (0);
		}
		else
		{
			add_history(line);
			printf("You typed: %s\n", line);
			free(line);
		}
	}
	return (0);
}
