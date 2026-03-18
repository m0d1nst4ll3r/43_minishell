/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 19:02:05 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:17:03 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_word_len(char *line, size_t i)
{
	t_parse_state	state;
	size_t			len;

	state = STATE_NONE;
	len = 0;
	while (!is_end_of_word(line[i], state))
	{
		if ((line[i] == '\'' && state != STATE_DQUOTE)
			|| (line[i] == '"' && state != STATE_QUOTE))
			update_state(line[i], &state, &i);
		else
		{
			i++;
			len++;
		}
	}
	if (state != STATE_NONE)
	{
		if (state == STATE_QUOTE)
			print_error(ERR_UNFQUOTE);
		if (state == STATE_DQUOTE)
			print_error(ERR_UNFDQUOTE);
		return (-1);
	}
	return (len);
}

static int	get_token_word(char *line, size_t *i, t_token *new)
{
	int	word_len;

	word_len = get_word_len(line, *i);
	if (word_len == -1)
		return (0);
	new->word = ft_malloc(word_len + 1);
	if (!new->word)
	{
		free(new);
		return (0);
	}
	new->word[word_len] = 0;
	fill_word(line, i, new->word);
	return (1);
}

static t_token	*get_token(char *line, size_t *i)
{
	t_token	*new;

	new = ft_malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->word = NULL;
	new->type = get_token_type(line + *i);
	if (new->type == TOKEN_WORD)
	{
		if (!get_token_word(line, i, new))
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

t_token	*tokenize(char *line)
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
			cleanup_token_list(token_list, 1);
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
