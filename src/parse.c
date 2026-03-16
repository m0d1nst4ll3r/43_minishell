/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:30:10 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 14:52:52 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	get_token_type(char *line)
{
	if (line[0] == '|')
		return (TOKEN_PIPE);
	else if (line[0] == '>' && line[1] == '>')
		return (TOKEN_GREATERGREATER);
	else if (line[0] == '>')
		return (TOKEN_GREATER);
	else if (line[0] == '<' && line[1] == '<')
		return (TOKEN_LESSERLESSER);
	else if (line[0] == '<')
		return (TOKEN_LESSER);
	else
		return (TOKEN_WORD);
}

static size_t	get_env_var_len(char *key, char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (key[j] && env[i][j] == key[j])
			j++;
		if (!key[j] && env[i][j] == '=')
			return (ft_strlen(env[i]) - ft_strlen(key) - 1));
		i++;
	}
	return (0);
}

typedef enum e_parse_state
{
	QUOTE,
	DQUOTE,
	NONE
}	t_parse_state;

static size_t	get_word_len(char *line)
{
	t_parse_state	state;

	state = NONE;
}

static char	*get_word(char *line, size_t *i)
{
}

static t_token	*get_token(char *line, size_t *i)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->word = NULL;
	new->type = get_token_type(line + *i);
	if (new->type == TOKEN_WORD)
	{
		new->word = get_word(line, i);
		if (!new->word)
		{
			free(new);
			return (NULL);
		}
	}
	else if (new->type == TOKEN_LESSERLESSER
		|| new->type == TOKEN_GREATERGREATER)
		(*i) += 2;
	else
		(*i) += 1;
	return (new);
}

static char	skip_whitespace(char *line, size_t *i)
{
	while (line[*i] == ' '
		|| line[*i] == '\t'
		|| line[*i] == '\n')
		(*i)++;
	return (line[*i]);
}

static t_token	*tokenize(char *line)
{
	t_token	*token_list;
	t_token	*new_node;
	t_token	*last;
	size_t	i;

	token_list = NULL;
	i = 0;
	while (skip_whitespace(line, &i))
	{
		new_node = get_token(line, &i);
		if (!new_node)
		{
			print_error(ERR_MALLOC);
			cleanup_token_list(token_list);
			return (NULL);
		}
		if (!token_list)
			token_list = new_node;
		else
			last->next = new_node;
		last = new_node;
	}
	return (token_list);
}

static t_command	*construct_cmd_list(t_token *token_list)
{
	(void)token_list;
	return (NULL);
}

static void	cleanup_token_list(t_token *token_list)
{
	t_token	*last;

	while (token_list)
	{
		last = token_list;
		token_list = token_list->next;
		free(last->file);
		free(last);
	}
}

t_command	*parse(char *line)
{
	t_token		*token_list;
	t_command	*cmd_list;

	token_list = tokenize(line);
	cmd_list = construct_cmd_list(token_list);
	cleanup_token_list(token_list);
	return (cmd_list);
}

/*	Parser
 *
 * Lines follow
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/
