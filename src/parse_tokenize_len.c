/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize_len.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdemouge <bdemouge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:43:47 by rapohlen          #+#    #+#             */
/*   Updated: 2026/04/10 17:33:45 by bdemouge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	check_state(t_parse_state state, size_t len, int had_quotes, int *last_return)
{
	if (state != STATE_NONE)
	{
		if (state == STATE_QUOTE)
			print_error(ERR_UNFQUOTE);
		if (state == STATE_DQUOTE)
			print_error(ERR_UNFDQUOTE);
		*last_return = 2;
		return (-2);
	}
	if (!len && !had_quotes)
		return (-1);
	return (len);
}

int	get_word_len(t_minishell *d, size_t i)
{
	t_parse_state	state;
	size_t			len;
	int				had_quotes;

	state = STATE_NONE;
	had_quotes = 0;
	len = 0;
	while (!is_end_of_word(d->line[i], state))
	{
		if ((d->line[i] == '\'' && state != STATE_DQUOTE)
			|| (d->line[i] == '"' && state != STATE_QUOTE))
		{
			had_quotes = 1;
			update_state(d->line[i], &state, &i);
		}
		else if (state != STATE_QUOTE && is_valid_envar_syntax(d->line + i))
			get_expanded_envar_len(d->line, d, &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	return (check_state(state, len, had_quotes, &d->last_return));
}
