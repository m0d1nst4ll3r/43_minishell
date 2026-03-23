/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokenize_fill.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 18:43:47 by rapohlen          #+#    #+#             */
/*   Updated: 2026/03/18 17:03:52 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_word(char *line, size_t *i, char *word)
{
	t_parse_state	state;
	size_t			len;

	state = STATE_NONE;
	len = 0;
	while (!is_end_of_word(line[*i], state))
	{
		if ((line[*i] == '\'' && state != STATE_DQUOTE)
			|| (line[*i] == '"' && state != STATE_QUOTE))
			update_state(line[*i], &state, i);
		else
			word[len++] = line[(*i)++];
	}
}
