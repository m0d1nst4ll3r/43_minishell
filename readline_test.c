#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int ac, char **av)
{
	char	*line;

	(void)ac;
	while (1)
	{
		line = readline(av[1]);
		if (!line || !strcmp(line, "exit"))
		{
			free(line);
			return (0);
		}
		add_history(line);
		printf("You typed: %s\n", line);
		free(line);
	}
	return (0);
}
