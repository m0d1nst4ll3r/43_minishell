#include "minishell.h"

#define ANSI_RESET_ALL          "\x1b[0m"

#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_WHITE        "\x1b[37m"

#define REDIR_IN_STR		" <"
#define REDIR_OUT_STR		" >"
#define REDIR_HEREDOC_STR	"<<"
#define REDIR_APPEND_STR	">>"

char	*get_redir_type_str(t_redir_type type)
{
	if (type == REDIR_IN)
		return (REDIR_IN_STR);
	if (type == REDIR_OUT)
		return (REDIR_OUT_STR);
	if (type == REDIR_HEREDOC)
		return (REDIR_HEREDOC_STR);
	return (REDIR_APPEND_STR);
}

void	print_cmd_list(t_command *cmd_list)
{
	t_redir	*cur;
	size_t	i;
	size_t	j;

	i = 0;
	while (cmd_list)
	{
		printf("%zu: ", i);
		j = 0;
		while (cmd_list->argv[j])
		{
			printf("[" ANSI_COLOR_CYAN "%s" ANSI_RESET_ALL "] ", cmd_list->argv[j]);
			j++;
		}
		printf("\n");
		printf("%zu: ", i);
		cur = cmd_list->redir;
		while (cur)
		{
			printf("[" ANSI_COLOR_RED "%s" ANSI_RESET_ALL "][" ANSI_COLOR_YELLOW "%s" ANSI_RESET_ALL "] ", get_redir_type_str(cur->type), cur->file);
			cur = cur->next;
		}
		printf("\n");
		cmd_list = cmd_list->next;
		i++;
	}
}

int	main(int ac, char **av, char **ep)
{
	t_minishell	data;

	if (ac != 3)
	{
		printf("usage: ./a.out (last_return) (input)\n");
		return (0);
	}
	data.env = ep;
	data.last_return = (unsigned char)atoi(av[1]);
	data.line = av[2];
	printf("Input:       [" ANSI_COLOR_GREEN "%s" ANSI_RESET_ALL "]\n", data.line);
	printf("Last return: [" ANSI_COLOR_GREEN "%s" ANSI_RESET_ALL "]\n", av[1]);
	data.cmd_list = parse(&data);
	printf("Command list:\n");
	if (!data.cmd_list)
		printf("(empty)\n");
	else
		print_cmd_list(data.cmd_list);
	cleanup_cmd_list(data.cmd_list, 1);
}
