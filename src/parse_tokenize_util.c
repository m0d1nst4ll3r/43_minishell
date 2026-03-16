/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:59:11 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/16 19:39:47 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_token_type(char *line)
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

int	is_end_of_word(char c, t_parse_state state)
{
	if (!c
		|| (state == STATE_NONE
			&& (c == ' ' || c == '\t' || c == '\n'
				|| c == '|' || c == '>' || c == '<')))
		return (1);
	return (0);
}

void	update_state(char c, t_parse_state *state, size_t *i)
{
	if (c == '\'')
	{
		if (*state == STATE_QUOTE)
			*state = STATE_NONE;
		else if (*state == STATE_NONE)
			*state = STATE_QUOTE;
	}
	else if (c == '"')
	{
		if (*state == STATE_DQUOTE)
			*state = STATE_NONE;
		else if (*state == STATE_NONE)
			*state = STATE_DQUOTE;
	}
	(*i)++;
}

void	cleanup_token_list(t_token *token_list)
{
	t_token	*last;

	while (token_list)
	{
		last = token_list;
		token_list = token_list->next;
		free(last->word);
		free(last);
	}
}
