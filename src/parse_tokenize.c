/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 19:02:05 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 22:28:29 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_token_word(t_minishell *d, size_t *i, t_token *new)
{
	int	word_len;

	word_len = get_word_len(d, *i);
	if (word_len == -2)
		return (0);
	if (word_len == -1)
	{
		new->type = TOKEN_EMPTY;
		fill_word(d, i, new->word);
		return (1);
	}
	new->word = malloc(word_len + 1);
	if (!new->word)
	{
		free(new);
		return (0);
	}
	fill_word(d, i, new->word);
	return (1);
}

static t_token	*get_token(t_minishell *d, size_t *i)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->word = NULL;
	new->type = get_token_type(d->line + *i);
	if (new->type == TOKEN_WORD)
	{
		if (!get_token_word(d, i, new))
			return (NULL);
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

t_token	*tokenize(t_minishell *d)
{
	t_token	*token_list;
	t_token	*new_node;
	t_token	*last;
	size_t	i;

	token_list = NULL;
	i = 0;
	while (skip_whitespace(d->line, &i))
	{
		new_node = get_token(d, &i);
		if (!new_node)
		{
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
