#include "minishell.h"

#define TOKEN_LESSER_STR			"  < "
#define TOKEN_LESSERLESSER_STR		" << "
#define TOKEN_GREATER_STR			"  > "
#define TOKEN_GREATERGREATER_STR	" >> "
#define TOKEN_PIPE_STR				"  | "
#define TOKEN_WORD_STR				"WORD"

char	*get_token_type_str(t_token_type token_type)
{
	if (token_type == TOKEN_LESSER)
		return (TOKEN_LESSER_STR);
	if (token_type == TOKEN_LESSERLESSER)
		return (TOKEN_LESSERLESSER_STR);
	if (token_type == TOKEN_GREATER)
		return (TOKEN_GREATER_STR);
	if (token_type == TOKEN_GREATERGREATER)
		return (TOKEN_GREATERGREATER_STR);
	if (token_type == TOKEN_PIPE)
		return (TOKEN_PIPE_STR);
	return (TOKEN_WORD_STR);
}

void	print_token_list(t_token *token_list)
{
	size_t	i;

	i = 0;
	while (token_list)
	{
		if (token_list->type == TOKEN_WORD)
			printf("%zu:\t[%s][%s]\n", i, get_token_type_str(token_list->type), token_list->word);
		else
			printf("%zu:\t[%s]\n", i, get_token_type_str(token_list->type));
		token_list = token_list->next;
		i++;
	}
}

int	main(int ac, char **av, char **ep)
{
	t_minishell	data;
	t_token		*token_list;

	if (ac != 3)
	{
		printf("usage: ./a.out (last_return) (input)\n");
		return (0);
	}
	data.env = ep;
	data.last_return = (unsigned char)atoi(av[1]);
	data.line = av[2];
	printf("Input:       [%s]\n", data.line);
	printf("Last return: [%s]\n", av[1]);
	printf("Tokens:\n");
	token_list = tokenize(&data);
	if (!token_list)
		printf("(empty)\n");
	print_token_list(token_list);
	cleanup_token_list(token_list);
}
