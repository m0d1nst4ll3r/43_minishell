#include "minishell.h"

int	main(int ac, char **av, char **ep)
{
	t_minishell	data;

	if (ac != 2)
		return (0);
	data.env = ep;
	data.last_return = 99;
	data.line = av[1];
	printf("%s\n", data.line);
	printf("%zu\n", get_word_len(&data, 0));
}
